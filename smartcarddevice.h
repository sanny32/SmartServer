//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SMARTCARDDEVICE_H
#define SMARTCARDDEVICE_H

#include <QObject>
#include <Windows.h>
#include "smartcardinfo.h"

class SmartCardDevice : public QObject
{
    Q_OBJECT
public:
    explicit SmartCardDevice(SCARDCONTEXT hContext, const QString& name, QObject *parent = nullptr);
    virtual ~SmartCardDevice();

signals:
    void smartCardDetected(SmartCardInfo smi);

private:
    SmartCardInfo readSmartCardInfo();

private:
    SCARDHANDLE _hCard;
    SCARDCONTEXT _hContext;
    unsigned long _actProtocol;
    QString _name;
    bool _destroy;
};

#endif // SMARTCARDDEVICE_H
