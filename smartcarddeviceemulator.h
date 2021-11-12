//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SMARTCARDDEVICEEMULATOR_H
#define SMARTCARDDEVICEEMULATOR_H

#include <QTimer>
#include "smartcarddevice.h"

///
/// \brief Класс эмулирования устройства чтения смарт-карт
///
class SmartCardDeviceEmulator : public SmartCardDevice
{
    Q_OBJECT

public:
    explicit SmartCardDeviceEmulator(quint32 timeout = 1000, QObject *parent = nullptr);
    ~SmartCardDeviceEmulator() override;

private slots:
    void on_timeout();

private:
    QTimer _timer;
};

#endif // SMARTCARDDEVICEEMULATOR_H
