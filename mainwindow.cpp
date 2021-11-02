//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <thread>
#include <QDebug>
#include <QSerialPortInfo>
#include <QAbstractEventDispatcher>
#include "applogger.h"
#include "smartcarderror.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,_hContext(0)
{
    ui->setupUi(this);
    AppLogger::getInstance().setup(*qApp, ui->logWidget);

    // событие, генерируемое при бездействии основного GUI-потока приложения
    auto dispatcher = QAbstractEventDispatcher::instance();
    connect(dispatcher, SIGNAL(awake()), this, SLOT(on_awake()));

    try
    {
        const auto retCode = SCardEstablishContext(SCARD_SCOPE_USER, nullptr, nullptr, &_hContext);
        if(retCode != SCARD_S_SUCCESS)
        {
            throw SmartCardError(retCode, "Ошибка инициализации контекста");
        }

        updateSmartReaderSelector();
        updateSerialPortSelector();
    }
    catch (std::exception& ex)
    {
        qCritical().noquote() << ex.what();
    }
}

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{    
    SCardReleaseContext(_hContext);
    delete ui;
}

///
/// \brief MainWindow::on_awake
///
void MainWindow::on_awake()
{
}

///
/// \brief MainWindow::on_refreshSmartReaders_clicked
///
void MainWindow::on_refreshSmartReaders_clicked()
{
    updateSmartReaderSelector();
}

///
/// \brief MainWindow::on_smartReaderSelector_currentTextChanged
/// \param text
///
void MainWindow::on_smartReaderSelector_currentTextChanged(const QString& text)
{
    _smartCardDevice = std::make_unique<SmartCardDevice>(_hContext, text);
    connect(_smartCardDevice.get(), &SmartCardDevice::smartCardDetected, this, &MainWindow::on_smartCardDetected);
}

///
/// \brief MainWindow::on_serialPortSelector_currentTextChanged
/// \param text
///
void MainWindow::on_serialPortSelector_currentTextChanged(const QString& text)
{
    Q_UNUSED(text)
    createRtuModbusServer();
}

///
/// \brief MainWindow::on_smartCardDetected
/// \param smi
///
void MainWindow::on_smartCardDetected(SmartCardInfo smi)
{
    qInfo().noquote().nospace() << "Обнаружена смарт-карта [" << smi.id().toString() << "]";
    _rtuModbusServer->addSmartCardInfo(smi);
}

///
/// \brief MainWindow::updateSmartReaderSelector
///
void MainWindow::updateSmartReaderSelector()
{
    ui->smartReaderSelector->clear();

    try
    {
        LPTSTR pmszReaders = nullptr;
        unsigned long size = SCARD_AUTOALLOCATE;
        const auto retCode = SCardListReaders(_hContext, nullptr, (LPTSTR)&pmszReaders, &size);

        if(retCode == SCARD_S_SUCCESS)
        {
            LPTSTR pReader = pmszReaders;
            while (pReader != nullptr && '\0' != *pReader)
            {
                const int len = (int)wcslen((wchar_t *)pReader);
                ui->smartReaderSelector->addItem(QString::fromWCharArray(pReader, len));

                pReader += len + 1;
            }

            SCardFreeMemory(_hContext, pmszReaders);
        }
        else
        {
            throw SmartCardError(retCode, "Ошибка получения списка устройств чтения смарт-карт");
        }
    }
    catch(std::exception& ex)
    {
        qCritical().noquote() << ex.what();
    }
}

///
/// \brief MainWindow::updateSerialPortSelector
///
void MainWindow::updateSerialPortSelector()
{
    ui->serialPortSelector->clear();
    const auto serialPorts = QSerialPortInfo::availablePorts();

    for(auto&& serialPort : serialPorts)
    {
        ui->serialPortSelector->addItem(serialPort.portName());
    }
}

///
/// \brief MainWindow::setupModbusTableWidget
///
void MainWindow::setupModbusTableWidget()
{
    const auto startAddress = ui->startAddress->text().toUShort();
    const auto bufferSize = ui->bufferSize->text().toUShort();

    ui->modbusTableWidget->clear();

    ui->modbusTableWidget->setColumnCount(4);
    ui->modbusTableWidget->setRowCount(bufferSize);

    for(int i = 0; i < ui->modbusTableWidget->columnCount(); i++)
    {
        ui->modbusTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
    }

    for(int i = 0; i < ui->modbusTableWidget->rowCount(); i++)
    {
        const int address = startAddress + i * ui->modbusTableWidget->columnCount();
        ui->modbusTableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString("3%1").arg(address, 4, 10, QLatin1Char('0'))));

        for(int j = 0; j < ui->modbusTableWidget->columnCount(); j++)
        {
            auto item = new QTableWidgetItem("00");
            item->setTextAlignment(Qt::AlignCenter);

            ui->modbusTableWidget->setItem(i, j, item);
        }
    }
}

///
/// \brief MainWindow::createRtuModbusServer
///
void MainWindow::createRtuModbusServer()
{
    _rtuModbusServer = std::make_unique<RtuModbusServer>();
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter, ui->serialPortSelector->currentText());
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, 19200);
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);

    const auto startAddress = ui->startAddress->text().toUShort();
    const auto bufferSize = ui->bufferSize->text().toUShort();
    _rtuModbusServer->createRegisters(QModbusDataUnit::InputRegisters, startAddress - 1, bufferSize * 4);

    _rtuModbusServer->connectDevice();

    setupModbusTableWidget();
}
