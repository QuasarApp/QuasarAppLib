/*
 * Copyright (C) 2021-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef SETTINGS_H
#define SETTINGS_H

#include "quasarapp_global.h"
#include "isettings.h"
#include <QSet>

namespace QuasarAppUtils {

/**
 * @brief The Settings class This is  wraper of the QSettings object.
 *
 * Example of initialisation :
 *
 *  ```
 *     auto settingsInstance = Setting::init();
 *  ```
 *
 *  @see Settings::init
 */
class QUASARAPPSHARED_EXPORT Settings: public ISettings
{
    Q_OBJECT
public:
    Settings();

    // ISettings interface
    /**
     * @brief boolOptions returns current map with keys that has a bool type.
     * @return current map with keys that has a bool type
     * @see Settings::setBoolOptions
     * @see Settings::isBool
    */
    const QSet<QString> &boolOptions() const;

    /**
     * @brief setBoolOptions This method sets new map of the boolean options.
     * @param newBoolOptions This is new map of the boolean options.
     * @see Settings::boolOptions
     * @see Settings::isBool
     */
    void setBoolOptions(const QSet<QString> &newBoolOptions);

protected:

    void syncImplementation();
    QVariant getValueImplementation(const QString &key, const QVariant &def);
    void setValueImplementation(const QString key, const QVariant &value);

    /**
     * @brief isBool This method should be return true if the key's type is bool.
     * This is needed because QVariant will be converted alvays to true value in a qml code.
     * @param key This is key of checks setting.
     * @return true if the key is boolean variable.
     * The default implementation check key in the inner map.
     *
     * @see Settings::setBoolOptions
     * @see Settings::boolOptions
     */
    virtual bool isBool(const QString& key) const;
    void setGroup(const QString&);

private:
    QSettings *_settings = nullptr;
    QSet<QString> _boolOptions;
};

}

#endif // SETTINGS_H
