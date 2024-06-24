/*
 * Copyright (C) 2018-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "params.h"
#include <QVariantMap>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QCoreApplication>
#include <QtLogging>

#ifdef Q_OS_WIN
#include "windows.h"
#else
#include <unistd.h>
#endif

using namespace QuasarAppUtils;
QMap<QString, QString> Params::params = QMap<QString, QString>();
QString Params::appPath = "";
QString Params::appName = "";
Help::Section Params::userHelp = {};
OptionsDataList Params::inputOptions = {};


bool Params::isEndable(const QString& key) {
    return params.contains(key);
}

void Params::log(const QString &log, VerboseLvl vLvl) {

    auto lvl = getVerboseLvl();
    if (vLvl <= lvl) {

        switch (vLvl) {

        case VerboseLvl::Error:
            qCritical().noquote() << log;
            break;

        case VerboseLvl::Warning: {
            qWarning().noquote() << log;
            break;
        }
        case VerboseLvl::Debug: {
            qDebug().noquote() << log;
            break;
        }

        case VerboseLvl::Info:
        default: {
            qInfo().noquote() <<  log;
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

    if (inputOptions.size() > 1) {
        showHelpForInputOptions();
    } else {
        Help::print(userHelp);
    }
}

void Params::showHelpForInputOptions() {
    Help::print(getHelpOfInputOptions());
}

Help::Section Params::getHelpOfInputOptions() {

    if (inputOptions.size() <= 1 ) {
        return {};
    }

    Help::Options help;
    for (const auto &optionData: std::as_const(inputOptions) ) {
        help.unite(optionData.toHelp());
    }

    return Help::Section{{"Information about using options", help}};
}

const Help::Section &Params::getHelp() {
    return userHelp;
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

bool Params::optionsForEach(const QStringList &paramsArray,
                            const OptionsDataList& availableOptions) {

    for (int i = 0 ; i < paramsArray.size(); ++i) {

        QStringList virtualOptionsList = paramsArray[i].split('=');

        if (virtualOptionsList.size() > 1) {
            return optionsForEach(virtualOptionsList, availableOptions);
        }

        auto optionData = availableOptions.value(paramsArray[i], {{}});
        if (!checkOption(optionData, paramsArray[i])) {
            return false;
        }

        inputOptions.insert(paramsArray[i], optionData);

        if (paramsArray[i][0] == '-') {

            if (i < (paramsArray.size() - 1) && paramsArray[i + 1][0] != '-') {
                params[paramsArray[i].mid(1)] = paramsArray[i + 1];
                i++;
            } else {
                qCritical() << "Missing argument for " + paramsArray[i];
                return false;
            }
        } else {
            params[paramsArray[i]] = "";
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
#endif

#ifdef Q_OS_LINUX
    char path[2048];
    memset(path, 0, sizeof path);

    if (readlink("/proc/self/exe", path, 2048) < 0) {
        qCritical() << "parseParams can't get self path!";
        return false;
    }
    appPath =  QFileInfo(path).absolutePath();
    appName =  QFileInfo(path).fileName();

#endif
#ifdef Q_OS_DARWIN
    appPath =  QCoreApplication::applicationDirPath();
    appName =  QCoreApplication::applicationName();
#endif

    if (!appPath.size()) {
        return false;
    }

    if (!optionsForEach(paramsArray, availableOptions)) {
        return false;
    }

    printWorkingOptions();

    return true;
}

void Params::printWorkingOptions() {
    qDebug() << "--- Working options table start ---";

    QMap<QString, QString>::const_iterator iter = params.constBegin();
    while (iter != params.constEnd()) {

        QString row = QString{"Option[%0]"}.arg(iter.key());

        QString value = iter.value();
        if (!value.isEmpty()) {
            row += QString{": %1"}.arg(value);
        }

        qDebug() << row;

        ++iter;
    }

    qDebug() << "--- Working options table end ---";
}

bool Params::checkOption(const OptionData& optionData, const QString& rawOptionName) {

#ifndef QA_ALLOW_NOT_SUPPORTED_OPTIONS
    if (!optionData.isValid()) {

        qCritical() << QString("The '%0' option not exists!"
                               " You use wrong option name,"
                               " please check the help before run your commnad.").
                       arg(rawOptionName);

        return false;
    }
#else
    Q_UNUSED(rawOptionName);
#endif

    if (optionData.isDepricated()) {



        if (optionData.isRemoved()) {
            qCritical() << optionData.depricatedMsg();

            return false;
        }

        qWarning() << QString("The %0 option(s) marked as deprecated! "
                               "And most likely will be removed in next release.").
                       arg(optionData.names().join("/"));

        qWarning() << QString("Option message: %0").arg(optionData.depricatedMsg());

    }

    return true;
}

void Params::parseAvailableOptions(const OptionsDataList &availableOptionsListIn,
                                   OptionsDataList *availableOptionsListOut,
                                   Help::Section *helpOut) {

    if (!(availableOptionsListOut && helpOut))
        return;

    helpOut->clear();

    QHash<QString, Help::Options> options;
    for (auto it = availableOptionsListIn.begin(); it != availableOptionsListIn.end(); ++it) {

        if (availableOptionsListOut) {
            for (const auto &name : std::as_const(it.value().names())) {
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
