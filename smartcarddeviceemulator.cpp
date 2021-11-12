//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include <QRandomGenerator>
#include "smartcarddeviceemulator.h"

///
/// \brief SmartCardDeviceEmulator::SmartCardDeviceEmulator
/// \param timeout
/// \param parent
///
SmartCardDeviceEmulator::SmartCardDeviceEmulator(quint32 timeout, QObject *parent)
    : SmartCardDevice(0, "test", parent)
{
    connect(&_timer, &QTimer::timeout, this, &SmartCardDeviceEmulator::on_timeout);
    _timer.setInterval(timeout);
    _timer.start();
}

///
/// \brief SmartCardDeviceEmulator::~SmartCardDeviceEmulator
///
SmartCardDeviceEmulator::~SmartCardDeviceEmulator()
{
    _timer.stop();
}

///
/// \brief SmartCardDeviceEmulator::on_timeout
///
void SmartCardDeviceEmulator::on_timeout()
{
     const quint32 value = QRandomGenerator::global()->generate();

     QBuffer buf;
     buf.open(QBuffer::ReadWrite);
     QDataStream stream(&buf);
     stream << value;

     SmartCardInfo smi;
     smi.setId(buf.buffer());

     emit smartCardDetected(smi);
}
