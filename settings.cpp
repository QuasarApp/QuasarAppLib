/*
 * Copyright (C) 2021-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "settings.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

namespace QuasarAppUtils {

Settings::Settings() {
    auto name = QCoreApplication::applicationName();
    auto company = QCoreApplication::organizationName();
    if (name.isEmpty()) {
        name = "QuasarAppUtils";
    }

    if (company.isEmpty()) {
        company = "QuasarApp";
    }

    _settings = new QSettings(QSettings::NativeFormat, QSettings::Scope::UserScope, company, name);
}

void Settings::syncImplementation() {
    return _settings->sync();
}

QVariant Settings::getValueImplementation(const QString &key, const QVariant &def) {
    return _settings->value(key, def);
}

void Settings::setValueImplementation(const QString key, const QVariant &value) {
    return _settings->setValue(key, value);
}

}
