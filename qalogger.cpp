/*
 * Copyright (C) 2024-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "qalogger.h"
#include "params.h"
#include <iostream>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace QuasarAppUtils {

Q_GLOBAL_STATIC(QString, _logFile)

static bool _toFile = false;
static VerboseLvl _verboseLevel = Debug;


#define MESSAGE_PATTERN                                                                                           \
"[%{time MM-dd h:mm:ss.zzz} %{threadid} "                                                                          \
    "%{if-debug}Debug%{endif}%{if-info}Info%{endif}%{if-warning}Warning%{endif}%{if-critical}Error%{endif}%{if-fatal}Fatal%{endif}] "       \
    "%{message}"


QALogger::QALogger() {
}

QALogger::~QALogger() {
}


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
void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString &msg) {



    if (checkLogType(type, _verboseLevel)) {
        if (_toFile && _logFile->size()) {
            QFile logFile(*_logFile);
            if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                QTextStream stream(&logFile);
                stream << qFormatLogMessage(type, context, msg) << Qt::endl;
            }

        }

        switch (type) {
        case QtMsgType::QtFatalMsg:
        case QtMsgType::QtCriticalMsg:
        case QtMsgType::QtWarningMsg: {
            std::cerr << qFormatLogMessage(type, context, msg).toStdString() << std::endl;
            break;
        }
        case QtMsgType::QtDebugMsg:
        case QtMsgType::QtInfoMsg:
        default: {
            std::cout << qFormatLogMessage(type, context, msg).toStdString() << std::endl;

            break;
        }

        }
    }
}


void QALogger::init() {
    qSetMessagePattern(MESSAGE_PATTERN);
    qInstallMessageHandler(messageHandler);


    _verboseLevel = Params::getVerboseLvl();

    if (Params::isEndable("fileLog")) {
        _toFile = true;
        QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QString filePath = path + "/" + QCoreApplication::applicationName() +
                           " " + QDate::currentDate().toString(Qt::DateFormat::ISODate) + ".log";
        auto file =  Params::getArg("fileLog");
        if (file.size()) {
            filePath = file;
        }

        QDir().mkpath(path);

        *_logFile = filePath;

    }

}

}
