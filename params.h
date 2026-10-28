/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PARAMS_H
#define PARAMS_H

#include <QMap>
#include <QVariant>
#include "quasarapp_global.h"
#include "helpdata.h"
#include "optiondata.h"

namespace QuasarAppUtils {


/**
 * @brief The VerboseLvl enum uses for sets log level.
 */
enum VerboseLvl {
    /// Error message. This logs will marked as a **Error** and printing if the verbose lvl >= 0
    Error   = 0x0,
    /// Warning message. This logs will marked as a **Warning** and printing if the verbose lvl >= 1
    Warning = 0x1,
    /// General information. This logs will marked as a **Info** and and printing if the verbose lvl >= 2.
    Info    = 0x2,
    /// Debug message. This logs will marked as a **Debug** and printing if the verbose lvl >= 3
    Debug   = 0x3,

};
#ifdef QA_DISABLE_LOG
#define DEFAULT_VERBOSE_LVL "0"
#else
#ifdef QT_DEBUG
#define DEFAULT_VERBOSE_LVL "3"
#else
#define DEFAULT_VERBOSE_LVL "2"
#endif
#endif

/**
 * @brief The Params class Contains functions for working with input arguments and logs.
 * This Class support next commandline arguments.
 *  * **-verbose** (level 1 - 3) Shows debug log
 *  * **-fileLog** (path to file) Sets path of log file. Default it is path to executable file with suffix '.log'
 *
 * ### Usage
 *
 * ```cpp
 *
 * #include <quasarapp.h>
 *
 * if (!QuasarAppUtils::Params::parseParams(argc, argv)) {
 *     QuasarAppUtils::Params::log("Warning message", QuasarAppUtils::Warning);
 *     QuasarAppUtils::Params::showHelp();
 *     exit(0);
 * }
 *
 * ```
 */
class QUASARAPPSHARED_EXPORT Params
{
public:
    Params() = delete;

    /**
     * @brief parseParams Parse input data of started application.
     * @param argc Count of arguments.
     * @param argv Array of arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more information.
     *  If you skip this argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(const int argc, const char *argv[], const OptionsDataList& options = {});

    /**
     * @brief parseParams Parse input data of started application.
     * @param argc Count of arguments.
     * @param argv Array of arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more information.
     *  If you skip this argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(int argc, char *argv[], const OptionsDataList& options = {});

    /**
     * @brief parseParams Parse input data of started application.
     * @param paramsArray Arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more information.
     *  If you skip this argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(const QStringList& paramsArray, const OptionsDataList& options = {});

    /**
     * @brief getArg return string value of a @a key if key is exits else return a @a def value.
     *  If a @a def value not defined returns empty string.
     * @param key This is key of a console argument.
     * @param def This is Default value. If key not exits This function will return a default value.
     * @return a string value of argument.
     */
    static QString getArg(const QString& key, const QString &def = {});

    /**
     * @brief setArg sets a new value of a @a key.
     * @param key This is a name of sets option.
     * @param val This is a new value of the @a key.
     */
    static void setArg(const QString& key, const QString& val);

    /**
     * @brief setEnable This method sets boolean value of key.
     * @param key This is name of the console option.
     * @param enable New value of key.
     * @note For check is enable @a key argument use the Params::isEndable method.
     */
    static void setEnable(const QString& key, bool enable);

    /**
     * @brief isEndable This method check if enable a @a key argument.
     * @param key This is name of the validate arguments
     * @return true if argument enabled.
     */
    static bool isEndable(const QString& key);

    /**
     * @brief log This method print @a log text on console.
     * @param log This is printed text message.
     * @param vLvl This is verbose level of message, for get more information see the QuasarAppUtils::VerboseLvl enum.
     * @note All messages will be printed according to the current verbose setting.
     * @note The verbose level sets by verbose option on console.
     */
    [[deprecated("Use QALogger and qt debug functions(qDebug, qInfo, qError...)")]]
    static void log(const QString& log, VerboseLvl vLvl = VerboseLvl::Debug);

    /**
     * @brief getVerboseLvl This method return the verbose log level.
     * @return verbose log lvl.
     */
    static VerboseLvl getVerboseLvl();

