//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef RTUMODBUSDATABUFFER_H
#define RTUMODBUSDATABUFFER_H

#include <QModbusDataUnitMap>

///
/// \brief The RtuModbusDataBuffer class
///
class RtuModbusDataBuffer
{
public:
    RtuModbusDataBuffer(QModbusDataUnit::RegisterType type, quint16 startAddress, quint16 bufferSize);

    ///
    /// \brief возвращает карту регистров Modbus
    /// \return
    ///
    QModbusDataUnitMap getModbusDataUnitMap() const;

    ///
    /// \brief выравнивание данных по количеству регистров
    /// \return
    ///
    quint8 dataAlignment() const;

    ///
    /// \brief задает выравнивание данных
    /// \param alignment - количество регистров для выравнивания данных
    ///
    void setDataAlignment(quint8 alignment);

    ///
    /// \brief возвращает следующий адрес для записи
    /// \return
    ///
    quint16 nextAddress();

    ///
    /// \brief возвращает тип регистров в карте Modbus
    /// \return
    ///
    QModbusDataUnit::RegisterType registerType() const;

private:
    quint8 _alignment;
    quint16 _currentAddress;
    quint16 _bufferSize;
    QModbusDataUnitMap _reg;
    QModbusDataUnit::RegisterType _regType;
};

#endif // RTUMODBUSDATABUFFER_H
