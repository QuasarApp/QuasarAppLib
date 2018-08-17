#
# Copyright (C) 2018 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(QUASARAPP_LIB):error("QuasarLib.pri already included")
QUASARAPP_LIB = 1

#DEPENDS
CONFIG(debug, debug|release): {
    OUTPUT_DIR_QUASARAPP = "$$PWD/build/debug/bin"
} else: {
    OUTPUT_DIR_QUASARAPP = "$$PWD/build/release/bin"
}

LIBS += -L"$$OUTPUT_DIR_QUASARAPP/" -lQuasarApp

INCLUDEPATH += "$$PWD/"
