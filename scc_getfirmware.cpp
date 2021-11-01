//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "scc_getfirmware.h"

///
/// \brief SCC_GetFirmware::SCC_GetFirmware
///
SCC_GetFirmware::SCC_GetFirmware()
    :SmartCardCommand(QByteArray("\xE0\x00\x00\x18\x00", 5))
{
}

///
/// \brief SCC_GetFirmware::processResponse
/// \param response
/// \param size
/// \return
///
QByteArray SCC_GetFirmware::processResponse(const BYTE* response, unsigned long size)
{
    if(size > 5)
    {
        return QByteArray((char*)response + 5, size - 5);
    }
    else
    {
        throw std::runtime_error("Некорректный размер ответа");
    }
}
