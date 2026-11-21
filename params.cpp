/*
 * Copyright (C) 2018-2026 QuasarApp.
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
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSet>
#include <QVector>
#include "qaglobalutils.h"
#include <QtLogging>

#ifdef Q_OS_DARWIN
#include <mach-o/dyld.h>
#endif

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

namespace {

/**
 * @brief strippedOptionName This method return the option name without the
 * leading dash ("-verbose" -> "verbose").
 * @param name This is raw option name.
 * @return option name without the leading dash.
 */
QString strippedOptionName(const QString &name) {
    return name.startsWith(QLatin1Char('-')) ? name.mid(1) : name;
}

/**
 * @brief optionValueName This method return the value name (placeholder) of
 * the QCommandLineOption that is generated from the @a optionData. The value
 * name must not be empty, otherwise the QCommandLineParser treats the option
 * as a boolean flag that takes no value.
 * @param optionData This is the option data that generates the QCommandLineOption.
 * @return non empty value name of the option.
 */
QString optionValueName(const OptionData &optionData) {
    QString valueName = optionData.arguments();
    if (valueName.startsWith(QLatin1Char('(')) && valueName.endsWith(QLatin1Char(')')))
        valueName = valueName.mid(1, valueName.size() - 2);

    valueName = valueName.trimmed();
    return valueName.isEmpty() ? QStringLiteral("value") : valueName;
}

} // namespace

