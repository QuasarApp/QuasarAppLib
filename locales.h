/*
 * Copyright (C) 2018-2019 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef LOCALES_H
#define LOCALES_H

#include "quasarapp_global.h"

class QCoreApplication;
class QTranslator;

namespace QuasarAppUtils {

/**
 * @brief The Locales class for parese local files
 */
class QUASARAPPSHARED_EXPORT Locales
{
public:
    Locales() = delete;

    /**
     * @brief initLocale init translation of applictaion
     * @param prefix - path to folder with qm files. example (/home)
     * @param locale - string value of locale. example (en)
     * @param app - app core of qt
     * @param translator - translator core of qt
     * @return return true if locale funded
     */
    static bool initLocale(const QString& prefix ,const QString &locale, QCoreApplication* app, QTranslator *translator);

};
}


#endif // LOCALES_H
