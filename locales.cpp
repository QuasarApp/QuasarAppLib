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

using namespace QuasarAppUtils;

bool QuasarAppUtils::Locales::findQmPrivate(const QString &prefix,
                                            QList<QTranslator*> &qmFiles) {

    for (const auto &location: qAsConst(_locations)) {

        const auto availableFiles = QDir(location).entryInfoList(
            {"*" + prefix + "*.qm"}, QDir::Files);

        for (const auto &file : availableFiles) {
            auto qmFile = new QTranslator();

            if(!qmFile->load(file.absoluteFilePath())) {
                QuasarAppUtils::Params::log("Failed to load translation file : "
                                                + file.absoluteFilePath(),
                                            QuasarAppUtils::Warning);
                delete qmFile;
                continue;
            }

            if (qmFile->isEmpty()) {
                QuasarAppUtils::Params::log("Translation file is Empty: " +
                                                file.absoluteFilePath(),
                                            QuasarAppUtils::Debug);
                delete qmFile;
                continue;
            }

            auto language = qmFile->language();
            if (language.size() && !language.contains(prefix, Qt::CaseInsensitive)) {
                auto message = QString("The target language (%0) and a choosed qm file (%1) "
                                       "is different, Loading will be skiped: ").
                               arg(language, file.absoluteFilePath());
                QuasarAppUtils::Params::log(message,  QuasarAppUtils::Debug);

                delete qmFile;
                continue;
            }

            qmFiles += qmFile;
        }
    }
    return qmFiles.size();
}

bool QuasarAppUtils::Locales::findQm(const QString &localePrefix,
                                     QList<QTranslator *> &qmFiles) {

    if (localePrefix.size() < 2) {
        if (localePrefix.compare('c', Qt::CaseInsensitive) == 0) {
            return findQmPrivate("en", qmFiles);

        }

        return false;
    }

    static QRegularExpression regexp("[_-]");
    const auto prefixes = localePrefix.split(regexp);

    auto prefixIt = prefixes.begin();
    return findQmPrivate((*prefixIt).toLower(), qmFiles);
}

bool Locales::setLocalePrivate(const QLocale &locale) {
    removeOldTranslation();

    const auto list = locale.uiLanguages();
    QList<QTranslator *> qmFiles;

    auto it = list.begin();
    while (it != list.end() && !findQm(*it, qmFiles)) {
        it++;
    }

    if (qmFiles.isEmpty())
        return false;

    for (const auto & translator: qAsConst(qmFiles)) {
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