bool Params::parseWithQCommandLineParser(const QStringList &paramsArray,
                                         const OptionsDataList &availableOptions) {

    // The QCommandLineParser keeps the added options as pointers, so the
    // QCommandLineOption objects are stored in a local list that stays alive
    // while the parser is in use.
    QVector<QCommandLineOption> optionList;
    QMap<QString, OptionData> optionDataByName; // the option name (without the dash) -> the option data

    QCommandLineParser parser;
    for (auto it = availableOptions.constBegin(); it != availableOptions.constEnd(); ++it) {
        const OptionData &optionData = it.value();
        if (!optionData.isValid())
            continue;

        for (const QString &rawName : optionData.names()) {
            const QString name = strippedOptionName(rawName);
            if (name.isEmpty() || optionDataByName.contains(name))
                continue; // the first definition of the name wins

            QCommandLineOption option(name, optionData.description(), optionValueName(optionData));
            if (!parser.addOption(option))
                continue;

            optionList.append(option);
            optionDataByName.insert(name, optionData);
        }
    }

    // Every option always requires a value. The value must be the next
    // argument and it must not look like an option (start with a dash).
    bool positionalMode = false;
    for (int i = 0; i < paramsArray.size(); ++i) {
        const QString &token = paramsArray[i];
        if (positionalMode)
            continue;

        if (token == QLatin1String("--")) {
            // the double dash ends the options parsing
            positionalMode = true;
            continue;
        }

        if (!token.startsWith(QLatin1Char('-')) || token.size() == 1
            || token.contains(QLatin1Char('=')))
            continue; // a positional argument, the "-" alone or the "option=value" form

        if (i + 1 >= paramsArray.size()
            || paramsArray[i + 1].startsWith(QLatin1Char('-'))) {
            qCritical() << "Missing argument for " + token;
            return false;
        }
    }

    // The QuasarApp options use the single dash prefix ("-verbose"), the
    // QCommandLineParser expects the double dash form ("--verbose") for the
    // long options. The arguments after the "--" separator are positional
    // and must not be transformed.
    QStringList parserArgs;
    parserArgs.reserve(paramsArray.size() + 1);
    positionalMode = false;
    for (const QString &token : paramsArray) {
        if (positionalMode) {
            parserArgs.append(token);
            continue;
        }

        if (token == QLatin1String("--")) {
            positionalMode = true;
            parserArgs.append(token);
            continue;
        }

        if (token.startsWith(QLatin1Char('-')) && token.size() > 1
            && !token.startsWith(QLatin1String("--"))) {
            parserArgs.append(QLatin1Char('-') + token);
        } else {
            parserArgs.append(token);
        }
    }


#ifdef QA_ALLOW_NOT_SUPPORTED_OPTIONS
    // Allow any command line option: register the unsupported options on the
    // fly, so that the QCommandLineParser can read their values.
    positionalMode = false;
    for (const QString &token : parserArgs) {
        if (positionalMode)
            break;

        if (token == QLatin1String("--")) {
            positionalMode = true;
            continue;
        }

        if (!token.startsWith(QLatin1String("--")) || token.size() <= 2)
            continue;

        const QString name = token.mid(2).section(QLatin1Char('='), 0, 0);
        if (name.isEmpty() || optionDataByName.contains(name))
            continue;

        QCommandLineOption unsupportedOption(name, QStringLiteral("Unsupported option"),
                                             QStringLiteral("value"));
        if (parser.addOption(unsupportedOption)) {
            optionList.append(unsupportedOption);
            optionDataByName.insert(name, OptionData(QStringList()));
        }
    }
#endif

    // The QCommandLineParser::parse method expects the executable name as
    // the first argument, it is skipped during the parsing.
    parserArgs.prepend(QStringLiteral("quasarapp"));

    if (!parser.parse(parserArgs)) {
#ifndef QA_ALLOW_NOT_SUPPORTED_OPTIONS
        const QStringList unknownOptions = parser.unknownOptionNames();
        if (!unknownOptions.isEmpty()) {
            qCritical() << QString("The '%0' option not exists!"
                                   " You use wrong option name,"
                                   " please check the help before run your command.")
                        .arg(QLatin1Char('-') + unknownOptions.first());
            return false;
        }
#endif
        qCritical() << parser.errorText();
        return false;
    }

#ifndef QA_ALLOW_NOT_SUPPORTED_OPTIONS
    // In the strict mode every argument that is not a known option
    // is rejected as well.
    if (!parser.positionalArguments().isEmpty()) {
        qCritical() << QString("The '%0' option not exists!"
                               " You use wrong option name,"
                               " please check the help before run your command.")
                    .arg(parser.positionalArguments().first());
        return false;
    }
#endif

    // An option value that was passed in the "option=value" form must not
    // look like an option (start with a dash) as well.
    for (const QString &name : parser.optionNames()) {
        const QString value = parser.value(name);
        if (value.startsWith(QLatin1Char('-'))) {
            qCritical() << "Missing argument for -" + name;
            return false;
        }
    }

    // Store the parsed options in the order in which they appear in the input.
    for (const QString &name : parser.optionNames()) {
        const OptionData &optionData = optionDataByName.value(name, OptionData(QStringList()));
        if (!checkOption(optionData))
            return false;

        inputOptions.insert(QLatin1Char('-') + name, optionData);
        params[name] = parser.value(name);
    }

    // The positional arguments are stored with the empty value,
    // the same way as in the previous implementation.
    for (const QString &positionalArgument : parser.positionalArguments()) {
        params[positionalArgument] = QString();
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
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);  // request to buffer size
    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) == 0) {
        appPath =  QString::fromUtf8(buffer.data());
    } else {
        appPath =  QString();
    }

    appName =  QCoreApplication::applicationName();
#endif

    if (!appPath.size()) {
        return false;
    }

    if (!parseWithQCommandLineParser(paramsArray, availableOptions)) {
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

bool Params::checkOption(const OptionData &optionData) {

    if (optionData.isDeprecated()) {

        if (optionData.isRemoved()) {
            qCritical() << optionData.deprecatedMsg();

            return false;
        }

        qWarning() << QString("The %0 option(s) marked as deprecated! "
                               "And most likely will be removed in next release.").
                    arg(optionData.names().join("/"));

        qWarning() << QString("Option message: %0").arg(optionData.deprecatedMsg());

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
