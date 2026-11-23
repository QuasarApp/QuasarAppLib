/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "Config/config.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>

using namespace QuasarAppUtils;

// Constructor with full control over app name and organization
Config::Config(int argc, const char* argv[], const QList<QCommandLineOption>& params,
               const QString& appName, const QString& org)
    : _argc(argc)
    , _argv(static_cast<char**>(argv))
    , _supportedParams(params)
    , _appName(appName)
    , _orgName(org)
    , _mode(SettingsSaveMode::Auto)
{
    _parseArguments(argc, argv, params);
    _setConfigFile();
    _isInitialized = true;
}

// Convenience constructor with default app name and organization
Config::Config(int argc, const char* argv[], const QList<QCommandLineOption>& params)
    : Config(argc, argv, params, "Config", "Config")
{
}

// Destructor
Config::~Config() = default;

// ISettings interface implementation
QVariant Config::getValue(const QString &key, const QVariant& defaultValue)
{
    return _getPriorityValue(key, defaultValue);
}

QString Config::getStrValue(const QString &key, const QString& defaultValue)
{
    return _getPriorityValueStr(key, defaultValue);
}

void Config::resetToDefault()
{
    _settings.clear();
    _paramsMap.clear();
    _defaultValues.clear();
}

bool Config::ignoreToRest(const QString& key) const
{
    return false;
}

void Config::sync()
{
    _settings.sync();
}

void Config::forceReloadCache()
{
    _settings.beginReadArray("");
    _settings.endReadArray();
}

SettingsSaveMode Config::getMode() const
{
    return _mode;
}

QHash<QString, QVariant> Config::defaultSettings()
{
    return _defaultValues;
}

// Value access methods
int Config::valueInt(const QString& key, int defaultValue) const
{
    return _getPriorityValueInt(key, defaultValue);
}

bool Config::valueBool(const QString& key, bool defaultValue) const
{
    return _getPriorityValueBool(key, defaultValue);
}

double Config::valueDouble(const QString& key, double defaultValue) const
{
    return _getPriorityValueDouble(key, defaultValue);
}

QString Config::valueStr(const QString& key, const QString& defaultValue) const
{
    return _getPriorityValueStr(key, defaultValue);
}

QVariant Config::value(const QString& key, const QVariant& defaultValue) const
{
    return _getPriorityValue(key, defaultValue);
}

// Value source checking methods
bool Config::hasCommandlineValue(const QString& key) const
{
    return _paramsMap.contains(key);
}

bool Config::hasConfigFileValue(const QString& key) const
{
    return _settings.contains(key);
}

bool Config::hasDefaultValue(const QString& key) const
{
    return _defaultValues.contains(key);
}

// Priority check methods
bool Config::isCommandlineValue(const QString& key) const
{
    if (!_paramsMap.contains(key)) {
        return false;
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toString() == _paramsMap[key];
    }
    
    return !_defaultValues.contains(key) || _defaultValues.value(key).toString() == _paramsMap[key];
}

bool Config::isConfigFileValue(const QString& key) const
{
    if (!_settings.contains(key)) {
        return false;
    }
    
    if (_paramsMap.contains(key)) {
        return _paramsMap[key] == _settings.value(key).toString();
    }
    
    return !_defaultValues.contains(key) || _defaultValues.value(key).toString() == _settings.value(key).toString();
}

bool Config::isDefaultValue(const QString& key) const
{
    if (!_defaultValues.contains(key)) {
        return false;
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toString() == _defaultValues.value(key).toString();
    }
    
    if (_paramsMap.contains(key)) {
        return _paramsMap[key] == _defaultValues.value(key).toString();
    }
    
    return true;
}

// Value setting methods
void Config::setValue(const QString& key, const QVariant& value)
{
    _setValueWithPriority(key, value, 2);
    _settings.setValue(key, value);
    emit valueChanged(key, value);
}

void Config::setValue(const QString& key, const QString& value)
{
    setValue(key, QVariant::fromValue(value));
}

void Config::setValue(const QString& key, int value)
{
    setValue(key, QVariant::fromValue(value));
}

void Config::setValue(const QString& key, bool value)
{
    setValue(key, QVariant::fromValue(value));
}

void Config::setValue(const QString& key, double value)
{
    setValue(key, QVariant::fromValue(value));
}

// Config file access methods
QString Config::getConfigFileValue(const QString& key) const
{
    return _getConfigFileValue(key);
}

QString Config::getConfigFile() const
{
    return _configFile;
}

void Config::setConfigFile(const QString& filePath)
{
    _configFile = filePath;
    if (!_settings.fileName().isEmpty()) {
        _settings.setPath(_settings.fileName().dir().path() + QDir::separator() + filePath);
    } else {
        _settings.setPath(filePath);
    }
}

