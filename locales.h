/*
 * Copyright (C) 2018-2023 QuasarApp.
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
 * @note If you want to add you own location of the qm files then add this into seccond arguments of the Locales::init method.
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
     * @return true if the all ltranstations files loaded successful.
     */
    static bool setLocale(const QLocale &locale, bool force = false);

    /**
     * @brief init This method initialize translations of applictaion.
     * @param locales This is list of locales that you want to locad to application cache. See info about QLocale.
     * @param location Path to folder with qm files. example (:/tr).
     * @return return true if locale set for application.
     */
    static bool init(const QList<QLocale> & locales,
                     const QSet<QString> & location = {});

    /**
     * @brief init This method initialize translation of applictaion.
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
     * @return current or last sets locate of applciation.
     */
    static const QLocale &currentLocate();

    /**
     * @brief tr This method will translate single string to choosed language.
     * @param source This is source string of translations.
     * @param locale This is choosed language.
     * @return translated string value.
     * @note use instant QOBject::tr, This method must be read by the lupdate utility
     */
    static QString tr(const char *source, const QLocale& locale);

signals:
    /**
     * @brief sigTranslationChanged Emited when set new locale for application.
     */
    void sigTranslationChanged();

private:
    Locales() = default;
    ~Locales();

    bool setLocalePrivate(const QLocale &locale = QLocale::system(), bool force = false, bool install = true);
    bool initPrivate(const QLocale &locale = QLocale::system(),
                     const QSet<QString> &location = {});

    bool initPrivate(const QList<QLocale> &locales,
                     const QSet<QString> &location = {});

    void clearCache(const QLocale& locale);
    void clearCache();

    void removeOldTranslation(const QLocale& locale);

    void addLocationPrivate(const QString& location);

    const QLocale &currentLocatePrivate() const;

    bool findQm(QString localePrefix,
                QList<QTranslator *> &result);
    bool findQmPrivate(const QString &prefix,
                       QList<QTranslator *> &qmFiles);
    void installTranslations(QList<QTranslator *> &qmFiles);
    QString translatePrivate(const char *source, const QLocale& locale);


    QLocale _currentLocate;
    QSet<QString> _locations;
    QHash<QLocale, QList<QTranslator *>> _translations;

};
}


#endif // LOCALES_H
