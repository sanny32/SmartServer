//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef RTUMODBUSSERVER_H
#define RTUMODBUSSERVER_H

#include <memory>
#include <QSerialPort>
#include <QObject>
#include "smartcardinfo.h"
#include "rtumodbusdatabuffer.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QModbusRtuSerialSlave>
    typedef QModbusRtuSerialSlave QModbusRtuSerialServer;
#else
    #include <QModbusRtuSerialServer>
#endif


///
/// \brief The RtuModbusServer class
///
class RtuModbusServer : public QModbusRtuSerialServer
{
    Q_OBJECT

public:
    RtuModbusServer();
    virtual ~RtuModbusServer();

    ///
    /// \brief Добавление информации о смарт-карте в регистры Modbus RTU
    /// \param smi
    ///
    void addSmartCardInfo(const SmartCardInfo& smi);

    ///
    /// \brief Создение адресного пространства Modbus RTU сервера
    /// \param type - тип регистров (3x, 4x)
    /// \param start - начальный адрес (начиная с 0)
    /// \param count - количество регистров
    /// \param alignment - выравнивание (количество регистров, кторое занимает ID смарт-карты)
    ///
    void createRegisters(QModbusDataUnit::RegisterType type, quint16 start, quint16 count, quint8 alignment = 1);

private:    
    std::unique_ptr<RtuModbusDataBuffer> _buffer;
};

#endif // RTUMODBUSSERVER_H
