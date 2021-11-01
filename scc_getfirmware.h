//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SCC_GETFIRMWARE_H
#define SCC_GETFIRMWARE_H

#include "smartcardcommand.h"

class SCC_GetFirmware : public SmartCardCommand
{
public:
    SCC_GetFirmware();

protected:
    QByteArray processResponse(const BYTE* response, unsigned long size) override;
};

#endif // SCC_GETFIRMWARE_H
