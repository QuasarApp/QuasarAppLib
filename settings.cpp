/*
 * Copyright (C) 2021-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "settings.h"
#include <QSettings>
#include <QCoreApplication>

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

    _settings = new QSettings(QSettings::IniFormat, QSettings::Scope::UserScope, company, name);
}

void Settings::syncImplementation() {
    return _settings->sync();
}

QVariant Settings::getValueImplementation(const QString &key, const QVariant &def) {

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    switch (def.type()) {
#else
    switch (def.type()) {
#endif
    case QVariant::Type::Bool: {
        return _settings->value(key, def).toBool();
    }

    case QVariant::Type::ULongLong: {
        return _settings->value(key, def).toULongLong();
    }

    case QVariant::Type::LongLong: {
        return _settings->value(key, def).toLongLong();
    }

    case QVariant::Type::UInt: {
        return _settings->value(key, def).toUInt();
    }

    case QVariant::Type::Int: {
        return _settings->value(key, def).toInt();
    }

    default:
        return _settings->value(key, def);

    }

}

void Settings::setValueImplementation(const QString key, const QVariant &value) {
    if (value.type() == QVariant::Bool) {
        _settings->setValue(key, value.toInt());
    }
    return _settings->setValue(key, value);
}

}
