/*
 * Copyright (C) 2018-2020 QuasarApp.
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
 * @brief The Params class for parese app params
 */

enum VerboseLvl {
    Info = 0x0,
    Error = 0x1,
    Warning = 0x2,
    Debug = 0x3,

};

#define DEFAULT_VERBOSE_LVL "1"

class QUASARAPPSHARED_EXPORT Params
{
private:
    static QString timeString();
    static std::string lvlToString(VerboseLvl vLvl);
    static bool writeLoginFile(const QString& log, VerboseLvl vLvl = VerboseLvl::Debug);

public:
    Params() = delete;

    /**
     * @brief parseParams - parase input data of started application
     * @param argc - count of arguments
     * @param argv - arrat of arguments
     * @return true if all arguments read else false
     */
    static bool parseParams(const int argc, const char *argv[]);
    static bool parseParams(int argc, char *argv[]);

    /**
     * @brief parseParams - parase input data of started application
     * @param params -  arguments
     * @return true if all arguments read else false
     */
    static bool parseParams(const QStringList& paramsArray);

    /**
     * @brief getStrArg - get string value of key
     * @param key
     * @param def - default value
     * @return string value of argument
     */
    static QString getStrArg(const QString& key, const QString& def = "");

    /**
     * @brief getArg - get string value of key
     * @param key
     * @param def - default value
     * @return string value of argument
     */
    static QVariant getArg(const QString& key, const QVariant &def = {});

    /**
     * @brief setArg - set value of key
     * @param key
     */
    static void setArg(const QString& key, const QVariant& val);

    /**
     * @brief setArg - set boolean value of key
     * @param key
     * @param enable - new value of key
     */
    static void setEnable(const QString& key, bool enable);

    /**
     * @brief isEndable - check if enable argument of key
     * @param key
     * @return true if argument enabled
     */
    static bool isEndable(const QString& key);

    /**
     * @brief log - print text on console if the flag "vergose" is enabled
     * @param log - printed text
     */
    static void log(const QString& log, VerboseLvl vLvl = VerboseLvl::Debug);

    /**
     * @brief getparamsHelp
     * @return help string of default params
     */
    static Help::Charters getparamsHelp();

    /**
     * @brief showHelp - show all strings of help
     * @param help
     */
    static void showHelp(const QStringList& help);

    /**
     * @brief showHelp - show structe of help value
     * @param help
     */
    static void showHelp(const Help::Charters& help);

    /**
     * @brief size
     * @return size of all params array
     */
    static int size();

    /**
     * @brief customParamasSize
     * @return size of params entered in conosole
     */
    static int customParamasSize();

    /**
     * @brief showHelp - show base help section of QuasarAppLib
     */
    static void showHelp();
};
}

#endif // PARAMS_H
