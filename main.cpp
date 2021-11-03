//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include "mainwindow.h"

#include <QtCore>
#include "application.h"

int main(int argc, char *argv[])
{
    // отключаем логирование qt.modbus
    QLoggingCategory::setFilterRules("qt.modbus* = false");

    Application::setOrganizationName(TARGET_COMPANY);
    Application::setOrganizationDomain(TARGET_DOMAIN);
    Application::setApplicationVersion(APP_VERSION);
    Application::setApplicationBuildNumber(APP_BUILD);
    Application::setRevisionNumber(APP_REVISION);
    Application::setApplicationBranch(APP_BRANCH);
    Application::setApplicationCopyright(APP_COPYRIGHT);

    Application a(argc, argv);
    a.setupTranslator(":/res/translations/qtbase_ru.qm");

    MainWindow w;
    w.show();

    return a.exec();
}
