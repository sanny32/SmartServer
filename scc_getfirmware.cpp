//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "scc_getfirmware.h"
#include "smartcarderror.h"

///
/// \brief SCC_GetFirmware::SCC_GetFirmware
///
SCC_GetFirmware::SCC_GetFirmware()
    :SmartCardCommand(QByteArray("\xE0\x00\x00\x18\x00", 5))
{
}

///
/// \brief SCC_GetFirmware::send
/// \param hCard
/// \param actProtocol
/// \return
///
QByteArray SCC_GetFirmware::send(SCARDHANDLE hCard)
{
    BYTE recvBuff[262];
    unsigned long recvLen = 262;
    const auto retCode = SCardControl(hCard, 0x310000 + 3500*4, (LPCBYTE)command().data(), length(), recvBuff, recvLen, &recvLen);
    if(retCode != SCARD_S_SUCCESS)
    {
        throw SmartCardError(retCode, QString("Команда %1. Функция SCardControl возвратила ошибку").arg(name()));
    }

    if(recvLen > 5)
    {
        return QByteArray((char*)recvBuff + 5, recvLen - 5);
    }
    else
    {
        const auto error = QString("Команда %1. Некорректный размер ответа").arg(name());
        throw std::runtime_error(error.toStdString());
    }
}

///
/// \brief SCC_GetFirmware::send
/// \param hCard
/// \param actProtocol
/// \return
///
QByteArray SCC_GetFirmware::send(SCARDHANDLE hCard, unsigned long actProtocol)
{
    Q_UNUSED(hCard)
    Q_UNUSED(actProtocol)
    throw std::runtime_error("Not implemented");
}
