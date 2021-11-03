//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "applicationbase.h"
#include "appsettings.h"
#include "applogger.h"

QString ApplicationBase::appDescription;
QString ApplicationBase::appCopyright;
QString ApplicationBase::appBuildNumber;
QString ApplicationBase::appRevision;
QString ApplicationBase::appBranch;
QTranslator ApplicationBase::appTranslator;

///
/// \brief ApplicationBase::ApplicationBase
/// \param argv
///
ApplicationBase::ApplicationBase(int&, char **argv)
{
    QCoreApplication::setApplicationName(QFileInfo(argv[0]).completeBaseName());

    qInfo().noquote() << QCoreApplication::applicationName() << appVersion(false, true);
    qInfo().noquote() << applicationCopyright() << "\n";

    // логирование
    AppLogger::getInstance();   
}

///
/// \brief ApplicationBase::setupTranslator
/// \param file
///
void ApplicationBase::setupTranslator(const QString& file)
{
    if(appTranslator.load(file))
    {
        QCoreApplication::installTranslator(&appTranslator);
    }
    else
    {
        qWarning() << "Не удалось установить переводчик из" << file;
    }
}

///
/// \brief ApplicationBase::applicationBuildNumber
/// \return
///
QString ApplicationBase::applicationBuildNumber()
{
    return appBuildNumber;
}

///
/// \brief ApplicationBase::setApplicationBuildNumber
/// \param build
///
void ApplicationBase::setApplicationBuildNumber(const QString& build)
{
    appBuildNumber = build;
}

///
/// \brief ApplicationBase::revisionNumber
/// \return
///
QString ApplicationBase::revisionNumber()
{
    return appRevision;
}

///
/// \brief ApplicationBase::setRevisionNumber
/// \param revision
///
void ApplicationBase::setRevisionNumber(const QString& revision)
{
    appRevision = revision;
}

///
/// \brief ApplicationBase::applicationBranch
/// \return
///
QString ApplicationBase::applicationBranch()
{
    return appBranch;
}

///
/// \brief ApplicationBase::setApplicationBranch
/// \param branch
///
void ApplicationBase::setApplicationBranch(const QString& branch)
{
    appBranch = branch;
}

///
/// \brief ApplicationBase::appVersion
/// \param includeQtVersion
/// \param includeCpuArch
/// \return
///
QString ApplicationBase::appVersion(bool includeQtVersion, bool includeCpuArch)
{
    QString version;
    if(applicationBranch() != "main")
    {
        version = QString("%1-%2.%3").arg(qApp->applicationVersion(), applicationBranch(), revisionNumber());
    }
    else
    {
        version = QString("%1.%2").arg(qApp->applicationVersion(), applicationBuildNumber());
    }

    if(includeQtVersion)
    {
        version.append(QString(" (Qt %1)").arg(qVersion()));
    }

    if(includeCpuArch)
    {
        version.append(QString(" %1").arg(QSysInfo::currentCpuArchitecture()));
    }

    return version;
}

///
/// \brief ApplicationBase::applicationCopyright
/// \return
///
QString ApplicationBase::applicationCopyright()
{
    return appCopyright;
}

///
/// \brief ApplicationBase::setApplicationCopyright
/// \param copyright
///
void ApplicationBase::setApplicationCopyright(const QString& copyright)
{
    appCopyright = copyright;
}

///
/// \brief ApplicationBase::applicationDescription
/// \return
///
QString ApplicationBase::applicationDescription()
{
    return appDescription;
}

///
/// \brief ApplicationBase::setApplicarionDescription
/// \param des
///
void ApplicationBase::setApplicarionDescription(const QString& desc)
{    
    appDescription = desc;
}
