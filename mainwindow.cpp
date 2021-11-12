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
#include "appsettings.h"
#include "smartcarderror.h"
#include "smartcarddeviceemulator.h"
#include "dialogabout.h"
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

    updateSmartReaderSelector();
    updateSerialPortSelector();
    updateAddressTypeSelector();

    loadSettings();
}


///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{    
    SCardReleaseContext(_hContext);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

///
/// \brief MainWindow::on_actionExit_triggered
///
void MainWindow::on_actionExit_triggered()
{
    close();
}

///
/// \brief MainWindow::on_actionAbout_triggered
///
void MainWindow::on_actionAbout_triggered()
{
    DialogAbout dlg(this);
    dlg.exec();
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
    if(_hContext != 0)
    {
        _smartCardDevice = std::make_unique<SmartCardDevice>(_hContext, text);
        connect(_smartCardDevice.get(), &SmartCardDevice::smartCardDetected, this, &MainWindow::on_smartCardDetected);
    }
}

///
/// \brief MainWindow::on_useSmartReaderEmulator_stateChanged
/// \param state
///
void MainWindow::on_useSmartReaderEmulator_stateChanged(int state)
{
    if(state)
    {
        ui->smartReaderSelector->clear();
        ui->smartReaderSelector->addItem("Smart Card Device Emulator");
        ui->smartReaderSelector->setEnabled(false);
        ui->refreshSmartReaders->setEnabled(false);

        _smartCardDevice = std::make_unique<SmartCardDeviceEmulator>(2000);
        connect(_smartCardDevice.get(), &SmartCardDevice::smartCardDetected, this, &MainWindow::on_smartCardDetected);
    }
    else
    {
        _smartCardDevice = nullptr;
        ui->smartReaderSelector->setEnabled(true);
        ui->refreshSmartReaders->setEnabled(true);
        updateSmartReaderSelector();
    }
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
    const auto portName = ui->serialPortSelector->currentText();
    DialogSerialPortSettings dlg(_serialPortsSettings[portName], this);
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
/// \brief MainWindow::on_rtuModbusServerStateChanged
/// \param state
///
void MainWindow::on_rtuModbusServerStateChanged(QModbusDevice::State state)
{
    QString modbusState;
    switch(state)
    {
        case QModbusDevice::ConnectingState:
            modbusState = "Запуск...";
        break;

        case QModbusDevice::ConnectedState:
            modbusState = "Запущен";
        break;

        case QModbusDevice::UnconnectedState:
            modbusState = "Остановлен";
        break;

        case QModbusDevice::ClosingState:
            modbusState = "Останов...";
        break;
    }

    ui->statusbar->showMessage(QString("Modbus RTU сервер: %1").arg(modbusState));
}

///
/// \brief MainWindow::on_rtuModbusServerErrorOccurred
/// \param error
///
void MainWindow::on_rtuModbusServerErrorOccurred(QModbusDevice::Error error)
{
    qWarning() << "Ошибка Modbus RTU сервера:" << error;
    if(error == QModbusDevice::ConnectionError)
    {
        _rtuModbusServer->disconnectDevice();
    }
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
/// \brief MainWindow::loadSettings
///
void MainWindow::loadSettings()
{
    const auto smartCardReader = AppSettings::instance()->GetSetting(AppSettings::SmartCardReader).toString();
    ui->smartReaderSelector->setCurrentText(smartCardReader);

    const auto serialPortSettings = AppSettings::instance()->GetSetting(AppSettings::SerialPortSettings).value<SerialPortSettings>();    
    if(!serialPortSettings.name().isEmpty())
    {
        _serialPortsSettings[serialPortSettings.name()] = serialPortSettings;
        ui->serialPortSelector->setCurrentText(serialPortSettings.name());
    }

    const auto modbusAddressType = AppSettings::instance()->GetSetting(AppSettings::ModbusAddressType).toString();
    ui->addressTypeSelector->setCurrentText(modbusAddressType);

    const auto modbusStartAddress = AppSettings::instance()->GetSetting(AppSettings::ModbusStartAddress).toUInt();
    ui->startAddress->setText(QString::number(modbusStartAddress));

    const auto modbusBufferSize = AppSettings::instance()->GetSetting(AppSettings::ModbusBufferSize).toUInt();
    ui->bufferSize->setText(QString::number(modbusBufferSize));

    createRtuModbusServer();
}

///
/// \brief MainWindow::saveSettings
///
void MainWindow::saveSettings()
{
    const auto smartCardReader = ui->smartReaderSelector->currentText();
    AppSettings::instance()->SetSetting(AppSettings::SmartCardReader, smartCardReader);

    const auto portName = ui->serialPortSelector->currentText();
    if(!portName.isEmpty())
    {
        const auto serialPortSettings = _serialPortsSettings[portName];
        AppSettings::instance()->SetSetting(AppSettings::SerialPortSettings, QVariant::fromValue(serialPortSettings));
    }

    const auto modbusAddressType = ui->addressTypeSelector->currentText();
    AppSettings::instance()->SetSetting(AppSettings::ModbusAddressType, modbusAddressType);

    const auto modbusStartAddress = ui->startAddress->text();
    AppSettings::instance()->SetSetting(AppSettings::ModbusStartAddress, modbusStartAddress);

    const auto modbusBufferSize = ui->bufferSize->text();
    AppSettings::instance()->SetSetting(AppSettings::ModbusBufferSize, modbusBufferSize);
}

///
/// \brief MainWindow::updateSmartReaderSelector
///
void MainWindow::updateSmartReaderSelector()
{
    ui->smartReaderSelector->clear();

    try
    {
        if(_hContext != 0)
        {
            SCardReleaseContext(_hContext);
            _hContext = 0;
        }

        auto retCode = SCardEstablishContext(SCARD_SCOPE_USER, nullptr, nullptr, &_hContext);
        if(retCode != SCARD_S_SUCCESS)
        {
            throw SmartCardError(retCode, "Ошибка инициализации контекста");
        }

        LPTSTR pmszReaders = nullptr;
        unsigned long size = SCARD_AUTOALLOCATE;
        retCode = SCardListReaders(_hContext, nullptr, (LPTSTR)&pmszReaders, &size);

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
    _serialPortsSettings.clear();

    const auto serialPorts = QSerialPortInfo::availablePorts();
    for(auto&& serialPort : serialPorts)
    {
        const auto portName = serialPort.portName();
        if(!portName.isEmpty())
        {
            ui->serialPortSelector->addItem(portName);
            _serialPortsSettings[portName] = SerialPortSettings(portName);
        }
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
    ui->modbusTableWidget->setRowCount(bufferSize + 1);

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
    const auto portName = ui->serialPortSelector->currentText();
    if(!portName.isEmpty())
    {
        const auto serialPortSettings = _serialPortsSettings[portName];
        _rtuModbusServer = std::make_unique<RtuModbusServer>();
        _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
        _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, serialPortSettings.baudRate());
        _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, serialPortSettings.dataBits());
        _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter, serialPortSettings.parity());
        _rtuModbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, serialPortSettings.stopBits());

        const auto startAddress = ui->startAddress->text().toUShort();
        const auto bufferSize = ui->bufferSize->text().toUShort();
        const auto addressType = ui->addressTypeSelector->currentData().value<QModbusDataUnit::RegisterType>();
        _rtuModbusServer->createRegisters(addressType, startAddress - 1, bufferSize * _dataAlignmnet + 2, _dataAlignmnet);

        connect(_rtuModbusServer.get(), &RtuModbusServer::dataWritten, this, &MainWindow::on_rtuModbusServerDataWritten);
        connect(_rtuModbusServer.get(), &RtuModbusServer::stateChanged, this, &MainWindow::on_rtuModbusServerStateChanged);
        connect(_rtuModbusServer.get(), &RtuModbusServer::errorOccurred, this, &MainWindow::on_rtuModbusServerErrorOccurred);
        _rtuModbusServer->connectDevice();

        setupModbusTableWidget();
    }
}
