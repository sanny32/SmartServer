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
#include "dialogserialportsettings.h"
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
        updateAddressTypeSelector();
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
/// \brief MainWindow::on_startAddress_textEdited
/// \param text
///
void MainWindow::on_startAddress_textEdited(const QString& text)
{
    bool ok = false;
    const auto address = text.toShort(&ok);

    if(ok && address > 0)
    {
        createRtuModbusServer();
    }
}

///
/// \brief MainWindow::on_bufferSize_textEdited
/// \param text
///
void MainWindow::on_bufferSize_textEdited(const QString& text)
{
    bool ok = false;
    const auto size = text.toShort(&ok);

    if(ok && size > 0)
    {
        createRtuModbusServer();
    }
}

///
/// \brief MainWindow::on_addressTypeSelector_currentIndexChanged
/// \param index
///
void MainWindow::on_addressTypeSelector_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    createRtuModbusServer();
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
/// \brief MainWindow::on_serialPortSettings_clicked
///
void MainWindow::on_serialPortSettings_clicked()
{
    DialogSerialPortSettings dlg(_serialPotSettings, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        createRtuModbusServer();
    }
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
/// \brief MainWindow::on_rtuModbusServerDataWritten
/// \param table
/// \param address
/// \param size
///
void MainWindow::on_rtuModbusServerDataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    const auto startAddress = ui->startAddress->text().toUShort() - 1;
    const auto columnCount = ui->modbusTableWidget->columnCount();

    for(int i = 0; i < size; i++)
    {
        quint16 data;
        if(_rtuModbusServer->data(table, address + i, &data))
        {
            const int col = (address + i - startAddress) % columnCount;
            const int row = (address + i - startAddress) / columnCount;
            const auto text = QString("%1H").arg(data, 4, 16, QLatin1Char('0'));

            auto item = ui->modbusTableWidget->item(row, col);
            item->setText(text.toUpper());
        }
    }
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

    ui->serialPortSettings->setEnabled(!serialPorts.isEmpty());
}

///
/// \brief MainWindow::updateAddressTypeSelector
///
void MainWindow::updateAddressTypeSelector()
{
    ui->addressTypeSelector->clear();
    ui->addressTypeSelector->addItem("3x", QModbusDataUnit::InputRegisters);
    ui->addressTypeSelector->addItem("4x", QModbusDataUnit::HoldingRegisters);
}

///
/// \brief MainWindow::setupModbusTableWidget
///
void MainWindow::setupModbusTableWidget()
{
    const auto startAddress = ui->startAddress->text().toUShort();
    const auto bufferSize = ui->bufferSize->text().toUShort();
    const auto addressType = ui->addressTypeSelector->currentData().value<QModbusDataUnit::RegisterType>();

    ui->modbusTableWidget->clear();
    ui->modbusTableWidget->setColumnCount(_dataAlignmnet);
    ui->modbusTableWidget->setRowCount(bufferSize);

    for(int i = 0; i < ui->modbusTableWidget->columnCount(); i++)
    {
        const auto text = QString::number(i);
        ui->modbusTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(text));
    }

    for(int i = 0; i < ui->modbusTableWidget->rowCount(); i++)
    {        
        QString addressPrefix;
        switch (addressType)
        {
            case QModbusDataUnit::InputRegisters:
                addressPrefix = "3";
            break;

            case QModbusDataUnit::HoldingRegisters:
                addressPrefix = "4";
            break;

            default:
            break;
        }

        const int address = startAddress + i * ui->modbusTableWidget->columnCount();
        const auto addressText = QString("%1").arg(address, 4, 10, QLatin1Char('0'));
        ui->modbusTableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1%2").arg(addressPrefix, addressText)));

        for(int j = 0; j < ui->modbusTableWidget->columnCount(); j++)
        {
            auto item = new QTableWidgetItem("0000H");
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
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, _serialPotSettings.baudRate());
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, _serialPotSettings.dataBits());
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter, _serialPotSettings.parity());
    _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, _serialPotSettings.stopBits());

    const auto startAddress = ui->startAddress->text().toUShort();
    const auto bufferSize = ui->bufferSize->text().toUShort();
    const auto addressType = ui->addressTypeSelector->currentData().value<QModbusDataUnit::RegisterType>();
    _rtuModbusServer->createRegisters(addressType, startAddress - 1, bufferSize * _dataAlignmnet, _dataAlignmnet);

    connect(_rtuModbusServer.get(), &RtuModbusServer::dataWritten, this, &MainWindow::on_rtuModbusServerDataWritten);
    _rtuModbusServer->connectDevice();

    setupModbusTableWidget();
}
