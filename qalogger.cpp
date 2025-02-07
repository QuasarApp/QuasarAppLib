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

static QFile* _logFile;
static bool _toFile = false;
static VerboseLvl _verboseLevel = Debug;


#define MESSAGE_PATTERN                                                                                           \
"[%{time MM-dd h:mm:ss.zzz} %{threadid} "                                                                          \
    "%{if-debug}Debug%{endif}%{if-info}Info%{endif}%{if-warning}Warning%{endif}%{if-critical}Error%{endif}%{if-fatal}Fatal%{endif}] "       \
    "%{message}"


QALogger::QALogger() {
    _logFile  = new QFile();
}

QALogger::~QALogger() {
    _logFile->close();
    delete _logFile;
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
        if (_toFile && _logFile) {
            QTextStream stream(_logFile);
            stream << qFormatLogMessage(type, context, msg) << Qt::endl;
            _logFile->flush();
        }

        switch (type) {
        case QtMsgType::QtFatalMsg: {
            Q_ASSERT_X(false, __FUNCTION__ , qFormatLogMessage(type, context, msg).toLatin1().data());
            break;
        }

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
        QString filePath = path + "/" + QCoreApplication::applicationName() + ".log";
        auto file =  Params::getArg("fileLog");
        if (file.size()) {
            filePath = file;
        }

        QDir().mkpath(path);

        _logFile->setFileName(filePath);

        if (!_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qFatal() << "Can't open log file";
        }

    }

}

}
