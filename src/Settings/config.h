/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Settings/settings.h"
#include "Params/params.h"

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
 * 1. The value that was provided on the command line (read by the
 *    Params class).
 * 2. The value that is saved in the config file (the Settings backend).
 * 3. The default value (the def argument of the ISettings::getValue
 *    method or the ISettings::defaultSettings map).
 *
 * All the command-line parsing logic is already implemented in the
 * Params class (the Params::parseParams method), so the Config class
 * does not use the QCommandLineParser directly.
 *
 * The config file is the same file that is used by the Settings base
 * class, so it is taken from the QCoreApplication::applicationName()
 * and the QCoreApplication::organizationName(). They should be set
 * before the Config object will be created.
 *
 * Example of the Config usage:
 *
 * @code{cpp}
 *     int main(int argc, char* argv[]) {
 *         QCoreApplication app(argc, argv);
 *         QCoreApplication::setApplicationName("MyApp");
 *         QCoreApplication::setOrganizationName("MyOrg");
 *
 *         OptionsDataList options;
 *         options.insert("Base Options",
 *                        OptionData{{"-verbose"}, "(level 1 - 3)", "Shows debug log"});
 *         options.insert("Base Options",
 *                        OptionData{{"-output"}, "(path)", "The output file"});
 *
 *         QuasarAppUtils::Config config(argc, argv, options);
 *
 *         // command line > config file > default value
 *         int level = config.getValue("verbose").toInt();
 *         QString file = config.getStrValue("output");
 *     }
 * @endcode
 *
 * @note The command-line value has the highest priority.
 * @note The value saved in the config file has the second priority.
 * @note The default value has the lowest priority.
 */
class Config: public Settings
{
public:
    /**
     * @brief Config This is the main constructor of the Config object.
     * It parses the @a argv command-line arguments with the @a options
     * list of the supported options using the Params class and uses the
     * Settings base class as the config file backend.
     * @param argc The number of the command-line arguments.
     * @param argv The command-line arguments array. The first argument
     * is the executable name and will be skipped.
     * @param options The list of the supported options. See the
     * OptionData class for more information. If you skip this argument
     * then only the QuasarAppLib base options (the -verbose and the
     * -fileLog options) and the positional arguments will be parsed.
     */
    Config(int argc, const char* argv[], const OptionsDataList& options = {});

    /**
     * @brief commandLineArgs This method returns the snapshot of the
     * command-line arguments that had been read by the Params class in
     * the Config constructor.
     * @return the map of the command-line arguments (the option name
     * without the dash -> the argument value).
     */
    const QMap<QString, QString>& commandLineArgs() const;

protected:
    /**
     * @brief getValueImplementation This method returns the value of
     * the @a key setting. The value is resolved with the priority:
     * command line (the Params class), config file (the Settings
     * backend), default value (the @a def argument).
     * @param key This is the name of the required setting value.
     * @param def This is the default value if the value is not found.
     * @return the value of the @a key setting.
     */
    QVariant getValueImplementation(const QString& key, const QVariant& def) override;

private:
    /// @brief _paramsMap This is the snapshot of the command-line
    /// arguments that had been read by the Params class in the Config
    /// constructor. The command-line values have the highest priority
    /// in the getValueImplementation method.
    QMap<QString, QString> _paramsMap;
};

}
#endif // CONFIG_H