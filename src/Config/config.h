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
#include <memory>

namespace QuasarAppUtils {

/**
 * @brief The Config class
 * 
 * Combines settings functionality with command-line parameter parsing.
 * The Config object works as a settings class while having access to the
 * parameters module.
 * 
 * @note This class implements ISettings and uses singleton pattern.
 * @note Command-line values provided to the app have the highest priority.
 * @note Values saved in the config file have secondary priority.
 * @note Default values saved in the settings have the lowest priority.
 */
class QUASARAPPSHARED_EXPORT Config: public ISettings
{
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
    ~Config() override;

    /**
     * @brief Check if the Config object is valid (has been initialized)
     * @return true if the Config object is valid
     */
    bool isValid() const { return _isInitialized; }

    // ISettings interface implementation
    Q_INVOKABLE QVariant getValue(const QString &key, const QVariant& defaultValue = {}) override;
    Q_INVOKABLE QString getStrValue(const QString &key, const QString& defaultValue = {}) override;
    Q_INVOKABLE void resetToDefault() override;
    bool ignoreToRest(const QString& key) const override;
    void sync() override;
    void forceReloadCache() override;
    SettingsSaveMode getMode() const override;
    QHash<QString, QVariant> defaultSettings() override;

    // Implementation of pure virtual methods from ISettings
    void syncImplementation() override;
    QVariant getValueImplementation(const QString &key, const QVariant& defaultValue) override;
    void setValueImplementation(const QString key, const QVariant& value) override;
    bool isBool(const QString& key) const override;

    // Convenience methods
    int valueInt(const QString& key, int defaultValue = 0) const;
    bool valueBool(const QString& key, bool defaultValue = false) const;
    double valueDouble(const QString& key, double defaultValue = 0.0) const;
    QString valueStr(const QString& key, const QString& defaultValue = {}) const;
    QVariant value(const QString& key, const QVariant& defaultValue = {}) const;

    // Value source checking
    bool hasCommandlineValue(const QString& key) const;
    bool hasConfigFileValue(const QString& key) const;
    bool hasDefaultValue(const QString& key) const;
    bool isCommandlineValue(const QString& key) const;
    bool isConfigFileValue(const QString& key) const;
    bool isDefaultValue(const QString& key) const;

    // Value setting
    Q_INVOKABLE void setValue(const QString& key, const QVariant& value) override;
    Q_INVOKABLE void setValue(const QString& key, const QString& value) override;
    Q_INVOKABLE void setValue(const QString& key, int value) override;
    Q_INVOKABLE void setValue(const QString& key, bool value) override;
    Q_INVOKABLE void setValue(const QString& key, double value) override;

    // Command-line access
    QString getCommandlineValue(const QString& key) const;
    QMap<QString, QString> getParamsMap() const;

    // Config file access
    QString getConfigFileValue(const QString& key) const;
    QString getConfigFile() const;
    void setConfigFile(const QString& filePath);

    // Help methods
    void printHelp();
    QString getHelp() const;
    QList<QCommandLineOption> getSupportedParams() const;
    void setSupportedParams(const QList<QCommandLineOption>& params);

    // Internal access
    QSettings* getSettings() const;
    QMap<QString, QString> getInternalParamsMap() const;
    QMap<QString, QVariant> getInternalDefaultValues() const;
    void setDefaultValues(const QMap<QString, QVariant>& values);
    void setDefaultValue(const QString& key, const QVariant& value);
    QVariant getDefault(const QString& key) const;
    void setDefaultStr(const QString& key, const QString& value);
    QString getDefaultStr(const QString& key) const;
    void setDefaultInt(const QString& key, int value);
    int getDefaultInt(const QString& key) const;
    void setDefaultBool(const QString& key, bool value);
    bool getDefaultBool(const QString& key) const;
    void setDefaultDouble(const QString& key, double value);
    double getDefaultDouble(const QString& key) const;
    QMap<QString, QVariant> getDefaultValues() const;

signals:
    void valueChanged(const QString& key, const QVariant& value);
    void valueStrChanged(const QString& key, const QString& value);

protected:
    QVariant _getPriorityValue(const QString& key, const QVariant& defaultValue) const;
    QString _getPriorityValueStr(const QString& key, const QString& defaultValue) const;
    int _getPriorityValueInt(const QString& key, int defaultValue) const;
    bool _getPriorityValueBool(const QString& key, bool defaultValue) const;
    double _getPriorityValueDouble(const QString& key, double defaultValue) const;
    void _setValueWithPriority(const QString& key, const QVariant& value, int priority);
    QString _generateHelpText() const;
    void _parseArguments(int argc, const char* argv[], const QList<QCommandLineOption>& params);
    void _setConfigFile();
    QString _getCommandlineValue(const QString& key) const;
    QString _getConfigFileValue(const QString& key) const;

private:
    int _argc;
    char** _argv;
    QList<QCommandLineOption> _supportedParams;
    QSettings* _settings = nullptr;
    QMap<QString, QString> _paramsMap;
    QMap<QString, QVariant> _defaultValues;
    QString _configFile;
    QString _appName;
    QString _orgName;
    SettingsSaveMode _mode = SettingsSaveMode::Auto;
    bool _isInitialized = false;
};

} // namespace QuasarAppUtils

#endif // CONFIG_H
