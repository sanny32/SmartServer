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
    :_restartTimeout(0)
    ,_lastRequestTime(QDateTime::currentDateTime().toSecsSinceEpoch())
{
    connect(&_restartTimer, &QTimer::timeout, this, &RtuModbusServer::on_timeout);
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

    setData(_buffer->registerType(), _buffer->startAddress(), address + 1);
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
/// \brief RtuModbusServer::restartTimeout
/// \return
///
int RtuModbusServer::restartTimeout() const
{
    return _restartTimeout;
}

///
/// \brief RtuModbusServer::setRestartTimeout
/// \param value
///
void RtuModbusServer::setRestartTimeout(int value)
{
    _restartTimer.stop();
    _restartTimeout = value;
    _restartTimer.start(1000);
}

///
/// \brief RtuModbusServer::processRequest
/// \param request
/// \return
///
QModbusResponse RtuModbusServer::processRequest(const QModbusPdu &request)
{
    _lastRequestTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    return QModbusRtuSerialServer::processRequest(request);
}

///
/// \brief RtuModbusServer::on_timeout
///
void RtuModbusServer::on_timeout()
{
    if(_restartTimeout == 0) return;
    if(state() != QModbusDevice::ConnectedState) return;

    const auto nowTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    if(nowTime - _lastRequestTime > _restartTimeout)
    {
        disconnectDevice();
        connectDevice();

        _lastRequestTime = nowTime;
        emit restarted();
    }
}
