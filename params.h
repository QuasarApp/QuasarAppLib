/*
 * Copyright (C) 2018-2019 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PARAMS_H
#define PARAMS_H

#include <QVariant>
#include "quasarapp_global.h"


namespace QuasarAppUtils {

/**
 * @brief The Params class for parese app params
 */

class QUASARAPPSHARED_EXPORT Params
{
public:
    Params() = delete;

    /**
     * @brief parseParams - parase input data of started application
     * @param argc - count of arguments
     * @param argv - arrat of arguments
     * @return true if all arguments read else false
     */
    static bool parseParams(int argc, char *argv[]);

    /**
     * @brief getStrArg - get string value of key
     * @param key
     * @return string value of argument
     */
    static QString getStrArg(const QString& key);

    /**
     * @brief getArg - get string value of key
     * @param key
     * @return string value of argument
     */
    static QVariant getArg(const QString& key);

    /**
     * @brief isEndable - check if enable argument of key
     * @param key
     * @return true if argument enabled
     */
    static bool isEndable(const QString& key);

    /**
     * @brief verboseLog - print text on console if the flag "vergose" is enabled
     * @param log - printed text
     */
    static void verboseLog(const QString& log);
};
}

#endif // PARAMS_H
