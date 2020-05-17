/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef LOCALES_H
#define LOCALES_H

#include "quasarapp_global.h"

#include <QLocale>
#include <QString>

class QCoreApplication;
class QTranslator;

namespace QuasarAppUtils {

/**
 * @brief The Locales class for parese local files
 */
class QUASARAPPSHARED_EXPORT Locales : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief setLocale set translation of applictaion
     * @param locale - see info about QLocale
     * @param location - path to folder with qm files. example (:/tr)
     * @param file - prefix for translations.
     * @return return true if locale set for application
     */
    static bool setLocale(const QLocale &locale = {}, const QString& file = {}, const QString& delimiter = "_", const QString& location = "");

    /**
     * @brief translate init translation of applictaion
     * @param locale - see info about QLocale
     * @param location - path to folder with qm files. example (:/tr)
     * @param file - prefix for translations.
     * @return return true if locale set for application
     */
    bool translate(const QLocale &locale = {},
                   const QString& file = {},
                   const QString& delimiter = "_",
                   const QString& location = "");

    /**
     * @brief instance
     * @return return static object
     */
    static Locales *instance();

signals:
    /**
     * @brief sigTranslationChanged - emited when set new locale for application.
     */
    void sigTranslationChanged();

private:
    Locales() = default;

    /**
     * @brief getTranslator
     * @return instance of Translation
     */
    static QTranslator* getTranslator();

};
}


#endif // LOCALES_H
