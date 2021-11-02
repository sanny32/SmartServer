//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QDebug>
#include "rtumodbusserver.h"

///
/// \brief RtuModbusServer::RtuModbusServer
///
RtuModbusServer::RtuModbusServer()
{
    connect(this, &RtuModbusServer::stateChanged, this, &RtuModbusServer::on_stateChanged);
    connect(this, &RtuModbusServer::dataWritten, this,  &RtuModbusServer::on_dataWritten);
}

///
/// \brief RtuModbusServer::addSmartCardInfo
/// \param smi
///
void RtuModbusServer::addSmartCardInfo(const SmartCardInfo& smi)
{
    const auto id = smi.id();
    const auto addr = _reg[QModbusDataUnit::InputRegisters].value(0);
    for(int i = 0; i < id.size() - 1; i+=2)
    {
        const quint16 value = id[i] | id[i + 1] << 8;
        setData(QModbusDataUnit::InputRegisters, addr + i % 2, value);
    }
}

///
/// \brief RtuModbusServer::createRegisters
/// \param type
/// \param start
/// \param count
///
void RtuModbusServer::createRegisters(QModbusDataUnit::RegisterType type, quint16 start, quint16 count)
{
    _reg.insert(type, { type, start, count });
    setMap(_reg);
}

///
/// \brief RtuModbusServer::on_stateChanged
/// \param state
///
void RtuModbusServer::on_stateChanged(QModbusDevice::State state)
{
    qDebug() << state;
}

///
/// \brief RtuModbusServer::on_dataWritten
/// \param table
/// \param address
/// \param size
///
void RtuModbusServer::on_dataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    qDebug() << table << address << size;
}
