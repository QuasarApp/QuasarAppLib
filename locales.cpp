/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "locales.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QRegularExpression>
#include "params.h"
#include "qdebug.h"

using namespace QuasarAppUtils;

bool QuasarAppUtils::Locales::findQm(const QString& localePrefix,
                                     QFileInfoList &qmFiles) {

    if (localePrefix.isEmpty())
        return false;

    qDebug() << "Search for " << localePrefix;

    const auto prefixes = localePrefix.split(QRegExp("[_-]"));

    auto prefixIt = prefixes.begin();

    for (const auto &location: qAsConst(_locations)) {
        qmFiles += QDir(location).entryInfoList({"*" + (*prefixIt).toLower() + "*.qm"}, QDir::Files);
    }

    qDebug() << "Found" << qmFiles;

    return qmFiles.size();
}

bool Locales::setLocalePrivate(const QLocale &locale) {
    removeOldTranslation();

    const auto list = locale.uiLanguages();
    QFileInfoList qmFiles;

    auto it = list.begin();
    while (it != list.end() && !findQm(*it, qmFiles)) {
        it++;
    }

    if (qmFiles.isEmpty())
        return false;

    for (const auto & file: qAsConst(qmFiles)) {
        auto translator = new QTranslator();

        if(!translator->load(file.absoluteFilePath())) {
            QuasarAppUtils::Params::log("Failed to load translation file : " + file.absoluteFilePath(),
                                        QuasarAppUtils::Warning);
            delete translator;
            continue;
        }

        if (!QCoreApplication::installTranslator(translator)) {

            QuasarAppUtils::Params::log("Failed to install translation file : " + translator->filePath(),
                                        QuasarAppUtils::Warning);

            delete translator;
            continue;
        }

        _translations.push_back(translator);

    }
    emit sigTranslationChanged();

    _currentLocate = locale;

    return _translations.size();
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

bool Locales::setLocale(const QLocale &locale) {
    return setLocalePrivate(locale);
}

bool Locales::init(const QLocale &locale, const QSet<QString> &location) {
    return initPrivate(locale, location);
}

void Locales::addLocation(const QString &location) {
    return addLocationPrivate(location);
}

const QLocale &Locales::currentLocate() {
    return currentLocatePrivate();
}


Locales::~Locales() {
    removeOldTranslation();
}

void LocalesService::addLocation(const QString &location) {
    auto obj = instance();
    obj->addLocation(location);
}

const QLocale &LocalesService::currentLocate() {
    auto obj = instance();
    return obj->currentLocate();
}

bool LocalesService::setLocale(const QLocale &locale) {
    auto obj = instance();
    return obj->setLocale(locale);
}

bool LocalesService::init(const QLocale &locale, const QSet<QString> & location) {
    auto obj = instance();
    return obj->init(locale, location);
}
