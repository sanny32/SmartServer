//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SMARTCARDERROR_H
#define SMARTCARDERROR_H

#include <stdexcept>
#include <QString>

class SmartCardError : public std::exception
{
public:
    SmartCardError(long code, const QString& message);

    char const* what() const override;

private:
    long _code;
    std::string _what;
};

#endif // SMARTCARDERROR_H
