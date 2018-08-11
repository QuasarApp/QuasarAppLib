

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -lQuasaraApp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -lQuasaraApp
else:unix: LIBS += -L$$OUT_PWD/./ -lQuasaraApp

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.


!isEmpty(QUASARAPP_LIB):error("QuasarLib.pri already included")
QUASARAPP_LIB = 1

#DEPENDS
include($$PWD/NodusDB_depends.pri)

INCLUDEPATH += "$$PWD/"
