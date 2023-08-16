/*
 * Copyright (C) 2021-2023 QuasarApp.
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
 *  @code{cpp}
 *     auto settingsInstance = QuasarAppUtils::Setting::initService();
 *
 *     // on any another site you can use the instance method for get settings instance object.
 *     auto settingsInstance = QuasarAppUtils::Setting::instance();
 *
 *     // and you can destroy setting object if they are not needed anymore
 *     QuasarAppUtils::Setting::deinitService();
 *  @endcode
 *
 *  **Or** you can use the Settings::autoInstance method.
 *
 *  @code{cpp}
 *     auto settingsInstance = QuasarAppUtils::Setting::autoInstance();
 *  @endcode
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

    /**
     * @brief initService This method initialize default object of the QuasarAppUtils::Settings type.
     * @return true if initialization finished successfull else false.
     * @see ISettings::initService
     */
    static bool initService();

protected:

    void syncImplementation() override;
    QVariant getValueImplementation(const QString &key, const QVariant &def) override;
    void setValueImplementation(const QString key, const QVariant &value) override;
    bool ignoreToRest(const QString &) const override;
    QHash<QString, QVariant> defaultSettings() override;

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
