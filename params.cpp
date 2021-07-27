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

using namespace QuasarAppUtils;
QMap<QString, QString> Params::params = QMap<QString, QString>();
QString Params::appPath = "";
QString Params::appName = "";
Help::Section Params::userHelp = {};


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
    Help::print(userHelp);
}

const QMap<QString, QString>& Params::getUserParamsMap() {
    return params;
}

void Params::clearParsedData() {
    params.clear();
    appPath = "";
    appName = "";
}

QString Params::getCurrentExecutable() {
    return getCurrentExecutableDir() + "/" + appName;
}

QString Params::getCurrentExecutableDir() {
    return appPath;
}

OptionsDataList Params::availableArguments() {
    return OptionsDataList{
        {
            "Base Options",
            OptionData{
                {"-verbose"}, "(level 1 - 3)", "Shows debug log"
            }
        },
        {
            "Base Options",
            OptionData{
                {"-fileLog"}, "(path to file)", "Sets path of log file. Default it is path to executable file with suffix '.log'"
            }
        }
    };
}

int Params::size() {
    return params.size();
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

bool Params::parseParams(const int argc, const char *argv[], const OptionsDataList& options) {

    QStringList params;
    for (int i = 1; i < argc; i++) {
        params.push_back(argv[i]);
    }

    return parseParams(params, options);
}

bool Params::parseParams(int argc, char *argv[], const OptionsDataList& options) {
    return parseParams(argc, const_cast<const char**>(argv), options);
}

bool Params::parseParams(const QStringList &paramsArray, const OptionsDataList &options) {
    params.clear();
    OptionsDataList availableOptions;

    parseAvailableOptions(OptionsDataList{}.unite(options).unite(availableArguments()),
                          &availableOptions,
                          &userHelp);

#ifdef Q_OS_WIN
    char buffer[MAX_PATH];
    memset(buffer, 0, sizeof buffer);

    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    appPath = QFileInfo(buffer).absolutePath();
    appName = QFileInfo(buffer).fileName();

#else
    char path[2048];
    memset(path, 0, sizeof path);

    if (readlink("/proc/self/exe", path, 2048) < 0) {
        QuasarAppUtils::Params::log("parseParams can't get self path!",
                                           QuasarAppUtils::Error);
        return false;
    }
    appPath =  QFileInfo(path).absolutePath();
    appName =  QFileInfo(path).fileName();

#endif

    if (!appPath.size()) {
        return false;
    }

    for (int i = 0 ; i < paramsArray.size(); ++i) {

        auto optionData = availableOptions.value(paramsArray[i], {{}});

        if (!optionData.isValid()) {
            QuasarAppUtils::Params::log("You use wrong option name, please check the help before run your commnad.",
                                        QuasarAppUtils::Error);
            return false;
        }

        if (optionData.isDepricated()) {



            if (optionData.isRemoved()) {
                QuasarAppUtils::Params::log(optionData.depricatedMsg(),
                                            QuasarAppUtils::Error);
                return false;
            }

            QuasarAppUtils::Params::log(optionData.depricatedMsg(),
                                        QuasarAppUtils::Warning);

        }

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

void Params::parseAvailableOptions(const OptionsDataList &availableOptionsListIn,
                                   OptionsDataList *availableOptionsListOut,
                                   Help::Section *helpOut) {

    if (!(availableOptionsListOut && helpOut))
        return;

    QHash<QString, Help::Options> options;
    for (auto it = availableOptionsListIn.begin(); it != availableOptionsListIn.end(); ++it) {

        if (availableOptionsListOut) {
            for (const auto &name : qAsConst(it.value().names())) {
                availableOptionsListOut->insert(name, it.value());
            }
        }

        if (helpOut) {
            options[it.key()].unite(it.value().toHelp());
        }
    }

    for (auto it = options.begin(); it != options.end(); ++it) {
        helpOut->insert(it.key(), it.value());
    }
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
