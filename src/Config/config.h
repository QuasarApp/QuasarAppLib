/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Settings/settings.h"

#include <QCommandLineOption>
#include <QHash>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

namespace QuasarAppUtils {

/**
 * @brief The Config class
 *
 * The Config is a Settings implementation that takes into account the
 * command-line arguments that had been read. The value of a setting is
 * resolved with the following priority (the highest first):
 *
 * 1. The value that was provided on the command line.
 * 2. The value that is saved in the config file (the Settings backend).
 * 3. The default value.
 *
 * The command-line options are declared as QCommandLineOption objects and
 * passed to the Config constructor. If an option was not provided on the
 * command line and it has a non-empty default value, that default value is
 * used as the default value (the lowest priority layer) of the setting.
 *
 * The config file is the same file that is used by the Settings base class,
 * so it is taken from the QCoreApplication::applicationName() and the
 * QCoreApplication::organizationName(). They should be set before the
 * Config object will be created.
 *
 * Example of the Config usage:
 *
 * @code{cpp}
 *     int main(int argc, char* argv[]) {
 *         QCoreApplication app(argc, argv);
 *         QCoreApplication::setApplicationName("MyApp");
 *         QCoreApplication::setOrganizationName("MyOrg");
 *
 *         QList<QCommandLineOption> params;
 *         params << QCommandLineOption{"verbose", "Enable the verbose logging", "level", "0"};
 *         params << QCommandLineOption{"output", "The output file", "path", ""};
 *
 *         QuasarAppUtils::Config config(argc, argv, params);
 *
 *         int level = config.valueInt("verbose");    // command line > config file > default
 *         QString file = config.valueStr("output");
 *     }
 * @endcode
 *
 * @note The command-line value has the highest priority.
 * @note The value saved in the config file has the second priority.
 * @note The default value has the lowest priority.
 * @note The command-line option names follow the QCommandLineOption naming
 * rules of the used Qt version. In the Qt 6 versions the option names must
 * not start with the '-' character (the 'verbose' option is provided on
 * the command line as '--verbose').
 */
class QUASARAPPSHARED_EXPORT Config: public Settings
{
public:
    /**
     * @brief Config This is the main constructor of the Config object.
     * It parses the @a argv command-line arguments with the @a params list
     * of the supported QCommandLineOption entries and uses the Settings
     * base class as the config file backend.
     * @param argc The number of the command-line arguments.
     * @param argv The command-line arguments array.
     * @param params The list of the supported QCommandLineOption entries.
     */
    Config(int argc, const char* argv[], const QList<QCommandLineOption>& params);

    /**
     * @brief ~Config This is the destructor of the Config object.
     */
    ~Config() override;

    /**
     * @brief isValid This method checks if the Config object is valid
     * (the constructor finished successfully).
     * @return true if the Config object is valid.
     */
    bool isValid() const;

    /**
     * @brief value This method returns the value of the @a key setting.
     * The value is resolved with the priority: command line, config file,
     * default value. If the value was not found in any of the layers, the
     * @a def value will be returned.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the value of the @a key setting.
     */
    QVariant value(const QString& key, const QVariant& def = {}) const;

    /**
     * @brief valueStr some as the Config::value method, but the result is
     * converted to the QString type.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the string value of the @a key setting.
     */
    QString valueStr(const QString& key, const QString& def = {}) const;

    /**
     * @brief valueInt some as the Config::value method, but the result is
     * converted to the int type.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the int value of the @a key setting.
     */
    int valueInt(const QString& key, int def = 0) const;

    /**
     * @brief valueBool some as the Config::value method, but the result is
     * converted to the bool type.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the bool value of the @a key setting.
     */
    bool valueBool(const QString& key, bool def = false) const;

    /**
     * @brief valueDouble some as the Config::value method, but the result
     * is converted to the double type.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the double value of the @a key setting.
     */
    double valueDouble(const QString& key, double def = 0.0) const;

    /**
     * @brief hasCommandlineValue This method checks if the @a key setting
     * was provided on the command line.
     * @param key This is the name of the checked setting.
     * @return true if the @a key setting was provided on the command line.
     */
    bool hasCommandlineValue(const QString& key) const;

    /**
     * @brief hasConfigFileValue This method checks if the @a key setting
     * is saved in the config file.
     * @param key This is the name of the checked setting.
     * @return true if the @a key setting is saved in the config file.
     */
    bool hasConfigFileValue(const QString& key) const;

    /**
     * @brief hasDefaultValue This method checks if the @a key setting has
     * a default value.
     * @param key This is the name of the checked setting.
     * @return true if the @a key setting has a default value.
     */
    bool hasDefaultValue(const QString& key) const;

    /**
     * @brief isCommandlineValue This method checks if the current value of
     * the @a key setting is the command-line value. The command-line value
     * has the highest priority, so this method returns true if the @a key
     * setting was provided on the command line.
     * @param key This is the name of the checked setting.
     * @return true if the current value of the @a key setting is the
     * command-line value.
     */
    bool isCommandlineValue(const QString& key) const;

