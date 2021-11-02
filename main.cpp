//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "mainwindow.h"

#include <QtCore>
#include <QApplication>
#include "applogger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

    MainWindow w;
    w.show();

    return a.exec();
}
