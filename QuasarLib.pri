#
# Copyright (C) 2018 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(QUASARAPP_LIB):error("QuasarLib.pri already included")
QUASARAPP_LIB = 1

TMP_DESTDIR=$$DESTDIR

include(deploy.pri)
#DEPENDS
LIBS += -L"$$DESTDIR" -lQuasarApp

copydata.commands += $(COPY_DIR) $$DESTDIR/* $$TMP_DESTDIR
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


DESTDIR=$$TMP_DESTDIR

INCLUDEPATH += "$$PWD/"



