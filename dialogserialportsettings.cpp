//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "dialogserialportsettings.h"
#include "ui_dialogserialportsettings.h"

///
/// \brief DialogSerialPortSettings::DialogSerialPortSettings
/// \param settings
/// \param parent
///
DialogSerialPortSettings::DialogSerialPortSettings(SerialPortSettings& settings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSerialPortSettings)
    ,_settings(settings)
{
    ui->setupUi(this);

    ui->baudRateSelector->addItem("1200", QSerialPort::Baud1200);
    ui->baudRateSelector->addItem("2400", QSerialPort::Baud2400);
    ui->baudRateSelector->addItem("4800", QSerialPort::Baud4800);
    ui->baudRateSelector->addItem("9600", QSerialPort::Baud9600);
    ui->baudRateSelector->addItem("19200", QSerialPort::Baud19200);
    ui->baudRateSelector->addItem("38400", QSerialPort::Baud38400);
    ui->baudRateSelector->addItem("57600", QSerialPort::Baud57600);
    ui->baudRateSelector->addItem("115200", QSerialPort::Baud115200);

    ui->dataBitsSelector->addItem("5", QSerialPort::Data5);
    ui->dataBitsSelector->addItem("6", QSerialPort::Data6);
    ui->dataBitsSelector->addItem("7", QSerialPort::Data7);
    ui->dataBitsSelector->addItem("8", QSerialPort::Data8);

    ui->paritySelector->addItem("NONE", QSerialPort::NoParity);
    ui->paritySelector->addItem("EVEN", QSerialPort::EvenParity);
    ui->paritySelector->addItem("ODD", QSerialPort::OddParity);

    ui->stopBitsSelector->addItem("1", QSerialPort::OneStop);
    ui->stopBitsSelector->addItem("2", QSerialPort::TwoStop);
    ui->stopBitsSelector->addItem("3", QSerialPort::OneAndHalfStop);

    int idx = ui->baudRateSelector->findData(settings.baudRate());
    ui->baudRateSelector->setCurrentIndex(idx);

    idx = ui->dataBitsSelector->findData(settings.dataBits());
    ui->dataBitsSelector->setCurrentIndex(idx);

    idx = ui->paritySelector->findData(settings.parity());
    ui->paritySelector->setCurrentIndex(idx);

    idx = ui->stopBitsSelector->findData(settings.stopBits());
    ui->stopBitsSelector->setCurrentIndex(idx);
}

///
/// \brief DialogSerialPortSettings::~DialogSerialPortSettings
///
DialogSerialPortSettings::~DialogSerialPortSettings()
{
    delete ui;
}

///
/// \brief DialogSerialPortSettings::accept
///
void DialogSerialPortSettings::accept()
{
    _settings.setBaudRate(ui->baudRateSelector->currentData().value<QSerialPort::BaudRate>());
    _settings.setDataBits(ui->dataBitsSelector->currentData().value<QSerialPort::DataBits>());
    _settings.setParity(ui->paritySelector->currentData().value<QSerialPort::Parity>());
    _settings.setStopBits(ui->stopBitsSelector->currentData().value<QSerialPort::StopBits>());
    close();
}
