//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef APPLOGGER_H
#define APPLOGGER_H

#include <mutex>
#include <QFile>
#include <QtDebug>
#include <QCoreApplication>

class AppLogger
{
public:
    static AppLogger& getInstance()
    {
        static AppLogger instance;
        return instance;
    }

    ///
    /// \brief setup
    /// \param app
    /// \param filename
    ///
    void setup(const QCoreApplication& app, const QString _filename = "");

    ///
    /// \brief hello
    /// \param app
    ///
    void hello(const QCoreApplication& app) const;

private:
    AppLogger();
    ~AppLogger();

    AppLogger(AppLogger const&) = delete;
    void operator=(AppLogger const&) = delete;    

private:
    QFile _logFile;
    QString _filename;

    static std::mutex mutex;
    static void defaultHandler(QtMsgType type, const QMessageLogContext &, const QString & msg);
    static void fileHandler(QtMsgType type, const QMessageLogContext &, const QString & msg);
};

#endif // APPLOGGER_H
