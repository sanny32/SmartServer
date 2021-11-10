//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QDataStream>
#include "serialportsettings.h"

///
/// \brief SerialPortSettings::SerialPortSettings
///
SerialPortSettings::SerialPortSettings(const QString& name)
    :_name(name)
    ,_baudRate(QSerialPort::Baud9600)
    ,_dataBits(QSerialPort::Data8)
    ,_parity(QSerialPort::NoParity)
    ,_stopBits(QSerialPort::OneStop)
{
    static const int reg = qRegisterMetaType<SerialPortSettings>();
    Q_UNUSED(reg)

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static const int regOps = qRegisterMetaTypeStreamOperators<SerialPortSettings>();
    Q_UNUSED(regOps)
#endif
}

///
/// \brief SerialPortSettings::name
/// \return
///
QString SerialPortSettings::name() const
{
    return _name;
}

///
/// \brief SerialPortSettings::setName
/// \param name
///
void SerialPortSettings::setName(const QString& name)
{
    _name = name;
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

///
/// \brief operator <<
/// \param out
/// \param s
/// \return
///
QDataStream& operator<<(QDataStream& out, const SerialPortSettings& s)
{
    out << s._name;
    out << s._baudRate;
    out << s._dataBits;
    out << s._parity;
    out << s._stopBits;
    return out;
}

///
/// \brief operator >>
/// \param in
/// \param s
/// \return
///
QDataStream& operator>>(QDataStream& in, SerialPortSettings& s)
{
    in >> s._name;
    in >> s._baudRate;
    in >> s._dataBits;
    in >> s._parity;
    in >> s._stopBits;
    return in;
}
