/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Settings/isettings.h"
#include "Params/params.h"
#include <QObject>
#include <QMap>
#include <QSettings>
#include <QCommandLineOption>
#include <QCommandLineParser>

namespace QuasarAppUtils {

/**
 * @brief The Config class
 * 
 * Combines settings functionality with command-line parameter parsing.
 * The Config object works as a settings class while having access to the
 * parameters module.
 * 
 * @note Command-line values provided to the app have the highest priority.
 * @note Values saved in the config file have secondary priority.
 * @note Default values saved in the settings have the lowest priority.
 */
class QUASARAPPSHARED_EXPORT Config : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor with full control over app name and organization
     * @param argc Command-line argument count
     * @param argv Command-line argument array
     * @param params List of supported QCommandLineOption entries
     * @param appName Application name
     * @param org Organization name
     */
    Config(int argc, const char* argv[], const QList<QCommandLineOption>& params,
           const QString& appName = "Config", const QString& org = "Config");

    /**
     * @brief Convenience constructor with default app name and organization
     * @param argc Command-line argument count
     * @param argv Command-line argument array
     * @param params List of supported QCommandLineOption entries
     */
    Config(int argc, const char* argv[], const QList<QCommandLineOption>& params);

    /**
     * @brief Destructor
     */
    ~Config() override = default;

    /**
    /**
     * @brief Get value with priority resolution (command-line > config > default)
     * @param key Setting key
     * @param defaultValue Default value if key not found
     * @return The value from highest priority source
     */
    QVariant value(const QString& key, const QVariant& defaultValue = {}) const;

    /**
     * @brief Get string value with priority resolution
     * @param key Setting key
     * @param defaultValue Default value if key not found
     * @return The string value from highest priority source
     */
    QString valueStr(const QString& key, const QString& defaultValue = {}) const;

    /**
     * @brief Get integer value with priority resolution
     * @param key Setting key
     * @param defaultValue Default value if key not found
     * @return The integer value from highest priority source
     */
    int valueInt(const QString& key, int defaultValue = 0) const;

    /**
     * @brief Get boolean value with priority resolution
     * @param key Setting key
    // Value source checking methods
    /**
     * @brief Check if value exists in command-line parameters
     * @param key Setting key
     * @return true if key exists in command-line parameters
     */
    bool hasCommandlineValue(const QString& key) const;

    /**
     * @brief Check if value exists in config file
     * @param key Setting key
     * @return true if key exists in config file
     */
    bool hasConfigFileValue(const QString& key) const;

    /**
     * @brief Check if default value exists for key
     * @param key Setting key
     * @return true if default value exists for key
     */
    bool hasDefaultValue(const QString& key) const;
     * @param defaultValue Default value if key not found
    // Priority check methods - determine which source has the winning value
    /**
     * @brief Check if the current value has command-line priority
     * @param key Setting key
     * @return true if command-line value takes precedence
     */
    bool isCommandlineValue(const QString& key) const;

    /**
     * @brief Check if the current value has config file priority
     * @param key Setting key
     * @return true if config file value takes precedence
     */
    bool isConfigFileValue(const QString& key) const;

    /**
     * @brief Check if the current value has default priority
     * @param key Setting key
     * @return true if default value takes precedence
     */
    bool isDefaultValue(const QString& key) const;
    // Value setting methods
    /**
     * @brief Set value for a key
     * @param key Setting key
     * @param value Value to set
     */
    void setValue(const QString& key, const QVariant& value);

    /**
     * @brief Set string value
     * @param key Setting key
     * @param value String value to set
     */
    void setValue(const QString& key, const QString& value);

    /**
     * @brief Set integer value
     * @param key Setting key
     * @param value Integer value to set
     */
    void setValue(const QString& key, int value);
    // Command-line access methods
    /**
     * @brief Get command-line value as string
     * @param key Setting key
     * @return Command-line value as string, or empty string if not found
     */
    QString getCommandlineValue(const QString& key) const;

    /**
     * @brief Get all parsed command-line parameters
     * @return Map of all parsed parameters
     */
    QMap<QString, QString> getParamsMap() const;

    /**
     * @brief Set boolean value
     * @param key Setting key
     * @param value Boolean value to set
     */
    void setValue(const QString& key, bool value);

    // Config file access methods
    /**
     * @brief Get config file value as string
     * @param key Setting key
     * @return Config file value as string, or empty string if not found
     */
    QString getConfigFileValue(const QString& key) const;

    /**
     * @brief Get config file path
     * @return Path to the config file
     */
    QString getConfigFile() const;

