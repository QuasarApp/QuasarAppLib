/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "locales.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include "params.h"

using namespace QuasarAppUtils;

bool Locales::setLocale(const QLocale &locale, const QString& file, const QString& delimiter, const QString& location) {
    auto obj = instance();
    return obj->translate(locale, file, delimiter, location);
}

bool Locales::translate(const QLocale &locale, const QString &file, const QString &delimiter, const QString &location) {
    auto translator = getTranslator();

    if(translator->load(locale, file, delimiter, location) && QCoreApplication::installTranslator(translator)) {
        emit sigTranslationChanged();
        return true;
    }

    QLocale defaultLocale = QLocale::system();
    if(translator->load(defaultLocale, file, delimiter, location) && QCoreApplication::installTranslator(translator)) {
        emit sigTranslationChanged();
        return true;
    }

    Params::log("set translations fail!", VerboseLvl::Warning);

    return false;
}

Locales *Locales::instance() {
    static auto instance = new Locales();
    return instance;
}

QTranslator *Locales::getTranslator() {
    static QTranslator *translator = new QTranslator();
    return translator;
}
