/*
 * Copyright (C) 2021-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef SETTINGS_H
#define SETTINGS_H

#include "quasarapp_global.h"
#include "isettings.h"

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
public:
    Settings();

    /**
    * @brief init This is simple wrapper of the Settings::init method for convenient access to initialisation.
    * @return instance of the setting.
    */
    static ISettings* init();

    // ISettings interface
protected:



    void syncImplementation();
    QVariant getValueImplementation(const QString &key, const QVariant &def);
    void setValueImplementation(const QString key, const QVariant &value);


private:
    QSettings *_settings = nullptr;
};

}

#endif // SETTINGS_H
