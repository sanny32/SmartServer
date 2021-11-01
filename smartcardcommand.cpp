//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QByteArray>
#include "smartcardcommand.h"
#include "smartcarderror.h"

///
/// \brief SmartCardCommand::SmartCardCommand
///
SmartCardCommand::SmartCardCommand(const QByteArray& bytes)
    :_command(bytes)
{
}

///
/// \brief SmartCardCommand::command
/// \return
///
QByteArray SmartCardCommand::command() const
{
    return _command;
}

///
/// \brief SmartCardCommand::length
/// \return
///
size_t SmartCardCommand::length() const
{
    return _command.length();
}

///
/// \brief SmartCardCommand::send
/// \param hCard
/// \param actProtocol
/// \return
///
QByteArray SmartCardCommand::send(SCARDHANDLE hCard, unsigned long actProtocol)
{
    SCARD_IO_REQUEST ioRequest;
    ioRequest.dwProtocol = actProtocol;
    ioRequest.cbPciLength = sizeof(SCARD_IO_REQUEST);

    BYTE recvBuff[262];
    unsigned long recvLen = 262;
    const auto retCode = SCardTransmit(hCard, &ioRequest, (LPCBYTE)_command.data(), length(), nullptr, recvBuff, &recvLen);
    if(retCode != SCARD_S_SUCCESS)
    {
        throw SmartCardError(retCode, "Функция SCardTransmit возвратила ошибку");
    }

    return processResponse(recvBuff, recvLen);
}
