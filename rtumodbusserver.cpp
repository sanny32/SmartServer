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
/// \brief RtuModbusServer::~RtuModbusServer
///
RtuModbusServer::~RtuModbusServer()
{
    disconnectDevice();
}

///
/// \brief RtuModbusServer::addSmartCardInfo
/// \param smi
///
void RtuModbusServer::addSmartCardInfo(const SmartCardInfo& smi)
{
    const auto id = smi.id().toUInts();
    const auto address = _buffer->nextAddress();
    for(int i = 0; i < id.size(); i++)
    {        
        setData(_buffer->registerType(), address + i, id[i]);
    }
}

///
/// \brief RtuModbusServer::createRegisters
/// \param type
/// \param start
/// \param count
/// \param alignment
///
void RtuModbusServer::createRegisters(QModbusDataUnit::RegisterType type, quint16 start, quint16 count, quint8 alignment)
{   
    _buffer = std::make_unique<RtuModbusDataBuffer>(type, start, count);
    _buffer->setDataAlignment(alignment);
    setMap(_buffer->getModbusDataUnitMap());
}

///
/// \brief RtuModbusServer::on_stateChanged
/// \param state
///
void RtuModbusServer::on_stateChanged(QModbusDevice::State state)
{
    //qDebug() << state;
}

///
/// \brief RtuModbusServer::on_dataWritten
/// \param table
/// \param address
/// \param size
///
void RtuModbusServer::on_dataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    //qDebug() << table << address << size;
}