    /**
     * @brief isDebug This method return true if the application verbose level >= VerboseLvl::Debug.
     * @return true if a verbose level >= VerboseLvl::Debug
     */
    static bool isDebug();

    /**
     * @brief isDebugBuild This method return true if the library buildet in debug mode.
     * @return true if this library buildet in debug mode.
     */
    [[deprecated("Use Qt MACROSSSES")]]
    static bool isDebugBuild();

    /**
     * @brief size This method return count of the all input arguments.
     * @return size of all params array.
     */
    static int size();

    /**
     * @brief showHelp This method shows all help message.
     */
    static void showHelp();

    /**
     * @brief showHelpForInputOptions This method show help for each input option.
     * @note Before using of this method invoke the parseParams method. This is needed for generate the help message.
     *
     * **Example:**
     *
     * ```bash
     * myTool help option1 -option2 arguments
     * ```
     */
    static void showHelpForInputOptions();

    /**
     * @brief getHelpOfInputOptions This method return help abut input options only. Except help and h options.
     * @return help abut input options only. Except help and h options.
     */
    static Help::Section getHelpOfInputOptions();

    /**
     * @brief getHelp This method return options help page.
     * @note Before using of this method invoke the parseParams method. This is needed for generate the help message.
     * @return help of available options.
     */
    static const Help::Section& getHelp();

    /**
     * @brief getUserParamsMap This method return const reference to the parsed arguments map.
     * @return A map object with parsed arguments.
     */
    static const QMap<QString, QString> &getUserParamsMap();

    /**
     * @brief clearParsedData This method clear all parsed data.
     */
    static void clearParsedData();

    /**
     * @brief getCurrentExecutable This method return path to the current executable.
     * @return path to current executable.
     */
    static QString getCurrentExecutable();

    /**
     * @brief getCurrentExecutableDir This method return a path to a folder with the current executable.
     * @return path of the folder with current executable.
     */
    static QString getCurrentExecutableDir();

    /**
     * @brief availableArguments This method return list of the available arguments of QuasarAppLibrary
     * @return list of the available arguments
     */
    static OptionsDataList availableArguments();

private:

    /**
     * @brief optionsForEach This is a private method for parsing the input options.
     * Iterates over the @a paramsArray and checks every parameter against the
     * @a availableOptions list. Unsupported options are reported according to
     * the QA_ALLOW_NOT_SUPPORTED_OPTIONS build option and the parsing result
     * is stored in the static params map.
     * @param paramsArray This is the input array of the raw parameters.
     * @param availableOptions This is the list of the available options for check.
     * @return true if all input options are supported and parsed correctly else false.
     */
    static bool optionsForEach(const QStringList& paramsArray,
                               const OptionsDataList &availableOptions);

    /**
     * @brief Traverse @a params and output its content (all the working
     *    options) to stdout in the form of option-value groups at the
     *    debug verbose level (-verbose 3).
     */
    static void printWorkingOptions();

    /**
     * @brief checkOption return true if the option is supported
     * @param option checked option
     * @return true if option is supported
     */
    static bool checkOption(const OptionData &option, const QString &rawOptionName);

    /**
     * @brief parseAvailableOptions This is private method for parsing available options.
     * @param availableOptionsListIn input data of the available options.
     * @param availableOptionsListOut hash of available options where key it options name and value it is options data
     * @param helpOut This is help object that generated from the
     */
    static void parseAvailableOptions(const OptionsDataList& availableOptionsListIn,
                                      OptionsDataList* availableOptionsListOut,
                                      Help::Section* helpOut);


    /// @brief params This is the static map with all parsed key-value input arguments.
    static QMap<QString, QString> params;

    /// @brief inputOptions This is the static list of the available input options that was parsed in the parseParams method.
    static OptionsDataList inputOptions;

    /// @brief userHelp This is the static help section generated from the available input options.
    static Help::Section userHelp;

    /// @brief appPath This is the static path of the current executable file.
    static QString appPath;

    /// @brief appName This is the static name of the current executable file.
    static QString appName;

};
}

#endif // PARAMS_H
