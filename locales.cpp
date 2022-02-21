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
#include <QLibraryInfo>
#include "params.h"

using namespace QuasarAppUtils;

bool Locales::setLocalePrivate(const QLocale &locale) {
    removeOldTranslation();
    QFileInfoList qmFiles;

    for (const auto &location: qAsConst(_locations)) {
        qmFiles += QDir(location).entryInfoList({"*" + locale.bcp47Name() + "*.qm"}, QDir::Files);
    }

    for (const auto & file: qAsConst(qmFiles)) {
        auto translator = new QTranslator();

        if(!(translator->load(file.absoluteFilePath()) && QCoreApplication::installTranslator(translator))) {
            delete translator;
            continue;
        }

        _translations.push_back(translator);

    }
    emit sigTranslationChanged();

    _currentLocate = locale;

    return _translations.size();
}

const QLocale &Locales::currentLocate() {
    auto obj = instance();
    return obj->currentLocatePrivate();
}

bool Locales::setLocale(const QLocale &locale) {
    auto obj = instance();
    return obj->setLocalePrivate(locale);
}

bool Locales::init(const QLocale &locale, const QSet<QString> & location) {
    auto obj = instance();
    return obj->initPrivate(locale, location);
}

bool Locales::initPrivate(const QLocale &locale, const QSet<QString> & locations) {

#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    auto defaultTr = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
    auto defaultTr = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#endif
    _locations = locations;
    if (!_locations.contains(defaultTr)) {
        _locations += defaultTr;
    }

    return setLocalePrivate(locale);
}

Locales *Locales::instance() {
    static auto instance = new Locales();
    return instance;
}

void Locales::removeOldTranslation() {
    for (const auto & tr :qAsConst(_translations)) {
        QCoreApplication::removeTranslator(tr);
        delete tr;
    }
    _translations.clear();
}

void Locales::addLocationPrivate(const QString &location) {
    _locations += location;
}

const QLocale &Locales::currentLocatePrivate() const {
    return _currentLocate;

}
void Locales::addLocation(const QString &location) {
    auto obj = instance();
    obj->addLocationPrivate(location);
}

Locales::~Locales() {
    removeOldTranslation();
}
