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
    unsigned long length() const;

    virtual QString name() const = 0;

    virtual QByteArray send(SCARDHANDLE hCard) = 0;
    virtual QByteArray send(SCARDHANDLE hCard, unsigned long actProtocol) = 0;

protected:
    SmartCardCommand(const QByteArray& bytes);

private:
    QByteArray _command;
};

#endif // SMARTCARDCOMMAND_H
