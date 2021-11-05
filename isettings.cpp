/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isettings.h"
#include <QSettings>
#include <QCoreApplication>

using namespace QuasarAppUtils;

ISettings::ISettings(SettingsSaveMode mode) {
    _mode = mode;
}

SettingsSaveMode ISettings::getMode() const {
    return _mode;
}

void ISettings::setMode(const SettingsSaveMode &mode) {
    _mode = mode;
}

//ISettings *ISettings::initSettings(SettingsSaveMode mode) {
//    static ISettings* res = new ISettings(mode);
//    return res;
//}

//ISettings *ISettings::instance() {
//    return initSettings();
//}

QVariant ISettings::getValue(const QString &key, const QVariant &def) {
    getValueImplementation(key, def);
}

QString ISettings::getStrValue(const QString &key, const QString &def) {
    return getValue(key, QVariant(def)).toString();
}

void ISettings::sync() {
    syncImplementation();
}

void ISettings::setValue(const QString key, const QVariant &value) {
    setValueImplementation(key, value);

    emit valueChanged(key, value);
    emit valueStrChanged(key, value.toString());

    if (_mode == SettingsSaveMode::Auto) {
        sync();
    }

}

void ISettings::setStrValue(const QString &key, const QString &value) {
    setValue(key, value);
}
