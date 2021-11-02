//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "smartcardinfo.h"

///
/// \brief SmId::SmId
///
SmId::SmId()
    :QByteArray()
{
}

///
/// \brief SmId::SmId
/// \param bytes
///
SmId::SmId(const QByteArray& bytes)
    :QByteArray(bytes)
{
}

///
/// \brief SmId::operator QString
///
SmId::operator QString() const
{
    return toString();
}

///
/// \brief SmId::toString
/// \return
///
QString SmId::toString() const
{
    QStringList id;
    for(auto&& b : *this)
    {
        id.append(QString::number((unsigned char)b, 16));
    }

    return id.join(" ");
}

///
/// \brief SmFirmware::SmFirmware
///
SmFirmware::SmFirmware()
    :QByteArray()
{
}

///
/// \brief SmFirmware::SmFirmware
/// \param bytes
///
SmFirmware::SmFirmware(const QByteArray& bytes)
    :QByteArray(bytes)
{
}

///
/// \brief SmFirmware::operator QString
///
SmFirmware::operator QString() const
{
    return toString();
}

///
/// \brief SmFirmware::toString
/// \return
///
QString SmFirmware::toString() const
{
    return QString::fromLatin1(*this);
}

///
/// \brief SmartCardInfo::SmartCardInfo
///
SmartCardInfo::SmartCardInfo()
{
    static const int reg = qRegisterMetaType<SmartCardInfo>();
    Q_UNUSED(reg)
}

///
/// \brief SmartCardInfo::firmware
/// \return
///
SmFirmware SmartCardInfo::firmware() const
{
    return _firmware;
}

///
/// \brief SmartCardInfo::setFirmware
/// \param bytes
///
void SmartCardInfo::setFirmware(const SmFirmware& bytes)
{
    _firmware = bytes;
}

///
/// \brief SmartCardInfo::id
/// \return
///
SmId SmartCardInfo::id() const
{
    return _id;
}

///
/// \brief SmartCardInfo::setId
/// \param bytes
///
void SmartCardInfo::setId(const SmId& bytes)
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

///
/// \brief operator ==
/// \param smi1
/// \param smi2
/// \return
///
bool operator==(const SmartCardInfo& smi1, const SmartCardInfo& smi2)
{
    return smi1.id() == smi2.id();
}

///
/// \brief operator !=
/// \param smi1
/// \param smi2
/// \return
///
bool operator!=(const SmartCardInfo& smi1, const SmartCardInfo& smi2)
{
    return !operator==(smi1, smi2);
}
