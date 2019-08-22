#
# Copyright (C) 2018-2019 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

QT_DIR = $$dirname(QMAKE_QMAKE)
LUPDATE = $$QT_DIR/lupdate
LRELEASE = $$QT_DIR/lrelease

win32 {
    LUPDATE = $${LUPDATE}.exe
    LRELEASE = $${LRELEASE}.exe
}

PRO_FILES = $$files(*.pro, true)

for(PRO, PRO_FILES) {
    commands += "$$LUPDATE $$PRO"
}


TS_FILES = $$files(*.ts, true)

for(TS, TS_FILES) {
    commands += "$$LRELEASE $$TS"
}

for(command, commands) {
    system($$command)|error("Failed to run: $$command")
}

