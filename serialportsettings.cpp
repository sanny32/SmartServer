//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "serialportsettings.h"

///
/// \brief SerialPortSettings::SerialPortSettings
///
SerialPortSettings::SerialPortSettings()
    :_baudRate(QSerialPort::Baud9600)
    ,_dataBits(QSerialPort::Data8)
    ,_parity(QSerialPort::NoParity)
    ,_stopBits(QSerialPort::OneStop)
{
}

///
/// \brief SerialPortSettings::baudRate
/// \return
///
QSerialPort::BaudRate SerialPortSettings::baudRate() const
{
    return _baudRate;
}

///
/// \brief SerialPortSettings::setBaudRate
/// \param value
///
void SerialPortSettings::setBaudRate(QSerialPort::BaudRate value)
{
    _baudRate = value;
}

///
/// \brief SerialPortSettings::dataBits
/// \return
///
QSerialPort::DataBits SerialPortSettings::dataBits() const
{
    return _dataBits;
}

///
/// \brief SerialPortSettings::setDataBits
/// \param value
///
void SerialPortSettings::setDataBits(QSerialPort::DataBits value)
{
    _dataBits = value;
}

///
/// \brief SerialPortSettings::parity
/// \return
///
QSerialPort::Parity SerialPortSettings::parity() const
{
    return _parity;
}

///
/// \brief SerialPortSettings::setParity
/// \param value
///
void SerialPortSettings::setParity(QSerialPort::Parity value)
{
    _parity = value;
}

///
/// \brief SerialPortSettings::stopBits
/// \return
///
QSerialPort::StopBits SerialPortSettings::stopBits() const
{
    return _stopBits;
}

///
/// \brief SerialPortSettings::setStopBits
/// \param value
///
void SerialPortSettings::setStopBits(QSerialPort::StopBits value)
{
    _stopBits = value;
}
