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

class SmartCardInfo
{
public:
    SmartCardInfo();

    QString firmware() const;
    void setFirmware(const QByteArray& bytes);

    QByteArray id() const;
    void setId(const QByteArray& bytes);

    bool isValid() const;

private:
    QByteArray _id;
    QByteArray _firmware;
};
Q_DECLARE_METATYPE(SmartCardInfo);


#endif // SMARTCARDINFO_H
