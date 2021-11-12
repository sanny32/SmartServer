//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "rtumodbusdatabuffer.h"

///
/// \brief RtuModbusDataBuffer::RtuModbusDataBuffer
/// \param type
/// \param startAddress
/// \param bufferSize
///
RtuModbusDataBuffer::RtuModbusDataBuffer(QModbusDataUnit::RegisterType type, quint16 startAddress, quint16 bufferSize)
    :_alignment(1)
    ,_startAddress(startAddress)
    ,_nextAddress(startAddress + 2)
    ,_bufferSize(bufferSize)
    ,_regType(type)
{
    _reg.insert(type, { type, startAddress, bufferSize });
}

///
/// \brief RtuModbusDataBuffer::getModbusDataUnitMap
/// \return
///
QModbusDataUnitMap RtuModbusDataBuffer::getModbusDataUnitMap() const
{
    return _reg;
}

///
/// \brief RtuModbusDataBuffer::dataAlignment
/// \return
///
quint8 RtuModbusDataBuffer::dataAlignment() const
{
    return _alignment;
}

///
/// \brief RtuModbusDataBuffer::setDataAlignment
/// \param alignment
///
void RtuModbusDataBuffer::setDataAlignment(quint8 alignment)
{
    _alignment = alignment;
}

///
/// \brief RtuModbusDataBuffer::startAddress
/// \return
///
quint16 RtuModbusDataBuffer::startAddress() const
{
    return _startAddress;
}

///
/// \brief RtuModbusDataBuffer::nextAddress
/// \return
///
quint16 RtuModbusDataBuffer::nextAddress()
{
    const auto nextAddress = _nextAddress;

    _nextAddress += _alignment; 
    if(_nextAddress >= _startAddress + _bufferSize)
    {
        _nextAddress = _startAddress + 2;
    }

    return nextAddress;
}

///
/// \brief RtuModbusDataBuffer::registerType
/// \return
///
QModbusDataUnit::RegisterType RtuModbusDataBuffer::registerType() const
{
    return _regType;
}
