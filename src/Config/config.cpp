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

using namespace QuasarAppUtils;

// Constructor with full control over app name and organization
Config::Config(int argc, const char* argv[], const QList<QCommandLineOption>& params,
               const QString& appName, const QString& org)
    : QObject(nullptr)
    , _argc(argc)
    , _argv(static_cast<char**>(argv))
    , _supportedParams(params)
    , _appName(appName)
    , _orgName(org)
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

// Check if the Config object is valid
bool Config::isValid() const
{
    return _isInitialized;
}

// Value access methods - these use priority-based resolution
QVariant Config::value(const QString& key, const QVariant& defaultValue) const
{
    return _getPriorityValue(key, defaultValue);
}

QString Config::valueStr(const QString& key, const QString& defaultValue) const
{
    return _getPriorityValueStr(key, defaultValue);
}

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

// Value source checking methods
bool Config::hasCommandlineValue(const QString& key) const
{
    return _paramsMap.contains(key);
}

bool Config::hasConfigFileValue(const QString& key) const
{
    return _settings.contains(key);
}
// Priority check methods - determine which source has the winning value
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
// Private helper methods

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

// Get command-line value as string
QString Config::_getCommandlineValue(const QString& key) const
{
    return _paramsMap.value(key, QString());
}

// Get config file value as string
QString Config::_getConfigFileValue(const QString& key) const
{
    return _settings.value(key, QString()).toString();
}

// Get priority value (command-line > config > default)
QVariant Config::_getPriorityValue(const QString& key, const QVariant& defaultValue) const
{
    if (_paramsMap.contains(key)) {
        return QVariant::fromValue(_paramsMap[key]);
    }
    
    if (_settings.contains(key)) {
        return _settings.value(key);
    }
    
    if (_defaultValues.contains(key)) {
        return _defaultValues.value(key, defaultValue);
    }
    
    return defaultValue;
}

// Get priority value as string
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

// Get priority value as int
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

// Get priority value as bool
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

// Get priority value as double
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
{
    _defaultValues = values;
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
}

// Command-line access methods
QString Config::getCommandlineValue(const QString& key) const
{
    return _getCommandlineValue(key);
}

QMap<QString, QString> Config::getParamsMap() const
{
    return _paramsMap;
}
    if (_paramsMap.contains(key) || _settings.contains(key)) {
        return _paramsMap.value(key, "").isEmpty() && _settings.value(key, "").toString() == _defaultValues.value(key).toString();
    }
    
    return true;
}

bool Config::hasDefaultValue(const QString& key) const
{
    return _defaultValues.contains(key);
}