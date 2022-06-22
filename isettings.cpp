/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isettings.h"
#include <QSettings>
#include <QCoreApplication>

namespace QuasarAppUtils {

ISettings* ISettings::_settings = nullptr;

ISettings::ISettings(SettingsSaveMode mode) {
    _mode = mode;
}

ISettings::~ISettings() {
    if (_defaultConfig)
        delete _defaultConfig;
}

void ISettings::clearCache() {
    _cache.clear();
}

QHash<QString, QVariant> &ISettings::settingsMap() {
    if (!_defaultConfig)
        _defaultConfig = new QHash<QString, QVariant>(defaultSettings());

    return *_defaultConfig;
}

SettingsSaveMode ISettings::getMode() const {
    return _mode;
}

void ISettings::setMode(const SettingsSaveMode &mode) {
    _mode = mode;
}

ISettings *ISettings::instance(){
    return _settings;
}

QVariant ISettings::getValue(const QString &key, const QVariant &def) {

    if (!_cache.contains(key)) {
        _cache[key] = getValueImplementation(key, def);
    }

    QVariant defVal = def;
    if (defVal.isNull()) {
        defVal = settingsMap().value(key);
    }

    auto result = _cache.value(key, defVal);

    if (result.isNull()) {
        return defVal;
    }

    return result;
}

QString ISettings::getStrValue(const QString &key, const QString &def) {
    if (def.isEmpty()) {
        return getValue(key).toString();

    }

    return getValue(key, QVariant(def)).toString();
}

void ISettings::resetToDefault() {

    for (auto it = _defaultConfig->begin(); it != _defaultConfig->end(); ++it) {
        setValue(it.key(), settingsMap().value(it.key()));
    }
}

void ISettings::sync() {
    for (auto it = _cache.begin(); it != _cache.end(); ++it) {
        setValueImplementation(it.key(), it.value());
    }

    return syncImplementation();
}

void ISettings::forceReloadCache() {
    for (auto it = _cache.begin(); it != _cache.end(); ++it) {
        setValue(it.key(), getValueImplementation(it.key(), it.value()));
    }
}

void ISettings::setValue(const QString key, const QVariant &value) {

    if (_cache.contains(key) && _cache.value(key) == value) {
        return;
    }

    _cache[key] = value;

    emit valueChanged(key, value);
    emit valueStrChanged(key, value.toString());

    if (_mode == SettingsSaveMode::Auto) {
        sync();
    }

}

void ISettings::setStrValue(const QString &key, const QString &value) {
    setValue(key, value);
}

}
