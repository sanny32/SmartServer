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
#include <QListWidget>
#include <QApplication>

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
    void setup(const QApplication& app, const QString _filename = "");

    ///
    /// \brief setup
    /// \param app
    /// \param listWidget
    ///
    void setup(const QApplication& app, QListWidget* listWidget);

    ///
    /// \brief hello
    /// \param app
    ///
    void hello(const QApplication& app) const;

private:
    AppLogger();
    ~AppLogger();

    AppLogger(AppLogger const&) = delete;
    void operator=(AppLogger const&) = delete;    

private:
    QFile _logFile;
    QString _filename;
    QListWidget* _listWidget;

    static std::mutex mutex;
    static void defaultHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    static void fileHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
    static void listWidgetHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
};

#endif // APPLOGGER_H
