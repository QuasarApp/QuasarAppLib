/*
 * Copyright (C) 2018-2021 QuasarApp.
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
 * @brief The Settings class This is wraper of the QSettings object.
 * @note This is singleton object.
 */
class QUASARAPPSHARED_EXPORT ISettings : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief instance This method return instance of the settings object
     * @return pointer to a settings object;
     */
    template <class SettingsType>
    static ISettings* instance() {
        static_assert (std::is_base_of<SettingsType, ISettings>::value,
                        "the Settingstype type must be ISettings");

        if(!_settings){
            _settings = new Settingstype();
        }

        return _settings;
    }

    /**
     * @brief getValue This method return the value of the settings.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not finded.
     * @return value of a @a key
     */
    Q_INVOKABLE QVariant getValue(const QString &key, const QVariant& def);

    /**
     * @brief getStrValue some as getValue but convert result object to QString type.
     * @param key This is name of the required settings value.
     * @param def This is default value if a value is not finded.
     * @return value of a @a key
     */
    Q_INVOKABLE QString getStrValue(const QString &key, const QString& def);

    /**
     * @brief sync This method save all setings data on a hard disk;
     */
    void sync();

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
    void setValue(const QString key, const QVariant& value);

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

private:
    SettingsSaveMode _mode = SettingsSaveMode::Auto;
    static ISettings* _settings;
};
} ;


#endif // ISETTINGS_H
