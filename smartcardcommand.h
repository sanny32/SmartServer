//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SMARTCARDCOMMAND_H
#define SMARTCARDCOMMAND_H

#include <QByteArray>
#include <winscard.h>

class SmartCardCommand
{
public:
    QByteArray command() const;
    size_t length() const;

    QByteArray send(SCARDHANDLE hCard, unsigned long actProtocol);

protected:
    SmartCardCommand(const QByteArray& bytes);

    virtual QByteArray processResponse(const BYTE* response, unsigned long size) = 0;

private:
    QByteArray _command;
};

#endif // SMARTCARDCOMMAND_H
