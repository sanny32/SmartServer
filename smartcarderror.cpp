//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "smartcarderror.h"

///
/// \brief SmartCardError::SmartCardError
/// \param code
///
SmartCardError::SmartCardError(long code, const QString& message)
    :_code(code)
{        
    _what = QString("%1. Код ошибки: %2").arg(message, QString::number(code, 10)).toStdString();
}

///
/// \brief SmartCardError::what
/// \return
///
char const* SmartCardError::what() const
{
    return _what.c_str();
}