    /**
     * @brief isConfigFileValue This method checks if the current value of
     * the @a key setting is the value that is saved in the config file.
     * @param key This is the name of the checked setting.
     * @return true if the current value of the @a key setting is the value
     * that is saved in the config file.
     */
    bool isConfigFileValue(const QString& key) const;

    /**
     * @brief isDefaultValue This method checks if the current value of the
     * @a key setting is the default value.
     * @param key This is the name of the checked setting.
     * @return true if the current value of the @a key setting is the
     * default value.
     */
    bool isDefaultValue(const QString& key) const;

    /**
     * @brief getCommandlineValue This method returns the value of the @a key
     * setting that was provided on the command line.
     * @param key This is the name of the required setting value.
     * @return the command-line value of the @a key setting or an empty
     * string if the value was not provided on the command line.
     */
    QString getCommandlineValue(const QString& key) const;

    /**
     * @brief getParamsMap This method returns the map of the settings that
     * had been provided on the command line.
     * @return the map of the command-line settings.
     */
    QMap<QString, QString> getParamsMap() const;

    /**
     * @brief getSupportedParams This method returns the list of the
     * supported QCommandLineOption entries that were passed to the
     * Config constructor.
     * @return the list of the supported QCommandLineOption entries.
     */
    QList<QCommandLineOption> getSupportedParams() const;

    /**
     * @brief getConfigFileValue This method returns the value of the @a key
     * setting that is saved in the config file.
     * @param key This is the name of the required setting value.
     * @return the config file value of the @a key setting or an empty
     * string if the value is not saved in the config file.
     */
    QString getConfigFileValue(const QString& key) const;

    /**
     * @brief setDefaultValues This method adds the @a values to the map of
     * the default values (the lowest priority layer). The values that were
     * provided in the @a values map override the default values of the
     * command-line options with the same keys.
     * @param values This is the map of the new default values.
     */
    void setDefaultValues(const QHash<QString, QVariant>& values);

    /**
     * @brief setDefaultValue This method sets a new default value for the
     * @a key setting (the lowest priority layer).
     * @param key This is the name of the changed setting.
     * @param value This is the new default value of the setting.
     */
    void setDefaultValue(const QString& key, const QVariant& value);

    /**
     * @brief getDefault This method returns the default value of the @a key
     * setting.
     * @param key This is the name of the required setting value.
     * @return the default value of the @a key setting or an invalid
     * QVariant if the default value is not set.
     */
    QVariant getDefault(const QString& key) const;

    /**
     * @brief getDefaultValues This method returns the map of the default
     * values (the lowest priority layer).
     * @return the map of the default values.
     */
    QHash<QString, QVariant> getDefaultValues() const;

    /**
     * @brief printHelp This method prints the help text of the supported
     * command-line options to the standard output.
     */
    void printHelp() const;

    /**
     * @brief getHelp This method returns the help text of the supported
     * command-line options.
     * @return the help text of the supported command-line options.
     */
    QString getHelp() const;

protected:
    /**
     * @brief defaultSettings This method returns the map of the default
     * values of the settings (the lowest priority layer).
     * @return the map of the default values.
     */
    QHash<QString, QVariant> defaultSettings() override;

    /**
     * @brief getValueImplementation This method returns the value of the
     * @a key setting. The value is resolved with the priority: command
     * line, config file (the Settings backend), default value.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the value of the @a key setting.
     */
    QVariant getValueImplementation(const QString& key, const QVariant& def) override;

    /**
     * @brief setValueImplementation This method saves a new value for the
     * @a key setting to the config file (the Settings backend).
     * @param key This is the name of the changed setting.
     * @param value This is the new value of the setting.
     */
    void setValueImplementation(const QString key, const QVariant& value) override;

private:
    /**
     * @brief _parseArguments This method parses the command-line arguments
     * with the QCommandLineParser and fills the internal maps.
     * @param params The list of the supported QCommandLineOption entries.
     */
    void _parseArguments(const QList<QCommandLineOption>& params);

    /**
     * @brief _priorityValue This method returns the value of the @a key
     * setting resolved with the priority: command line, config file,
     * default value.
     * @param key This is the name of the required setting value.
     * @param def This is the value that will be returned if the setting
     * value was not found in any of the layers.
     * @return the value of the @a key setting.
     */
    QVariant _priorityValue(const QString& key, const QVariant& def) const;

    /**
     * @brief _normalizeKey This method removes the leading dashes from the
     * command-line option name and returns it as the setting key.
     * @param name This is the name of the command-line option.
     * @return the setting key.
     */
    static QString _normalizeKey(const QString& name);

    /// @brief _argc This is the number of the command-line arguments.
    int _argc;

    /// @brief _argv This is the command-line arguments array.
    const char** _argv;

    /// @brief _supportedParams This is the list of the supported QCommandLineOption entries.
    QList<QCommandLineOption> _supportedParams;

    /// @brief _paramsMap This is the map of the settings that had been provided on the command line.
    QMap<QString, QString> _paramsMap;

    /// @brief _defaultValues This is the map of the default values (the lowest priority layer).
    QHash<QString, QVariant> _defaultValues;

    /// @brief _isInitialized This is the flag that shows that the Config constructor finished successfully.
    bool _isInitialized = false;
};

} // namespace QuasarAppUtils

#endif // CONFIG_H