//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SERIALPORTSETTINGS_H
#define SERIALPORTSETTINGS_H

#include <QSerialPort>

///
/// \brief Настройки COM порта
///
class SerialPortSettings
{
public:
    SerialPortSettings();

    QString name() const;
    void setName(const QString& name);

    QSerialPort::BaudRate baudRate() const;
    void setBaudRate(QSerialPort::BaudRate value);

    QSerialPort::DataBits dataBits() const;
    void setDataBits(QSerialPort::DataBits value);

    QSerialPort::Parity parity() const;
    void setParity(QSerialPort::Parity value);

    QSerialPort::StopBits stopBits() const;
    void setStopBits(QSerialPort::StopBits value);

private:
    QString _name;
    QSerialPort::BaudRate _baudRate;
    QSerialPort::DataBits _dataBits;
    QSerialPort::Parity _parity;
    QSerialPort::StopBits _stopBits;
};

#endif // SERIALPORTSETTINGS_H
