/*
 * Copyright (C) 2024-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "qalogger.h"
#include "params.h"

#include <QFile>

namespace QuasarAppUtils {


#define MESSAGE_PATTERN                                                                                           \
    "[%{time MM-dd h:mm:ss.zzz} %{threadid} "                                                                          \
    "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] "       \
    "%{message}"


bool checkLogType(QtMsgType type, VerboseLvl lvl) {
    switch (type) {
    case QtDebugMsg: return lvl >= Debug;
    case QtInfoMsg: return lvl >= Info;
    case QtWarningMsg: return lvl >= Warning;

    case QtCriticalMsg:
    case QtFatalMsg:
        return true;
    }

    return true;
}
void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg) {

    if (Params::isEndable("fileLog")) {
        auto verboselvl = Params::getVerboseLvl();
        if (checkLogType(type, verboselvl)) {
            QString path = Params::getCurrentExecutable() + ".log";
            auto file =  Params::getArg("fileLog");
            if (file.size()) {
                path = file;
            }

            QFile logFile(path);
            if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {

                QTextStream stream(&logFile);
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
                stream << msg << Qt::endl;
#else
                stream << msg << endl;
#endif
                logFile.close();
            }
        }
    }
}

void QALogger::init() {
    qSetMessagePattern(MESSAGE_PATTERN);
    qInstallMessageHandler(messageHandler);

}

}
