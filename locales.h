/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef LOCALES_H
#define LOCALES_H

#include "quasarapp_global.h"

#include <QLocale>
#include <QSet>
#include <QString>
#include <QDir>

class QCoreApplication;
class QTranslator;

namespace QuasarAppUtils {

/**
 * @brief The Locales class for parese local files
 * **Example :**
 * @code{cpp}
 * QuasarAppUtils::Locales::init();
 * @endcode
 *
 * @note If you want to add you own location of the qm files then add this into second arguments of the Locales::init method.
 *
 * @code{cpp}
 *  QuasarAppUtils::Locales::init(QLocale::system(), "myPath");
 * @endcode
 *
 * @note All translations qm files should be named with lower case example : en.qm
 */
class QUASARAPPSHARED_EXPORT Locales : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief setLocale This method sets locale for application and loaded all translations for this locale.
     * @param locale This is new locale.
     * @param force This is a flag that forces reload of the translations even if the locale was already set.
     * @return true if the all ltranstations files loaded successful.
     */
    static bool setLocale(const QLocale &locale, bool force = false);

    /**
     * @brief init This method initialize translations of application.
     * @param locales This is list of locales that you want to locad to application cache. See info about QLocale.
     * @param location Path to folder with qm files. example (:/tr).
     * @return return true if locale set for application.
     */
    static bool init(const QList<QLocale> & locales,
                     const QSet<QString> & location = {});

    /**
     * @brief init This method initialize translation of application.
     * @param locale See info about QLocale.
     * @param location Path to folder with qm files. example (:/tr).
     * @return return true if locale set for application.
     */
    static bool init(const QLocale &locale = QLocale::system(),
                     const QSet<QString> & location = {});

    /**
     * @brief addLocation This method add location for qm files. Use This method if you create a own library with translations supports.
     * @param location This is a new location of the qm files.
     */
    static void addLocation(const QString& location);

    /**
     * @brief instance This method return pointer to the Locales service.
     * @return return pointer to the Locales static object
     */
    static Locales *instance();

    /**
     * @brief currentLocate This method return current locate of applicatuon.
     * @return current or last sets locate of application.
     */
    static const QLocale &currentLocate();

    /**
     * @brief tr This method will translate single string to chosen language.
     * @param source This is source string of translations.
     * @param locale This is chosen language.
     * @return translated string value.
     * @note use instant QOBject::tr, This method must be read by the lupdate utility
     */
    static QString tr(const char *source, const QLocale& locale);

signals:
    /**
     * @brief sigTranslationChanged Emitted when set new locale for application.
     */
    void sigTranslationChanged();

private:
    Locales() = default;
    ~Locales();

    /**
     * @brief setLocalePrivate This is a private method for setting the locale of the application.
     * @param locale This is the new locale for application.
     * @param force This is a flag that forces reload of the translations.
     * @param install This is a flag that allows to install the translator in the QCoreApplication object.
     * @return true if the locale was set and the translations loaded successfully else false.
     */
    bool setLocalePrivate(const QLocale &locale = QLocale::system(), bool force = false, bool install = true);
    /**
     * @brief initPrivate This is a private method for initializing the translations of the application.
     * @param locale This is the locale for initialization. See info about QLocale.
     * @param location This is a list of paths to folders with qm files. example (:/tr).
     * @return true if the locale was set and the translations loaded successfully else false.
     */
    bool initPrivate(const QLocale &locale = QLocale::system(),
                     const QSet<QString> &location = {});

    /**
     * @brief initPrivate This is a private method for initializing the translations of the application.
     * @param locales This is a list of locales that will be loaded to the application cache. See info about QLocale.
     * @param location This is a list of paths to folders with qm files. example (:/tr).
     * @return true if the locale was set and the translations loaded successfully else false.
     */
    bool initPrivate(const QList<QLocale> &locales,
                     const QSet<QString> &location = {});

    /**
     * @brief clearCache This method removes all cached translations of the @a locale from the internal cache.
     * @param locale This is the locale that will be removed from the cache.
     */
    void clearCache(const QLocale& locale);
    /**
     * @brief clearCache This method removes all cached translations from the internal cache.
     */
    void clearCache();

    /**
     * @brief removeOldTranslation This method removes old installed translations of the @a locale from the QCoreApplication object.
     * @param locale This is the locale that translations will be removed.
     */
    void removeOldTranslation(const QLocale& locale);

    /**
     * @brief addLocationPrivate This is a private method for adding a new location of qm files to the internal list.
     * @param location This is a new location of the qm files.
     */
    void addLocationPrivate(const QString& location);

    /**
     * @brief currentLocatePrivate This is a private method that returns the current locale of the application.
     * @return current locale of the application.
     */
    const QLocale &currentLocatePrivate() const;

    /**
     * @brief findQm This method finds all qm translation files of the @a localePrefix in the registered locations.
     * @param localePrefix This is the prefix (code) of the locale for search, example: "en".
     * @param result This is the output list of the found translators.
     * @return true if at least one translation file was found else false.
     */
    bool findQm(QString localePrefix,
                QList<QTranslator *> &result);
    /**
     * @brief findQmPrivate This is a private method for finding translation files of the @a prefix in one registered location.
     * @param prefix This is the prefix (code) of the locale for search, example: "en".
     * @param qmFiles This is the output list of the found translators.
     * @return true if at least one translation file was found else false.
     */
    bool findQmPrivate(const QString &prefix,
                       QList<QTranslator *> &qmFiles);
    /**
     * @brief installTranslations This method installs the found translation files of the qmFiles list to the QCoreApplication object.
     * @param qmFiles This is the list of translators that will be installed.
     */
    void installTranslations(QList<QTranslator *> &qmFiles);
    /**
     * @brief translatePrivate This is a private method that translates the @a source string to the @a locale.
     * @param source This is the source string of translations.
     * @param locale This is the chosen language.
     * @return translated string value of the @a source string.
     */
    QString translatePrivate(const char *source, const QLocale& locale);

    /// @brief _currentLocate This is the current locale of the application.
    QLocale _currentLocate;

    /// @brief _locations This is the set of registered locations that contains qm translation files.
    QSet<QString> _locations;

    /// @brief _translations This is the cache of the loaded translators per locale.
    QHash<QLocale, QList<QTranslator *>> _translations;

};
}


#endif // LOCALES_H
