#-------------------------------------------------
#
# Modbus RTU сервер чения смарт карт
#
# Copyright (C) 2021 АО "Нефтеавтоматика"
#
#-------------------------------------------------


QT       += core gui widgets serialbus serialport

TARGET = SmartServer
TEMPLATE = app

VERSION_NUMBER = 0.0.1
GIT_REVISION = $$system(git --git-dir $$PWD/.git log --format="%h" -n 1)
GIT_BRANCH = $$system(git --git-dir $$PWD/.git rev-parse --abbrev-ref HEAD)
BUILD_NUMBER = $$system(git --git-dir $$PWD/.git rev-list main --count)
QMAKE_TARGET_COMPANY = "JSC Nefteavtomatika"
QMAKE_TARGET_PRODUCT = SmartServer
QMAKE_TARGET_DESCRIPTION = "Modbus RTU server fro smart cards"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2021 JSC Nefteavtomatika"
QMAKE_TARGET_DOMAIN = "nefteavtomatika.ru"

VERSION = $${VERSION_NUMBER}.$${BUILD_NUMBER}

win32
{
    RC_ICONS += res/windows/app.ico
    RC_CODEPAGE = 1200
    RC_LANG = 0x0419

    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='asInvoker'"

    DEFINES += _WIN32_WINNT=0x601
}

CONFIG += c++11
CONFIG += debug_and_release
CONFIG += debug_and_release_target

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Номер билда
DEFINES += APP_BUILD=\"\\\"$${BUILD_NUMBER}\\\"\"

# Ревизия
DEFINES += APP_REVISION=\"\\\"$${GIT_REVISION}\\\"\"

# Ветка
DEFINES += APP_BRANCH=\"\\\"$${GIT_BRANCH}\\\"\"

# Компания разработчик
DEFINES += TARGET_COMPANY=\"\\\"$${QMAKE_TARGET_COMPANY}\\\"\"

# Домен
DEFINES += TARGET_DOMAIN=\"\\\"$${QMAKE_TARGET_DOMAIN}\\\"\"

# Версия приложения
DEFINES += APP_VERSION=\\\"$${VERSION_NUMBER}\\\"

# Описание приложения
DEFINES += APP_DESCRIPTION=\"\\\"$${QMAKE_TARGET_DESCRIPTION}\\\"\"

# Копирайт
DEFINES += APP_COPYRIGHT=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"

SOURCES += \
    applogger.cpp \
    main.cpp \
    mainwindow.cpp \
    rtumodbusdatabuffer.cpp \
    rtumodbusserver.cpp \
    scc_getfirmware.cpp \
    scc_getid.cpp \
    smartcardcommand.cpp \
    smartcarddevice.cpp \
    smartcarderror.cpp \
    smartcardinfo.cpp

HEADERS += \
    applogger.h \
    mainwindow.h \
    qwaitcursor.h \
    rtumodbusdatabuffer.h \
    rtumodbusserver.h \
    scc_getfirmware.h \
    scc_getid.h \
    smartcardcommand.h \
    smartcarddevice.h \
    smartcarderror.h \
    smartcardinfo.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

LIBS += -lWinscard
