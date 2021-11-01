//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "smartcardinfo.h"

SmartCardInfo::SmartCardInfo()
{
    static const int reg = qRegisterMetaType<SmartCardInfo>();
    Q_UNUSED(reg)
}

///
/// \brief SmartCardInfo::firmware
/// \return
///
QString SmartCardInfo::firmware() const
{
    return QString::fromLatin1(_firmware);
}

///
/// \brief SmartCardInfo::setFirmware
/// \param bytes
///
void SmartCardInfo::setFirmware(const QByteArray& bytes)
{
    _firmware = bytes;
}

///
/// \brief SmartCardInfo::id
/// \return
///
QByteArray SmartCardInfo::id() const
{
    return _id;
}

///
/// \brief SmartCardInfo::setId
/// \param bytes
///
void SmartCardInfo::setId(const QByteArray& bytes)
{
    _id = bytes;
}

///
/// \brief SmartCardInfo::isValid
/// \return
///
bool SmartCardInfo::isValid() const
{
    return !_id.isEmpty();
}
