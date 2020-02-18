/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "params.h"
#include <QVariantMap>
#include <QDebug>
#include <QFileInfo>
#include <iostream>
#include <QDateTime>

#ifdef Q_OS_WIN
#include "windows.h"
#else
#include <unistd.h>
#include <limits.h>
#endif

using namespace QuasarAppUtils;

static QVariantMap params = QVariantMap();

bool Params::isEndable(const QString& key) {
    return params.contains(key);
}

void Params::verboseLog(const QString &log, VerboseLvl vLvl) {

    writeLoginFile(log, vLvl);

    if (isEndable("verbose")) {

        auto lvl = static_cast<VerboseLvl>(getArg("verbose").toInt());

        if (vLvl <= lvl) {

            switch (vLvl) {

            case VerboseLvl::Error: {
                qCritical() << lvlToString(vLvl) + ": " + log;
                break;
            }

            case VerboseLvl::Warning: {
                qWarning() << lvlToString(vLvl) + ": " + log;
                break;
            }

            case VerboseLvl::Info: {
                qInfo() << lvlToString(vLvl) + ": " + log;
                break;
            }

            default: {
                qDebug() << lvlToString(vLvl) + ": " + log;
                break;
            }
            }
        }
    }
}

Help::Charters Params::getparamsHelp() {
    return {
        {
            "Base Options", {
                {"-verbose (level 1 - 3)",  "Shows debug log"},
                {"-fileLog (path to file)", "Sets path of log file. Default it is path to executable file with suffix '.log'"},
                {"noWriteInFileLog",        "Disables loging into file"}
            }
        }
    };
}

void Params::showHelp(const QStringList &help) {
    for (const QString& line : help) {
        std::cout << line.toStdString() << std::endl;
    }
}

void Params::showHelp(const Help::Charters &help) {
    Help::print(help);
}

void Params::showHelp() {
    Help::print(getparamsHelp());
}

int Params::size() {
    return params.size();
}

int Params::customParamasSize() {
    return size() - 2;
}

QString Params::timeString() {
    return QDateTime::currentDateTime().toString();
}

QString Params::lvlToString(VerboseLvl vLvl) {
    switch (vLvl) {

    case VerboseLvl::Error: {
        return "Error";
    }

    case VerboseLvl::Warning: {
        return "Warning";
    }

    case VerboseLvl::Info: {
        return "Info";
    }

    case VerboseLvl::Debug: {
        return "Verbose log";
    }
    }

    return "";
}

bool Params::writeLoginFile(const QString &log, VerboseLvl vLvl) {
    if (!isEndable("noWriteInFileLog")) {

        QString path = getStrArg("appPath") + "/" + getStrArg("appName") + ".log";

        if (isEndable("fileLog")) {
            QString path = getStrArg("fileLog");
        }

        QFile logFile(path);

        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {

            QTextStream stream(&logFile);
            stream << timeString() <<"| " << lvlToString(vLvl) + ": " + log << endl;
            logFile.close();
        } else {
            return false;
        }

    }

    return true;
}

bool Params::parseParams(int argc, const char *argv[]) {

    QStringList params;
    for (int i = 1; i < argc; i++) {
        params.push_back(argv[i]);
    }

    return parseParams(params);
}

bool Params::parseParams(int argc, char *argv[]) {
    return parseParams(argc, const_cast<const char**>(argv));
}

bool Params::parseParams(const QStringList &paramsArray) {
    params.clear();

#ifdef Q_OS_WIN
    char buffer[MAX_PATH];
    memset(buffer, 0, sizeof buffer);

    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    params ["appPath"] = QFileInfo(buffer).absolutePath();
    params ["appName"] = QFileInfo(buffer).fileName();

#else
    char path[2048];
    memset(path, 0, sizeof path);

    if (readlink("/proc/self/exe", path, 2048) < 0) {
        qWarning() << "parseParams can't get self path!" ;
        return false;
    }
    params ["appPath"] =  QFileInfo(path).absolutePath();
    params ["appName"] =  QFileInfo(path).fileName();

#endif

    if (!getStrArg("appPath").size()) {
        return false;
    }

    for (int i = 0 ; i < paramsArray.size(); ++i) {
        if (paramsArray[i][0] == '-') {
            if (i < (paramsArray.size() - 1) && paramsArray[i + 1][0] != '-') {
                params[paramsArray[i].mid(1)] = paramsArray[i + 1];
                i++;
            } else {
                qWarning() << "Missing argument for " + paramsArray[i] ;
            }
        } else {
            params[paramsArray[i]] = "";
        }
    }

    return true;
}

QString Params::getStrArg(const QString& key, const QString &def) {
    return params.value(key, def).toString();
}

QVariant Params::getArg(const QString& key,const QVariant& def) {
    return params.value(key, def);
}

void Params::setArg(const QString &key, const QVariant &val) {
    params.insert(key, val);
}

void Params::setEnable(const QString &key, bool enable) {
    if (enable) {
        params.insert(key, "");
    } else {
        params.remove(key);
    }
}
