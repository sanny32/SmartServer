//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SCC_GETID_H
#define SCC_GETID_H

#include <QString>
#include "smartcardcommand.h"

class SCC_GetId final : public SmartCardCommand
{
public:
    SCC_GetId();

    QString name() const override
    {
        return "GETID";
    }

    QByteArray send(SCARDHANDLE hCard, unsigned long actProtocol) override;

private:
    QByteArray send(SCARDHANDLE hCard) override;
};

#endif // SCC_GETID_H
