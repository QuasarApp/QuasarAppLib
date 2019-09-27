/*
 * Copyright (C) 2018-2019 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "settings.h"
#include <QSettings>
#include <QCoreApplication>

using namespace QuasarAppUtils;

Settings::Settings() {
    auto name = QCoreApplication::applicationName();
    auto company = QCoreApplication::organizationName();
    if (name.isEmpty()) {
        name = "QuasarAppUtils";
    }

    if (company.isEmpty()) {
        company = "QuasarApp";
    }

    _settings = new QSettings(QSettings::IniFormat, QSettings::Scope::UserScope, company, name);

}

Settings *Settings::initSettings() {
    static Settings* res = new Settings();
    return res;
}

Settings *Settings::get() {
    return initSettings();
}

const Settings *Settings::getConst() {
    return initSettings();
}

QVariant Settings::getValue(const QString &key, const QVariant &def) {
    return _settings->value(key, def);
}

QString Settings::getStrValue(const QString &key, const QString &def) {
    return getValue(key, QVariant(def)).toString();
}

void Settings::setValue(const QString key, const QVariant &value) {
    _settings->setValue(key, value);

    emit valueChanged(key, value);
    emit valueStrChanged(key, value.toString());

}

void Settings::setStrValue(const QString &key, const QString &value) {
    setValue(key, value);
}
