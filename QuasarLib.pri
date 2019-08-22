#
# Copyright (C) 2018-2019 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(QUASARAPP_LIB):error("QuasarLib.pri already included")
QUASARAPP_LIB = 1

#DEPENDS
CONFIG(release, debug|release): {
    QUASARAPP_LIB_OUTPUT_DIR="$$PWD/build/release"
} else {
    QUASARAPP_LIB_OUTPUT_DIR="$$PWD/build/debug"
}

unix:LIBS += -L$$QUASARAPP_LIB_OUTPUT_DIR -lQuasarApp

win32:LIBS += -L$$QUASARAPP_LIB_OUTPUT_DIR -lQuasarApp1


INCLUDEPATH += "$$PWD/"

include(Etalons/qmake/ccache.pri);


