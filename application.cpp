//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//


#include <QtCore>
#include <QFileInfo>
#include "application.h"
#include "appsettings.h"
#include "applogger.h"

///
/// \brief Application::Application
/// \param argc
/// \param argv
///
Application::Application(int &argc, char **argv)
    :QApplication(argc, argv)
    ,ApplicationBase(argc, argv)

{   
    setAttribute(Qt::AA_EnableHighDpiScaling);
    setAttribute(Qt::AA_UseHighDpiPixmaps);   
}

///
/// \brief Application::notify
/// \param receiver
/// \param e
/// \return
///
bool Application::notify(QObject *receiver, QEvent *e)
{
    try
    {
        return QApplication::notify(receiver, e);
    }
    catch (std::exception& ex)
    {
        qCritical() << "Исключение из QApplication::notify:" << QString(ex.what()).simplified();
    }

    return false;
}

///
/// \brief Application::setUserStyle
/// \param file
///
void Application::setUserStyle(const QString& file)
{
    QFile f(file);
    if(f.open(QFile::ReadOnly | QFile::Text))
    {
        setStyleSheet(f.readAll());
    }   
}
