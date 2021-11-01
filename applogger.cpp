//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QDir>
#include <QSysInfo>
#include <QDateTime>
#include "applogger.h"

std::mutex AppLogger::mutex;

///
/// \brief AppLogger::AppLogger
///
AppLogger::AppLogger()
{
    qInstallMessageHandler(AppLogger::defaultHandler);
}

///
/// \brief AppLogger::~AppLogger
///
AppLogger::~AppLogger()
{
    if(_logFile.isOpen())
    {
        _logFile.close();
    }
}

///
/// \brief AppLogger::setup
/// \param app
/// \param filename
///
void AppLogger::setup(const QCoreApplication& app, const QString filename)
{
    QString appDir = app.applicationDirPath();
    if(filename.isEmpty())
    {
        _logFile.setFileName(appDir + QDir::separator() + app.applicationName() + ".log");
    }
    else
    {
        _logFile.setFileName(filename);
    }

    if(_logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        fputs(qPrintable(QString("Установлено логирование в файл %1").arg(_logFile.fileName())), stdout);
        qInstallMessageHandler(AppLogger::fileHandler);

        hello(app);
    }
    else
    {
        qCritical() << "Не удалось открыть (создать) файл лога" << _logFile.fileName();
    }
}

///
/// \brief AppLogger::hello
/// \param app
///
void AppLogger::hello(const QCoreApplication& app) const
{
    qInfo().noquote() << app.applicationName() << app.applicationVersion() << QSysInfo::currentCpuArchitecture();
}

///
/// \brief AppLogger::defaultHandler
/// \param type
/// \param msg
///
void AppLogger::defaultHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{       
    std::lock_guard<std::mutex> lock(AppLogger::mutex);
    {        
        auto now = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz");

        QString category = ctx.category;
        QString text = (category == "default") ? msg : QString("[%1] %2").arg(ctx.category, msg);

        switch (type)
        {
            case QtDebugMsg:
                qDebug().noquote() << QString("[%1] Debug: %2").arg(now, text);
            break;

            case QtInfoMsg:
                qInfo().noquote() << QString("[%1] Info: %2").arg(now, text);
            break;

            case QtWarningMsg:
                qWarning().noquote() << QString("[%1] Warning: %2").arg(now, text);
            break;

            case QtCriticalMsg:
                qCritical().noquote() << QString("[%1] Critical: %2").arg(now, text);
            break;

            case QtFatalMsg:
                qFatal("[%s] Fatal: %s", now.data(), text.data());
        }
    }
}

///
/// \brief AppLogger::fileHandler
/// \param type
/// \param msg
///
void AppLogger::fileHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{    
    auto now = QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz");

    QString category = ctx.category;
    QString text = (category == "default") ? msg : QString("[%1] %2").arg(ctx.category, msg);

    switch (type)
    {
        case QtDebugMsg:
            text = QString("[%1] Debug: %2").arg(now, text);
        break;

        case QtInfoMsg:
            text = QString("[%1] Info: %2").arg(now, text);
        break;

        case QtWarningMsg:
            text = QString("[%1] Warning: %2").arg(now, text);
        break;

        case QtCriticalMsg:
            text = QString("[%1] Critical: %2").arg(now, text);
        break;

        case QtFatalMsg:
            text = QString("[%1] Fatal: %2").arg(now, text);
        break;
    }

    std::lock_guard<std::mutex> lock(AppLogger::mutex);
    {
        auto&& logFile = AppLogger::getInstance()._logFile;
        if(logFile.isOpen())
        {
            QTextStream ts(&logFile);
            ts << text << endl;
        }
    }
}
