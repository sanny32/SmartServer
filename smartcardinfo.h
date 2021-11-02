//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef SMARTCARDINFO_H
#define SMARTCARDINFO_H

#include <QString>
#include <QMetaClassInfo>

///
/// \brief Идентификатор смарт-карты
///
class SmId: public QByteArray
{
public:
    SmId();
    SmId(const QByteArray& bytes);

    operator QString() const;
    QString toString() const;
};

///
/// \brief Версия firmware
///
class SmFirmware: public QByteArray
{
public:
    SmFirmware();
    SmFirmware(const QByteArray& bytes);

    operator QString() const;
    QString toString() const;
};

///
/// \brief Информация о смарт-карте
///
class SmartCardInfo
{
public:
    SmartCardInfo();

    SmFirmware firmware() const;
    void setFirmware(const SmFirmware& bytes);

    SmId id() const;
    void setId(const SmId& bytes);

    bool isValid() const;

    friend bool operator==(const SmartCardInfo& smi1, const SmartCardInfo& smi2);
    friend bool operator!=(const SmartCardInfo& smi1, const SmartCardInfo& smi2);

private:
    SmId _id;
    SmFirmware _firmware;
};
Q_DECLARE_METATYPE(SmartCardInfo);


#endif // SMARTCARDINFO_H
