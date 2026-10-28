/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef SETTINGS_H
#define SETTINGS_H

#include "quasarapp_global.h"
#include "isettings.h"
#include <QSet>
#include <QSettings>

namespace QuasarAppUtils {

/**
 * @brief The Settings class This is  wrapper of the QSettings object.
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
    /**
     * @brief Settings This is the main constructor of the Settings object.
     * @param format This is the format of the QSettings backend. By default QSettings::IniFormat.
     */
    Settings(QSettings::Format format = QSettings::IniFormat);

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
     * @return true if initialization finished successful else false.
     * @see ISettings::initService
     */
    static bool initService();

    /**
     * @brief autoInstance This method returns the default object of the QuasarAppUtils::Settings type
     * and if it was not initialized yet - it will be created automatically.
     * @return pointer to the default settings object.
     * @see ISettings::autoInstance
     */
    static ISettings *autoInstance();


protected:

    /**
     * @brief syncImplementation This method saves all settings data to the hard disk.
     * This is the QSettings based implementation of the ISettings::syncImplementation method.
     */
    void syncImplementation() override;
    /**
     * @brief getValueImplementation This method returns the value of the @a key setting from the QSettings backend.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not found.
     * @return value of a @a key
     */
    QVariant getValueImplementation(const QString &key, const QVariant &def) override;
    /**
     * @brief setValueImplementation This method sets a new value for the @a key setting in the QSettings backend.
     * @param key This is name of the changed setting.
     * @param value This is a new value of the setting.
     */
    void setValueImplementation(const QString key, const QVariant &value) override;
    /**
     * @brief ignoreToRest This method checks if the key is a boolean option.
     * @return true if the key option can't be reset to default else false.
     */
    bool ignoreToRest(const QString &) const override;
    /**
     * @brief defaultSettings This method returns the default map of the settings and their values.
     * By default the map is empty.
     * @return the default settings map.
     */
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
    /**
     * @brief setGroup This method sets a new settings group in the QSettings backend.
     * All values that will be read or written after the call will be stored in this group.
     * Use empty string for remove the current group.
     */
    void setGroup(const QString&);

private:
    /// @brief _settings This is the internal QSettings object that is used as a settings backend.
    QSettings *_settings = nullptr;

    /// @brief _boolOptions This is the set of the keys that has a bool type in the settings map.
    QSet<QString> _boolOptions;
};

}

#endif // SETTINGS_H
