/*
 * Copyright (C) 2018 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QUASARAAPP_H
#define QUASARAAPP_H

#include "quasaraapp_global.h"
#include <QVariant>


class QGuiApplication;
class QTranslator;

/**
 * @brief The QuasaraAppUtils class
 * this lib include base functions for the all applications of QuasarApp group.
 * all methods of the Quasar AppUtils is static
 */
class QUASARAAPPSHARED_EXPORT QuasaraAppUtils
{

public:
    QuasaraAppUtils() = delete;

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
     * @brief initLocale init translation of applictaion
     * @param locale - string value of locale. example (en)
     * @param app - app core of qt
     * @param translator - translator core of qt
     * @return return true if locale funded
     */
    static bool initLocale(const QString &locale, QGuiApplication* app, QTranslator *translator);
};

#endif // QUASARAAPP_H
