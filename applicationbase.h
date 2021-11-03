//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef APPLICATIONBASE_H
#define APPLICATIONBASE_H

#include <QtCore>

class ApplicationBase
{
public:
    ApplicationBase(int& argc, char **argv);

    static QString applicationBuildNumber();
    static void setApplicationBuildNumber(const QString& build);

    static QString revisionNumber();
    static void setRevisionNumber(const QString& revision);

    static QString applicationBranch();
    static void setApplicationBranch(const QString& branch);

    static QString appVersion(bool includeQtVersion = false, bool includeCpuArch = false);

    static QString applicationCopyright();
    static void setApplicationCopyright(const QString& copyright);

    static QString applicationDescription();
    void setApplicarionDescription(const QString& desc);

    void setupTranslator(const QString& file);

private:
    static QString appDescription;
    static QString appBuildNumber;
    static QString appRevision;
    static QString appBranch;
    static QString appCopyright;
    static QTranslator appTranslator;
};

#endif // APPLICATIONBASE_H
