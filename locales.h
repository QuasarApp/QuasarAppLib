/*
 * Copyright (C) 2018-2021 QuasarApp.
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

class QCoreApplication;
class QTranslator;

namespace QuasarAppUtils {

/**
 * @brief The Locales class for parese local files
 * Example :
 * @code{cpp}
 * QuasarAppUtils::Locales::init()
 * @endcode
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
    static bool setLocale(const QLocale &locale);

    /**
     * @brief init This method initialize translation of applictaion.
     * @param locale See info about QLocale.
     * @param location Path to folder with qm files. example (:/tr).
     * @return return true if locale set for application.
     */
    static bool init(const QLocale &locale = QLocale::system(),
                     const QSet<QString> & location = {});
    /**
     * @brief instance
     * @return return static object
     */
    static Locales *instance();

signals:
    /**
     * @brief sigTranslationChanged Emited when set new locale for application.
     */
    void sigTranslationChanged();

private:
    Locales() = default;
    ~Locales();

    bool setLocalePrivate(const QLocale &locale = QLocale::system());
    bool initPrivate(const QLocale &locale = QLocale::system(),
                     const QSet<QString> &location = {});
    void removeOldTranslation();

    QSet<QString> _locations;
    QList<QTranslator *> _translations;

};
}


#endif // LOCALES_H
