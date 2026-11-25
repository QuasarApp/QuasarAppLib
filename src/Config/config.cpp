/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "Config/config.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

namespace QuasarAppUtils {

Config::Config(int argc, const char* argv[], const QList<QCommandLineOption>& params)
    : _argc(argc)
    , _argv(argv)
{
    _parseArguments(params);
    _isInitialized = true;
}

Config::~Config() = default;

bool Config::isValid() const {
    return _isInitialized;
}

QString Config::_normalizeKey(const QString& name) {
    QString key = name;
    while (key.startsWith('-')) {
        key = key.mid(1);
    }
    return key;
}

void Config::_parseArguments(const QList<QCommandLineOption>& params) {
    _supportedParams = params;

    QStringList arguments;
    for (int i = 0; i < _argc && _argv && _argv[i]; ++i) {
        arguments.append(QLatin1String(_argv[i]));
    }

    if (!arguments.isEmpty()) {
        QCommandLineParser parser;
        parser.setApplicationDescription("The command-line options of the "
                                         + QCoreApplication::applicationName());

        for (const auto& param : params) {
            parser.addOption(param);
        }

        if (parser.parse(arguments)) {
            // The values that had been provided on the command line
            // have the highest priority.
            for (const auto& param : params) {
                if (!parser.isSet(param)) {
                    continue;
                }

                const QString value = parser.value(param);
                for (const auto& name : param.names()) {
                    _paramsMap.insert(_normalizeKey(name), value);
                }
            }
        } else {
            qWarning() << "The command-line arguments parse error:" << parser.errorText();
        }
    }

    // The default value of a declared option (if it is not empty)
    // is the default value (the lowest priority layer) of the setting.
    for (const auto& param : params) {
        const QStringList defaults = param.defaultValues();
        if (defaults.isEmpty() || defaults.first().isEmpty()) {
            continue;
        }

        for (const auto& name : param.names()) {
            _defaultValues.insert(_normalizeKey(name), defaults.first());
        }
    }
}

QVariant Config::value(const QString& key, const QVariant& def) const {
    return _priorityValue(key, def);
}

QString Config::valueStr(const QString& key, const QString& def) const {
    return _priorityValue(key, QVariant(def)).toString();
}

int Config::valueInt(const QString& key, int def) const {
    return _priorityValue(key, QVariant(def)).toInt();
}

bool Config::valueBool(const QString& key, bool def) const {
    return _priorityValue(key, QVariant(def)).toBool();
}

double Config::valueDouble(const QString& key, double def) const {
    return _priorityValue(key, QVariant(def)).toDouble();
}

bool Config::hasCommandlineValue(const QString& key) const {
    return _paramsMap.contains(key);
}

bool Config::hasConfigFileValue(const QString& key) const {
    const QSettings* settings = getSettings();
    return settings && settings->contains(key);
}

bool Config::hasDefaultValue(const QString& key) const {
    return _defaultValues.contains(key);
}

bool Config::isCommandlineValue(const QString& key) const {
    // The command-line value has the highest priority.
    return hasCommandlineValue(key);
}

bool Config::isConfigFileValue(const QString& key) const {
    return !hasCommandlineValue(key) && hasConfigFileValue(key);
}

bool Config::isDefaultValue(const QString& key) const {
    return !hasCommandlineValue(key) && !hasConfigFileValue(key) && hasDefaultValue(key);
}

QString Config::getCommandlineValue(const QString& key) const {
    return _paramsMap.value(key);
}

QMap<QString, QString> Config::getParamsMap() const {
    return _paramsMap;
}

QList<QCommandLineOption> Config::getSupportedParams() const {
    return _supportedParams;
}

QString Config::getConfigFileValue(const QString& key) const {
    const QSettings* settings = getSettings();
    if (settings && settings->contains(key)) {
        return settings->value(key).toString();
    }
    return QString();
}

void Config::setDefaultValues(const QHash<QString, QVariant>& values) {
    for (auto it = values.cbegin(); it != values.cend(); ++it) {
        _defaultValues.insert(it.key(), it.value());
    }
}

void Config::setDefaultValue(const QString& key, const QVariant& value) {
    _defaultValues.insert(key, value);
}

QVariant Config::getDefault(const QString& key) const {
    return _defaultValues.value(key);
}

QHash<QString, QVariant> Config::getDefaultValues() const {
    return _defaultValues;
}

void Config::printHelp() const {
    qInfo().noquote() << getHelp();
}

QString Config::getHelp() const {
    QString appName = QCoreApplication::applicationName();
    if (appName.isEmpty()) {
        appName = "app";
    }

    QString help;
    help += "Usage: " + appName + " [options]\n";
    help += "The command-line options:\n";

    for (const auto& param : _supportedParams) {
        QStringList names;
        for (const auto& name : param.names()) {
            // The single character names are the short options (-name),
            // the others are the long options (--name).
            names << (name.size() == 1 ? "-" : "--") + name;
        }

        QString option = names.join(", ");
        if (!param.valueName().isEmpty()) {
            option += " " + param.valueName();
        }

        help += "  " + option + "\n";
        help += "      " + param.description() + "\n";
    }

    return help;
}

// The ISettings virtual methods. The Config is a Settings implementation
// that takes into account the command-line arguments that had been read,
// so the value resolution is done with the priority:
// command line > config file (the Settings backend) > default value.

QHash<QString, QVariant> Config::defaultSettings() {
    return _defaultValues;
}

QVariant Config::getValueImplementation(const QString& key, const QVariant& def) {
    return _priorityValue(key, def);
}

void Config::setValueImplementation(const QString key, const QVariant& value) {
    // The value is saved in the config file (the Settings backend).
    Settings::setValueImplementation(key, value);
}

QVariant Config::_priorityValue(const QString& key, const QVariant& def) const {
    // 1. The command-line value has the highest priority.
    if (_paramsMap.contains(key)) {
        return QVariant(_paramsMap.value(key));
    }

    // 2. The value that is saved in the config file.
    const QSettings* settings = getSettings();
    if (settings && settings->contains(key)) {
        return settings->value(key);
    }

    // 3. The default value has the lowest priority.
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key);
    }

    return def;
}

} // namespace QuasarAppUtils