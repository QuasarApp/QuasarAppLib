/*
 * Copyright (C) 2018-2021 QuasarApp.
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
#include <QCoreApplication>

#ifdef Q_OS_WIN
#include "windows.h"
#else
#include <unistd.h>
#include <limits.h>
#endif

#define APP_PATH "appPath"
#define APP_NAME "appName"

using namespace QuasarAppUtils;

static QMap<QString, QString> params = QMap<QString, QString>();
static int _argc = 0;


bool Params::isEndable(const QString& key) {
    return params.contains(key);
}

void Params::log(const QString &log, VerboseLvl vLvl) {

    writeLoginFile(log, vLvl);

    auto lvl = getVerboseLvl();

    if (vLvl <= lvl) {

        switch (vLvl) {

        case VerboseLvl::Error:
        case VerboseLvl::Warning: {
            std::cerr << lvlToString(vLvl) + ": " + log.toStdString() << std::endl;
            break;
        }

        default: {
            std::cout << lvlToString(vLvl) + ": " + log.toStdString() << std::endl;
            break;
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

VerboseLvl Params::getVerboseLvl() {
    return static_cast<VerboseLvl>(getArg("verbose", DEFAULT_VERBOSE_LVL).toInt());
}

bool Params::isDebug() {
    return getVerboseLvl() >= VerboseLvl::Debug;
}

bool Params::isDebugBuild() {
#ifdef QT_DEBUG
    return true;
#else
    return false;
#endif
}

void Params::showHelp() {
    Help::print(getparamsHelp());
}

QMap<QString, QString> Params::getUserParamsMap() {
    auto result = params;
    result.remove(APP_PATH);
    result.remove(APP_NAME);

    return result;
}

void Params::clearParsedData() {
    params.clear();
    _argc = 0;
}

QString Params::getCurrentExecutable() {
    return getCurrentExecutableDir() + "/" + getArg(APP_NAME);
}

QString Params::getCurrentExecutableDir() {
    return getArg(APP_PATH);
}

int Params::size() {
    return params.size();
}

int Params::customParamasSize() {
    if (_argc)
        return _argc - 1;

    return size() - 2;
}

QString Params::timeString() {
    return QDateTime::currentDateTime().toString();
}

std::string Params::lvlToString(VerboseLvl vLvl) {
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
    default: return "";
    }

    return "";
}

bool Params::writeLoginFile(const QString &log, VerboseLvl vLvl) {
    if (isEndable("fileLog")) {

        QString path = getCurrentExecutable() + ".log";
        auto file =  getArg("fileLog");
        if (file.size()) {
            path = file;
        }

        QFile logFile(path);

        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {

            QTextStream stream(&logFile);
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
            stream << timeString() <<"| " << QString::fromStdString(lvlToString(vLvl)) + ": " + log << Qt::endl;
#else
            stream << timeString() <<"| " << QString::fromStdString(lvlToString(vLvl)) + ": " + log << endl;
#endif
            logFile.close();
        } else {
            return false;
        }

    }

    return true;
}

bool Params::parseParams(const int argc, const char *argv[]) {

    QStringList params;
    for (int i = 1; i < argc; i++) {
        params.push_back(argv[i]);
    }

    _argc = argc;
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
    params [APP_PATH] = QFileInfo(buffer).absolutePath();
    params [APP_NAME] = QFileInfo(buffer).fileName();

#else
    char path[2048];
    memset(path, 0, sizeof path);

    if (readlink("/proc/self/exe", path, 2048) < 0) {
        QuasarAppUtils::Params::log("parseParams can't get self path!",
                                           QuasarAppUtils::Error);
        return false;
    }
    params [APP_PATH] =  QFileInfo(path).absolutePath();
    params [APP_NAME] =  QFileInfo(path).fileName();

#endif

    if (!getArg(APP_PATH).size()) {
        return false;
    }

    for (int i = 0 ; i < paramsArray.size(); ++i) {
        if (paramsArray[i][0] == '-') {
            if (i < (paramsArray.size() - 1) && paramsArray[i + 1][0] != '-') {
                params[paramsArray[i].mid(1)] = paramsArray[i + 1];
                i++;
            } else {
                QuasarAppUtils::Params::log("Missing argument for " + paramsArray[i],
                                                   QuasarAppUtils::Error);
                return false;
            }
        } else {
            params[paramsArray[i]] = "";
        }
    }

    printWorkingOptions();

    return true;
}

void Params::printWorkingOptions() {
    QuasarAppUtils::Params::log("--- Working options table start ---",
                                QuasarAppUtils::Debug);

    QMap<QString, QString>::const_iterator iter = params.constBegin();
    while (iter != params.constEnd()) {

        QString row = QString{"Option[%0]"}.arg(iter.key());

        QString value = iter.value();
        if (!value.isEmpty()) {
            row += QString{": %1"}.arg(value);
        }

        QuasarAppUtils::Params::log(row, QuasarAppUtils::Debug);

        ++iter;
    }

    QuasarAppUtils::Params::log("--- Working options table end ---",
                                QuasarAppUtils::Debug);
}

QString Params::getArg(const QString& key,const QString& def) {
    return params.value(key, def);
}

void Params::setArg(const QString &key, const QString &val) {
    params.insert(key, val);
}

void Params::setEnable(const QString &key, bool enable) {
    if (enable) {
        params.insert(key, "");
    } else {
        params.remove(key);
    }
}
