/*
 * Copyright (C) 2018-2019 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include "quasarapp_global.h"
#include <QObject>
#include <QVariant>

class QSettings;

namespace QuasarAppUtils {

/**
 * @brief The SettingsSaveMode enum
 * Auto - value save on hard disk when calling method "value"
 * manual - save all data on hard disk when calling method Settings::sync
 */
enum class SettingsSaveMode: quint64 {
    Auto,
    Manual
};

/**
 * @brief The Settings class - singleton for QSettings
 */
class QUASARAPPSHARED_EXPORT Settings : public QObject
{
    Q_OBJECT
private:
    explicit Settings(SettingsSaveMode mode = SettingsSaveMode::Auto);
    QSettings *_settings = nullptr;
    SettingsSaveMode _mode = SettingsSaveMode::Auto;

    static Settings* initSettings(SettingsSaveMode mode = SettingsSaveMode::Auto);
public:

    /**
     * @brief get
     * @return object of all settings app;
     */
    static Settings* get();

    /**
     * @brief get
     * @return const object of all settings app;
     */
    static const Settings* getConst();

    /**
     * @brief getValue
     * @param key - key of value
     * @param def - default value if is value not finded
     * @return value of key
     */
    Q_INVOKABLE QVariant getValue(const QString &key, const QVariant& def);

    /**
     * @brief getStrValue some as getValue but work with QString
     */
    Q_INVOKABLE QString getStrValue(const QString &key, const QString& def);

    /**
     * @brief sync - save all data on hard disk;
     */
    void sync();

    /**
     * @brief getMode
     * @return
     */
    SettingsSaveMode getMode() const;

    /**
     * @brief setMode
     * @param mode
     */
    void setMode(const SettingsSaveMode &mode);

public slots:
    /**
     * @brief setValue - set new value of key
     * @param key - key pf settings
     * @param value - new value
     */
    void setValue(const QString key, const QVariant& value);

    /**
     * @brief setStrValue - some as setValue< but use QString
     */
    void setStrValue(const QString& key, const QString& value);


signals:
    void valueChanged(QString key, QVariant value);
    void valueStrChanged(QString key, QString value);

};
} ;


#endif // SETTINGS_H
