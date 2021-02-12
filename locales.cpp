/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "locales.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include <QRegularExpression>
#include "params.h"

using namespace QuasarAppUtils;

bool Locales::setLocalePrivate(const QLocale &locale) {
    auto translator = getTranslator();

    const auto qmFiles = QDir(_location).entryInfoList({"*" + locale.bcp47Name() + "*.qm"}, QDir::Files);

    for (const auto & file: qmFiles) {
        if(!translator->load(file.absoluteFilePath())) {
            return false;
        }
    }

    emit sigTranslationChanged();

    return true;
}

bool Locales::setLocale(const QLocale &locale) {
    auto obj = instance();
    return obj->setLocalePrivate(locale);
}

bool Locales::init(const QLocale &locale, QString location) {
    auto obj = instance();
    return obj->initPrivate(locale, location);
}

bool Locales::initPrivate(const QLocale &locale, QString location) {
    auto translator = getTranslator();

    if (location.isEmpty())
        _location = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    else
        _location = location;

    if (!setLocale(locale)) {
        return false;
    }

    QCoreApplication::installTranslator(translator);

    return true;
}

Locales *Locales::instance() {
    static auto instance = new Locales();
    return instance;
}

QTranslator *Locales::getTranslator() {
    static QTranslator *translator = new QTranslator();
    return translator;
}
