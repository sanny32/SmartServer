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

///
/// \brief The RtuModbusServer class
///
class RtuModbusServer : public QModbusRtuSerialSlave
{
    Q_OBJECT

public:
    RtuModbusServer();

    void addSmartCardInfo(const SmartCardInfo& smi);
    void createRegisters(QModbusDataUnit::RegisterType type, quint16 start, quint16 count);    

private slots:
    void on_stateChanged(QModbusDevice::State state);
    void on_dataWritten(QModbusDataUnit::RegisterType table, int address, int size);

private:
    QModbusDataUnitMap _reg;
};

#endif // RTUMODBUSSERVER_H
