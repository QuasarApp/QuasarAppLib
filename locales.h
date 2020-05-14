/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef LOCALES_H
#define LOCALES_H

#include "quasarapp_global.h"

#include <QString>

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
     * @param locale - string value of locale. example (en) by default empty. @note If use by default this function set sstem language.
     * @return return true if locale set for application
     */
    static bool setLocale(const QString& prefix , const QString &locale = {});

private:
    /**
     * @brief initLocale
     * @return true if function finished seccusseful
     */
    static bool initLocale(QTranslator*);

    /**
     * @brief getTranslator
     * @return instance of Translation
     */
    static QTranslator* getTranslator();

};
}


#endif // LOCALES_H
