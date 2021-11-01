//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SCC_GETID_H
#define SCC_GETID_H

#include "smartcardcommand.h"

class SCC_GetId : public SmartCardCommand
{
public:
    SCC_GetId();

protected:
    QByteArray processResponse(const BYTE* response, unsigned long size) override;
};

#endif // SCC_GETID_H