    /**
     * @brief Set config file path
     * @param filePath Path to the config file
     */
    void setConfigFile(const QString& filePath);
    /**
     * @brief Set double value
    // Default values methods
    /**
     * @brief Get map of default values
     * @return Map of default values
     */
    QMap<QString, QVariant> getDefaultValues() const;

    /**
     * @brief Set default values
     * @param values Map of default values
     */
    void setDefaultValues(const QMap<QString, QVariant>& values);
     * @param key Setting key
     * @param value Double value to set
     */
    void setValue(const QString& key, double value);
     * @return The boolean value from highest priority source
     */
    bool valueBool(const QString& key, bool defaultValue = false) const;

    /**
     * @brief Get double value with priority resolution
    // Help methods
    /**
     * @brief Print help to stdout
     */
    void printHelp();

    /**
     * @brief Get help text as string
     * @return Help text
     */
    QString getHelp() const;

    /**
     * @brief Get list of supported QCommandLineOption objects
     * @return List of supported parameters
     */
    QList<QCommandLineOption> getSupportedParams() const;

    /**
     * @brief Set supported parameters
     * @param params List of supported parameters
     */
    void setSupportedParams(const QList<QCommandLineOption>& params);
     * @param key Setting key
     * @param defaultValue Default value if key not found
     * @return The double value from highest priority source
    // Internal access methods
    /**
     * @brief Get internal QSettings pointer
     * @return Pointer to internal QSettings object
     */
    QSettings* getSettings() const;

signals:
    /**
     * @brief Emitted when a value changes
     * @param key The setting key that changed
     * @param value The new value
     */
    void valueChanged(const QString& key, const QVariant& value);

protected:
    /**
     * @brief Get priority value (command-line > config > default)
     * @param key Setting key
     * @param defaultValue Default value
     * @return The value from highest priority source
     */
    QVariant _getPriorityValue(const QString& key, const QVariant& defaultValue) const;

    /**
     * @brief Get priority value as string
     * @param key Setting key
     * @param defaultValue Default value
     * @return The string value from highest priority source
     */
    QString _getPriorityValueStr(const QString& key, const QString& defaultValue) const;

    /**
     * @brief Get priority value as int
     * @param key Setting key
     * @param defaultValue Default value
     * @return The int value from highest priority source
     */
    int _getPriorityValueInt(const QString& key, int defaultValue) const;

    /**
     * @brief Get priority value as bool
     * @param key Setting key
     * @param defaultValue Default value
     * @return The bool value from highest priority source
     */
    bool _getPriorityValueBool(const QString& key, bool defaultValue) const;

    /**
     * @brief Get priority value as double
     * @param key Setting key
     * @param defaultValue Default value
     * @return The double value from highest priority source
     */
    double _getPriorityValueDouble(const QString& key, double defaultValue) const;

    /**
     * @brief Set value in the highest priority map
     * @param key Setting key
     * @param value Value to set
     * @param priority Priority level (0=command-line, 1=config, 2=default)
     */
    void _setValueWithPriority(const QString& key, const QVariant& value, int priority);

    /**
     * @brief Generate help text from supported parameters
     * @return Generated help text
     */
    QString _generateHelpText() const;

private:
    /**
     * @brief Parse command-line arguments
     * @param argc Argument count
     * @param argv Argument array
     * @param params List of supported options
     */
    void _parseArguments(int argc, const char* argv[], const QList<QCommandLineOption>& params);

    /**
     * @brief Set config file path based on app info
     */
    void _setConfigFile();

    /**
     * @brief Get command-line value as string
     * @param key Setting key
     * @return Command-line value or empty string
     */
    QString _getCommandlineValue(const QString& key) const;

    /**
     * @brief Get config file value as string
     * @param key Setting key
     * @return Config file value or empty string
     */
    QString _getConfigFileValue(const QString& key) const;

    // Internal data members
    int _argc;
    char** _argv;
    QList<QCommandLineOption> _supportedParams;
    QSettings _settings;
    QMap<QString, QString> _paramsMap;
    QMap<QString, QVariant> _defaultValues;
    QString _configFile;
    QString _appName;
    QString _orgName;
    bool _isInitialized = false;
};

} // namespace QuasarAppUtils

#endif // CONFIG_H
     */
    double valueDouble(const QString& key, double defaultValue = 0.0) const;
     * @brief Check if the Config object is valid
     * @return true if the Config object is initialized
     */
    bool isValid() const { return _isInitialized; }