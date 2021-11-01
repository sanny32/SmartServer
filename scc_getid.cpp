//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "scc_getid.h"
#include "smartcarderror.h"

///
/// \brief SCC_GetId::SCC_GetId
///
SCC_GetId::SCC_GetId()
    :SmartCardCommand(QByteArray("\xFF\xCA\x00\x00\x00", 5))
{
}

///
/// \brief SmartCardCommand::send
/// \param hCard
/// \param actProtocol
/// \return
///
QByteArray SCC_GetId::send(SCARDHANDLE hCard, unsigned long actProtocol)
{
    SCARD_IO_REQUEST ioRequest;
    ioRequest.dwProtocol = actProtocol;
    ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);

    BYTE recvBuff[262];
    unsigned long recvLen = 262;
    const auto retCode = SCardTransmit(hCard, &ioRequest, (LPCBYTE)command().data(), length(), nullptr, recvBuff, &recvLen);
    if(retCode != SCARD_S_SUCCESS)
    {
        throw SmartCardError(retCode, QString("Команда %1. Функция SCardTransmit возвратила ошибку").arg(name()));
    }

    if(recvLen > 2)
    {
        return QByteArray((char*)recvBuff, recvLen - 2);
    }
    else
    {
        const auto error = QString("Команда %1. Некорректный размер ответа").arg(name());
        throw std::runtime_error(error.toStdString());
    }
}

///
/// \brief SmartCardCommand::send
/// \param hCard
/// \return
///
QByteArray SCC_GetId::send(SCARDHANDLE hCard)
{
    Q_UNUSED(hCard)
    throw std::runtime_error("Not implemented");
}
