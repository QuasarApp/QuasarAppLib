/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "Settings/settings.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

namespace QuasarAppUtils {

Settings::Settings(QSettings::Format format) {
    auto name = QCoreApplication::applicationName();
    auto company = QCoreApplication::organizationName();
    if (name.isEmpty()) {
        name = "QuasarAppUtils";
    }

    if (company.isEmpty()) {
        company = "QuasarApp";
    }

    _settings = new QSettings(format, QSettings::Scope::UserScope, company, name);
}

void Settings::syncImplementation() {
    return _settings->sync();
}

QVariant Settings::getValueImplementation(const QString &key, const QVariant &def) {
    if (isBool(key)) {
        return _settings->value(key, def).toBool();
    }

    return _settings->value(key, def);
}

void Settings::setValueImplementation(const QString key, const QVariant &value) {
    return _settings->setValue(key, value);
}

void Settings::setValue(const QString &key, const QVariant &value) {
    if (_mode == SettingsSaveMode::Auto) {
        _settings->setValue(key, value);
    } else {
        _cache[key] = value;
    }
    emit valueChanged(key, value);
}

QVariant Settings::getValue(const QString &key, const QVariant& def) {
    if (_mode == SettingsSaveMode::Auto) {
        if (isBool(key)) {
            return _settings->value(key, def).toBool();
        }
        return _settings->value(key, def);
    } else {
        if (isBool(key)) {
            return _cache.value(key).toBool();
        }
        return _cache.value(key);
    }
}

void Settings::setStrValue(const QString& key, const QString& value) {
    setValue(key, value);
}

bool Settings::ignoreToRest(const QString &) const {
    return false;
}

QHash<QString, QVariant> Settings::defaultSettings() {
    return {};
}

bool Settings::isBool(const QString &) const {
    return false;
}

const QSet<QString> &Settings::boolOptions() const
{
    return _boolOptions;
}

void Settings::setBoolOptions(const QSet<QString> &newBoolOptions)
{
    _boolOptions = newBoolOptions;
}

bool Settings::initService() {
    return ISettings::initService(std::make_unique<Settings>());
}

ISettings *Settings::autoInstance() {
    if (auto result = instance()) {
        return result;
    }

    Settings::initService();
    return ISettings::instance();
}

}
