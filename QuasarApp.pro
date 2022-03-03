
#
# Copyright (C) 2018-2022 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

QT       -= gui
CONFIG += c++17

TARGET = QuasarApp
TEMPLATE = lib

DEFINES += QUASARAPP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(release, debug|release): {
    DESTDIR = $$PWD/build/release
    DEFINES += RELEASE_BUILD

} else {
    DESTDIR = $$PWD/build/debug
}

SOURCES += \
        helpdata.cpp \
        optiondata.cpp \
        quasarapp.cpp \
        params.cpp \
        locales.cpp \
        settings.cpp \
        isettings.cpp

HEADERS += \
        helpdata.h \
        optiondata.h \
        quasarapp.h \
        quasarapp_global.h \ 
        params.h \
        locales.h \
        settings.h \
        isettings.h \
        qaglobalutils.h

DISTFILES += \
    QuasarLib.pri \
    qmakeEtalons/locales.pri

RESOURCES += \
    res.qrc


include(Etalons/qmake/ccache.pri)

VERSION = 1.5.1

# Add empty target for CI system
QMAKE_EXTRA_TARGETS += \
    test \
    deploy
