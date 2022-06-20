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

ISettings::ISettings(SettingsSaveMode mode) {
    _mode = mode;
}

void ISettings::clearCache() {
    _cache.clear();
}

SettingsSaveMode ISettings::getMode() const {
    return _mode;
}

void ISettings::setMode(const SettingsSaveMode &mode) {
    _mode = mode;
}

QVariant ISettings::getValue(const QString &key, const QVariant &def) {

    if (!_cache.contains(key)) {
        _cache[key] = getValueImplementation(key, def);
    }

    auto result = _cache.value(key, def);

    if (result.isNull()) {
        return def;
    }

    return result;
}

QString ISettings::getStrValue(const QString &key, const QString &def) {
    return getValue(key, QVariant(def)).toString();
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
