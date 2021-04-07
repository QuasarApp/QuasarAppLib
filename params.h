/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PARAMS_H
#define PARAMS_H

#include <QVariant>
#include "quasarapp_global.h"
#include "helpdata.h"

namespace QuasarAppUtils {

/**
 * @brief The Params class for parese app params.
 */

enum VerboseLvl {
    Info    = 0x0,
    Error   = 0x1,
    Warning = 0x2,
    Debug   = 0x3,

};

#ifdef RELEASE_BUILD
#define DEFAULT_VERBOSE_LVL "1"

#else
#define DEFAULT_VERBOSE_LVL "3"

#endif
class QUASARAPPSHARED_EXPORT Params
{
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

public:
    Params() = delete;

    /**
     * @brief parseParams Parse input data of started application.
     * @param argc Count of arguments.
     * @param argv Array of arguments.
     * @return true if all arguments read else false.
     */
    static bool parseParams(const int argc, const char *argv[]);
    static bool parseParams(int argc, char *argv[]);

    /**
     * @brief parseParams Parse input data of started application.
     * @param paramsArray Arguments.
     * @return true if all arguments read else false.
     */
    static bool parseParams(const QStringList& paramsArray);

    /**
     * @brief getArg Get string value of key.
     * @param key This is key of the parameter.
     * @param def Default value.
     * @return string value of argument.
     */
    static QString getArg(const QString& key, const QString &def = {});

    /**
     * @brief setArg Sets value of key.
     * @param key This is new value of the @a key.
     */
    static void setArg(const QString& key, const QString& val);

    /**
     * @brief setArg Sets boolean value of key.
     * @param key
     * @param enable New value of key.
     */
    static void setEnable(const QString& key, bool enable);

    /**
     * @brief isEndable Check if enable argument of key.
     * @param key
     * @return true if argument enabled.
     */
    static bool isEndable(const QString& key);

    /**
     * @brief log Print text on console if the flag "vergose" is enabled.
     * @param log Printed textP.     
     */
    static void log(const QStrinP& log, VerboseLvl vLvl = VerboseLvl::Debug);

    /**
     * @brief getparamsHelp
     * @return help string of default params.
     */
    static Help::Charters getparamsHelp();

    /**
     * @brief showHelp Show all strings of help.
     * @param help
     */
    static void showHelp(const QStringList& help);

    /**
     * @brief showHelp Show structe of help value.
     * @param help
     */
    static void showHelp(const Help::Charters& help);

    /**
     * @brief getVerboseLvl This method return the verbose log level.
     * @return verbose lvl
     */
    static VerboseLvl getVerboseLvl();

    /**
     * @brief isDebug
     * @return true if verbose lvl >= 3
     */
    static bool isDebug();

    /**
     * @brief isDebugBuild This method return true if the library buildet in debug mode.
     * @return true if this library buildet in debug mode.
     */
    static bool isDebugBuild();


    /**
     * @brief size This method return size of all params array.
     * @return size Of all params array.
     */
    static int size();

    /**
     * @brief customParamasSize This method return a size of params entered in conosole.
     * @return size of params entered in conosole.
     */
    static int customParamasSize();

    /**
     * @brief showHelp - show base help section of QuasarAppLib.
     */
    static void showHelp();

    /**
     * @brief getUserParamsMap.
     * @return QVariantMap With user params.
     */
    static QMap<QString, QString> getUserParamsMap();

    /**
     * @brief clearParsedData - This method clear allparsed data.
     */
    static void clearParsedData();

    /**
     * @brief getCurrentExecutable
     * @return path to current executable.
     */
    static QString getCurrentExecutable();

    /**
     * @brief getCurrentExecutableDir This method return a path to a folder with the current executable.
     * @return path of executable.
     */
    static QString getCurrentExecutableDir();

};
}

#endif // PARAMS_H
