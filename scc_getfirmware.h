//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SCC_GETFIRMWARE_H
#define SCC_GETFIRMWARE_H

#include <QString>
#include "smartcardcommand.h"

class SCC_GetFirmware final : public SmartCardCommand
{
public:
    SCC_GetFirmware();

    QString name() const override
    {
        return "GETFIRMWARE";
    }

    QByteArray send(SCARDHANDLE hCard) override;

private:
    QByteArray send(SCARDHANDLE hCard, unsigned long actProtocol) override;

};

#endif // SCC_GETFIRMWARE_H
