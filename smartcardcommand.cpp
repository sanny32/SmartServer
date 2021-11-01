//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QByteArray>
#include "smartcardcommand.h"

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
unsigned long SmartCardCommand::length() const
{
    return _command.length();
}