// Default values methods
QMap<QString, QVariant> Config::getDefaultValues() const
{
    return _defaultValues;
}

void Config::setDefaultValues(const QMap<QString, QVariant>& values)
{
    _defaultValues = values;
}

void Config::setDefaultValue(const QString& key, const QVariant& value)
{
    _defaultValues[key] = value;
}

void Config::setDefaultStr(const QString& key, const QString& value)
{
    setDefaultValue(key, QVariant::fromValue(value));
}

QString Config::getDefaultStr(const QString& key) const
{
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toString();
    }
    return QString();
}

void Config::setDefaultInt(const QString& key, int value)
{
    setDefaultValue(key, QVariant::fromValue(value));
}

int Config::getDefaultInt(const QString& key) const
{
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toInt();
    }
    return 0;
}

void Config::setDefaultBool(const QString& key, bool value)
{
    setDefaultValue(key, QVariant::fromValue(value));
}

bool Config::getDefaultBool(const QString& key) const
{
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toBool();
    }
    return false;
}

void Config::setDefaultDouble(const QString& key, double value)
{
    setDefaultValue(key, QVariant::fromValue(value));
}

double Config::getDefaultDouble(const QString& key) const
{
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toDouble();
    }
    return 0.0;
}

QVariant Config::getDefault(const QString& key) const
{
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key);
    }
    return QVariant();
}

// Helper methods
QVariant Config::_getPriorityValue(const QString& key, const QVariant& defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key].toVariant();
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key);
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key);
    }
    
    return defaultValue;
}

QString Config::_getPriorityValueStr(const QString& key, const QString& defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key];
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toString();
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toString();
    }
    
    return defaultValue;
}

int Config::_getPriorityValueInt(const QString& key, int defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key].toInt();
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toInt();
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toInt(defaultValue);
    }
    
    return defaultValue;
}

bool Config::_getPriorityValueBool(const QString& key, bool defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key].toBool(defaultValue);
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toBool();
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toBool(defaultValue);
    }
    
    return defaultValue;
}

double Config::_getPriorityValueDouble(const QString& key, double defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key].toDouble(defaultValue);
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key).toDouble();
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key).toDouble(defaultValue);
    }
    
    return defaultValue;
}

// Set value in the highest priority map
void Config::_setValueWithPriority(const QString& key, const QVariant& value, int priority)
{
    if (priority == 0) {
        _paramsMap[key] = value.toString();
    } else if (priority == 1) {
        _settings.setValue(key, value);
    } else {
        _defaultValues[key] = value;
    }
}

// Generate help text from supported parameters
QString Config::_generateHelpText() const
{
    QCommandLineParser parser;
    parser.setApplication(_appName);
    
    for (const auto& param : _supportedParams) {
        parser.addOption(param);
    }
    
    return parser.helpText();
}

// Parse command-line arguments
void Config::_parseArguments(int argc, const char* argv[], const QList<QCommandLineOption>& params)
{
    _argc = argc;
    _argv = static_cast<char**>(argv);
    _supportedParams = params;
    
    QCommandLineParser parser;
    parser.setApplication(_appName);
    parser.setApplicationDescription("Config Application");
    
    for (const auto& param : _supportedParams) {
        parser.addOption(param);
    }
    
    parser.parse(_argv);
    _paramsMap = parser.result();
}

// Set config file path based on app info
void Config::_setConfigFile()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (configDir.isEmpty()) {
        configDir = QDir::homePath() + QDir::separator() + ".config";
    }
    
    _configFile = _orgName + QDir::separator() + _appName + ".conf";
}

QString Config::_getCommandlineValue(const QString& key) const
{
    if (_paramsMap.contains(key)) {
        return _paramsMap[key];
    }
    return QString();
}

QString Config::_getConfigFileValue(const QString& key) const
{
    if (_settings.contains(key)) {
        return _settings.value(key).toString();
    }
    return QString();
}

// Help methods
void Config::printHelp()
{
    qDebug() << getHelp();
}

QString Config::getHelp() const
{
    return _generateHelpText();
}

QList<QCommandLineOption> Config::getSupportedParams() const
{
    return _supportedParams;
}

void Config::setSupportedParams(const QList<QCommandLineOption>& params)
{
    _supportedParams = params;
}

// Internal access methods
QSettings* Config::getSettings() const
{
    return &_settings;
}

QMap<QString, QString> Config::getInternalParamsMap() const
{
    return _paramsMap;
}

QMap<QString, QVariant> Config::getInternalDefaultValues() const
{
    return _defaultValues;
}

void Config::setDefaultValues(const QMap<QString, QVariant>& values)
{
    _defaultValues = values;
}


} // namespace QuasarAppUtils

#endif // CONFIG_CPP
