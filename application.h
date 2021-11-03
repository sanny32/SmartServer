//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QFile>
#include <QProxyStyle>
#include <QTranslator>
#include <QApplication>
#include "applicationbase.h"

class Application : public QApplication, public ApplicationBase
{
public:
    Application(int &argc, char **argv);
    void setUserStyle(const QString& file);

    bool notify(QObject *receiver, QEvent *e) override;          
};

#endif // APPLICATION_H
