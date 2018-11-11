#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T20:32:41
#
#-------------------------------------------------

QT       -= gui

TARGET = QuasarApp
TEMPLATE = lib

DEFINES += QUASARAAPP_LIBRARY

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

} else {
    DESTDIR = $$PWD/build/debug
}

SOURCES += \
        quasarapp.cpp \
    params.cpp \
    locales.cpp \
    settings.cpp

HEADERS += \
        quasarapp.h \
        quasarapp_global.h \ 
    params.h \
    locales.h \
    settings.h

DISTFILES += \
    QuasarLib.pri

RESOURCES += \
    res.qrc


