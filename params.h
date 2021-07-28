/*
 * Copyright (C) 2018-2021 QuasarApp.
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

#ifdef QT_DEBUG
#define DEFAULT_VERBOSE_LVL "3"
#else
#define DEFAULT_VERBOSE_LVL "2"
#endif

/**
 * @brief The Params class Contains fonctions for working with input arguments and logs.
 * This Class support next comandline arguments.
 *  * **-verbose** (level 1 - 3) Shows debug log
 *  * **-fileLog** (path to file) Sets path of log file. Default it is path to executable file with suffix '.log'
 */
class QUASARAPPSHARED_EXPORT Params
{
public:
    Params() = delete;

    /**
     * @brief parseParams Parse input data of started application.
     * @param argc Count of arguments.
     * @param argv Array of arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more inforamtion.
     *  If you skip thi argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(const int argc, const char *argv[], const OptionsDataList& options = {});

    /**
     * @brief parseParams Parse input data of started application.
     * @param argc Count of arguments.
     * @param argv Array of arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more inforamtion.
     *  If you skip thi argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(int argc, char *argv[], const OptionsDataList& options = {});

    /**
     * @brief parseParams Parse input data of started application.
     * @param paramsArray Arguments.
     * @param options This is list of the available options for parse. See the OptionData class for more inforamtion.
     *  If you skip thi argument then QuasarAppLib will not check input options.
     * @return true if all arguments read successful else false.
     */
    static bool parseParams(const QStringList& paramsArray, const OptionsDataList& options = {});

    /**
     * @brief getArg return string value of a @a key if key is exits else return a @a def value.
     *  If a @a def value not defined retunr empty string.
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
     * @brief setArg This method sets boolean value of key.
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
    static bool isDebugBuild();

    /**
     * @brief size This method return count of the all input arguments.
     * @return size of all params array.
     */
    static int size();

    /**
     * @brief showHelp This method shows all help message.
     * @param option This is option key that needed show a help message.
     */
    static void showHelp(const QString& option = "");

    /**
     * @brief showHelpForInputOptions This method show help for each input option.
     * @note Befor using of this method invoke the parseParams method. This is needed for generate the help message.
     *
     * **Example:**
     *
     * ```bash
     * myTool help option1 -option2 argumets
     * ```
     */
    static void showHelpForInputOptions();

    /**
     * @brief getHelpOfInputOptions This method return help abut input options only. Exept help and h options.
     * @return help abut input options only. Exept help and h options.
     */
    static Help::Section getHelpOfInputOptions();

    /**
     * @brief getHelp This method return options help page.
     * @note Befor using of this method invoke the parseParams method. This is needed for generate the help message.
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
    static QString timeString();
    static std::string lvlToString(VerboseLvl vLvl);
    static bool writeLoginFile(const QString& log, VerboseLvl vLvl = VerboseLvl::Debug);

    /**
     * @brief Traverse @a params and output its content (all the working
     *    options) to stdout in the form of option-value groups at the
     *    debug verbose level (-verbose 3).
     */
    static void printWorkingOptions();

    /**
     * @brief checkOption return tru if the option is supported
     * @param option checked option
     * @return true if option is supported
     */
    static bool checkOption(const OptionData &option, const QString &rawOptionName);

    /**
     * @brief parseAvailableOptions This is private method for parsing availabel options.
     * @param availableOptionsListIn input data of the available options.
     * @param availableOptionsListOut hash of available options wher key it options name and value it is options data
     * @param helpOut This is help object that generated from the
     */
    static void parseAvailableOptions(const OptionsDataList& availableOptionsListIn,
                                      OptionsDataList* availableOptionsListOut,
                                      Help::Section* helpOut);


    static QMap<QString, QString> params;
    static OptionsDataList inputOptions;

    static Help::Section userHelp;
    static QString appPath;
    static QString appName;

};
}

#endif // PARAMS_H
