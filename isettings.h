/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ISETTINGS_H
#define ISETTINGS_H

#include "quasarapp_global.h"
#include <QObject>
#include <QVariant>

class QSettings;

namespace QuasarAppUtils {

/**
 * @brief The SettingsSaveMode enum
 */
enum class SettingsSaveMode: quint64 {
    /// a settings will be saved on hard disk when called the Settings::setValue method.
    Auto,
    /// a settings will be saved on hard disk when called the Settings::Sync method.
    Manual
};

/**
 * @brief The Settings class base interface for implementation settings backends.
 * Available implementations:
 *  Setting (based on QSettings backend)
 * @note This is singleton object.
 *
 * @note The all child classes should be initialized before used.
 *
 * ```
 *     auto settingsInstance = Setting::init<Setting>();
 * ```
 *
 * @see ISettings::init method.
 *
 */
class QUASARAPPSHARED_EXPORT ISettings : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief init This method return instance of the settings object and initialize new settings model if object not exists.
     * @code{cpp}
         auto settingsObject = ISettings::init<SettingsModelClass>(arg...)
     * @endcode
     * @return pointer to a settings object;
     * @see ISettings::instance
     */
    template <class SettingsType, class... Args>
    static ISettings* init(Args&&... args) {
        static_assert (std::is_base_of<ISettings, SettingsType>::value,
                        "the Settingstype type must be ISettings");

        if(!_settings){
            _settings = new SettingsType(std::forward<Args>(args)...);
        }

        return _settings;
    }

    /**
     * @brief instance This method return pointer to current settings model. if this model not initialized then return nullptr.
     * @return pointer to current settings model if object initialized else nullptr.
     * @see ISettings::init
     */
    static ISettings *instance();

    /**
     * @brief getValue This method return the value of the settings.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not finded. If this params will be skipped, then The Settngs model try find default value in the defaultSettings map.
     * @return value of a @a key
     * @see ISettings::defaultSettings
     */
    Q_INVOKABLE QVariant getValue(const QString &key, const QVariant& def = {});

    /**
     * @brief getStrValue some as getValue but convert result object to QString type.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not finded. If this params will be skipped, then The Settngs model try find default value in the defaultSettings map.
     * @return value of a @a key
     * @see ISettings::defaultSettings
     * @warning If you set a def arguments to empty string then this method will return default value from the defaultSettings map.
     */
    Q_INVOKABLE QString getStrValue(const QString &key, const QString& def = {});

    /**
     * @brief resetToDefault This method reset all settings to default values.
     */
    Q_INVOKABLE void resetToDefault();

    /**
     * @brief sync This method save all setings data on a hard disk;
     */
    void sync();

    /**
     * @brief forceReloadCache This method force reload settings data from disk.
     * @note Cache will be refreshed
     */
    void forceReloadCache();

    /**
     * @brief getMode This method return the current mode of the settings.
     * @return the current mode of the settings.
     */
    SettingsSaveMode getMode() const;

    /**
     * @brief setMode This method sets a new value of the settings mode.
     * @param mode This is a new value of the settings mode.
     */
    void setMode(const SettingsSaveMode &mode);

public slots:
    /**
     * @brief setValue This slot sets new value for a @a key setting
     * @param key This is name of the changed setting.
     * @param value This is a new value of the setting
     */
    void setValue(const QString &key, const QVariant& value);

    /**
     * @brief setStrValue This is some as setValue but working with the QString type.
     * @param key This is name of the changed setting.
     * @param value This is a new value of the setting
     */
    void setStrValue(const QString& key, const QString& value);


signals:
    /**
     * @brief valueChanged This signal when value of the @a key settings changed
     * @param key This is name of change setting.
     * @param value This is a new value of @a key.
     */
    void valueChanged(QString key, QVariant value);

    /**
     * @brief valueStrChanged some as valueChanged(QString key, QVariant value) but value has ben converted to the QString type.
     * @param key This is name of change setting.
     * @param value This is a new value of @a key.
     */
    void valueStrChanged(QString key, QString value);

protected:

    explicit ISettings(SettingsSaveMode mode = SettingsSaveMode::Auto);
    ~ISettings() override;

    /**
     * @brief defaultSettings This method must be return default map of the settings and them values. If the default value argument in a getValue method will be skipped, then settings model try find a default value in this map.
     * @return The default settings map.
     * @see ISettings::getValue
     * @example example of implementation of this method:
     *
     * @code{cpp}
     *
        QHash<QString, QVariant> SettingsModel::defaultSettings() {
            QHash<QString, QVariant> settings;

            settings["colorTheme"] = "#ff6b01";
            settings["shareName"] = true;
            settings["devSettingEnable"] = false;
            settings["host"] = "";
            settings["APIVersion"] = 2;

            return settings;
        }
     * @endcode
     */
    virtual QHash<QString, QVariant> defaultSettings() = 0;

    /**
     * @brief syncImplementation This method should save all configuration data to the hard drive;
     */
    virtual void syncImplementation() = 0;

    /**
     * @brief getValueImplementation This method will return the value of the settings.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not finded.
     * @return value of a @a key
     */
    virtual QVariant getValueImplementation(const QString &key, const QVariant& def) = 0;

    /**
     * @brief setValueImplementation This slot will set a new value for the @a key parameter.
     * @param key This is name of the changed setting.
     * @param value This is a new value of the setting
     */
    virtual void setValueImplementation(const QString key, const QVariant& value) = 0;

    /**
     * @brief clearCache This method clear all data from cache.
     */
    void clearCache();
private:

    QHash<QString, QVariant>& settingsMap();

    SettingsSaveMode _mode = SettingsSaveMode::Auto;

    QHash<QString, QVariant> _cache;
    QHash<QString, QVariant> *_defaultConfig = nullptr;

    static ISettings* _settings;
};
} ;


#endif // ISETTINGS_H
