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
    ,_currentAddress(startAddress)
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
/// \brief RtuModbusDataBuffer::nextAddress
/// \return
///
quint16 RtuModbusDataBuffer::nextAddress()
{
    _currentAddress += _alignment;
    return (_currentAddress - _alignment) % _bufferSize ;
}

///
/// \brief RtuModbusDataBuffer::registerType
/// \return
///
QModbusDataUnit::RegisterType RtuModbusDataBuffer::registerType() const
{
    return _regType;
}
