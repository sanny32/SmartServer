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
    connect(this, &RtuModbusServer::errorOccurred, this, &RtuModbusServer::on_errorOccurred);
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
    if(_buffer == nullptr)
    {
        return;
    }

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
    Q_UNUSED(state)
}

///
/// \brief RtuModbusServer::on_errorOccurred
/// \param error
///
void RtuModbusServer::on_errorOccurred(QModbusDevice::Error error)
{
    qWarning() << "Ошибка Modbus RTU сервера" << error;
}
