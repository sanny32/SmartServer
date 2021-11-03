//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef RTUMODBUSSERVER_H
#define RTUMODBUSSERVER_H

#include <QSerialPort>
#include <QModbusRtuSerialSlave>
#include <QObject>
#include "smartcardinfo.h"
#include "rtumodbusdatabuffer.h"

///
/// \brief The RtuModbusServer class
///
class RtuModbusServer : public QModbusRtuSerialSlave
{
    Q_OBJECT

public:
    RtuModbusServer();
    virtual ~RtuModbusServer();

    void addSmartCardInfo(const SmartCardInfo& smi);
    void createRegisters(QModbusDataUnit::RegisterType type, quint16 start, quint16 count, quint8 alignment = 1);

private slots:
    void on_stateChanged(QModbusDevice::State state);
    void on_errorOccurred(QModbusDevice::Error error);

private:    
    std::unique_ptr<RtuModbusDataBuffer> _buffer;
};

#endif // RTUMODBUSSERVER_H
