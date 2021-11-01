//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "scc_getid.h"

///
/// \brief SCC_GetId::SCC_GetId
///
SCC_GetId::SCC_GetId()
    :SmartCardCommand(QByteArray("\xFF\xCA\x00\x00\x00", 5))
{
}

///
/// \brief SCC_GetId::processResponse
/// \param response
/// \param size
/// \return
///
QByteArray SCC_GetId::processResponse(const BYTE* response, unsigned long size)
{
    if(size > 2)
    {
        return QByteArray((char*)response, size - 2);
    }
    else
    {
        throw std::runtime_error("Некорректный размер ответа");
    }
}
