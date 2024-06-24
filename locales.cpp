/*
 * Copyright (C) 2018-2024 QuasarApp.
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
#include <QLocale>
#include <QMap>

using namespace QuasarAppUtils;

bool QuasarAppUtils::Locales::findQmPrivate(const QString &prefix,
                                            QList<QTranslator*> &qmFiles) {

    for (const auto &location: std::as_const(_locations)) {

        const auto availableFiles = QDir(location).entryInfoList(
            {prefix + ".qm"}, QDir::Files);

        for (const auto &file : availableFiles) {
            auto qmFile = new QTranslator();

            if(!qmFile->load(file.absoluteFilePath())) {
                qWarning() << "Failed to load translation file : "
                                  + file.absoluteFilePath();
                delete qmFile;
                continue;
            }

            if (qmFile->isEmpty()) {
                qDebug() << "Translation file is Empty: " +
                                file.absoluteFilePath();
                delete qmFile;
                continue;
            }

            auto language = qmFile->language();
            if (language.size() && !language.contains(prefix, Qt::CaseInsensitive)) {
                auto message = QString("The target language (%0) and a choosed qm file (%1) "
                                       "is different, Loading will be skiped: ").
                               arg(language, file.absoluteFilePath());
                qDebug() << message;

                delete qmFile;
                continue;
            }

            qmFiles += qmFile;
        }
    }
    return qmFiles.size();
}

bool QuasarAppUtils::Locales::findQm(QString localePrefix,
                                     QList<QTranslator *> &qmFiles) {

    if (localePrefix.size() < 2) {
        if (localePrefix.compare('c', Qt::CaseInsensitive) == 0) {
            return findQmPrivate("en", qmFiles);

        }

        return false;
    } else if (localePrefix.size() >= 4) {
        return findQmPrivate(localePrefix.replace('-', '_'), qmFiles);
    }

    return findQmPrivate(localePrefix, qmFiles);
}

void QuasarAppUtils::Locales::installTranslations( QList<QTranslator *> &qmFiles)
{
    for (const auto & translator: std::as_const(qmFiles)) {
        if (!QCoreApplication::installTranslator(translator)) {

            qWarning() << "Failed to install translation file : " + translator->filePath();
            delete translator;
            // we use a link of qmFiles so remove all invalid translations.
            qmFiles.removeAll(translator);

            continue;
        }
    }
}

QString Locales::translatePrivate(const char *source, const QLocale &locale) {
    auto translations = _translations.value(locale);

    for (const auto& tr : translations) {
        auto result = tr->translate("QuasarAppUtils::Locales", source);
        if (result.size()) {
            return result;
        }
    }

    return source;
}

bool Locales::setLocalePrivate(const QLocale &locale, bool force, bool install) {
    if (force) {
        clearCache(locale);
    }

    removeOldTranslation(_currentLocate);

    // take a link to list of translations.
    QList<QTranslator *> &qmFiles = _translations[locale];

    if (qmFiles.isEmpty()) {
        // fill list of translations
        const auto list = locale.uiLanguages();

        auto it = list.rbegin();
        while (qmFiles.isEmpty() && it != list.rend() && !findQm(*it, qmFiles)) {
            it++;
        }

        if (qmFiles.isEmpty())
            return false;

    }

    if (install)
        installTranslations(qmFiles);

    emit sigTranslationChanged();

    _currentLocate = locale;

    return _translations[locale].size();
}

const QLocale &Locales::currentLocate() {
    auto obj = instance();
    return obj->currentLocatePrivate();
}

QString Locales::tr(const char *source, const QLocale &locale) {
    auto obj = instance();
    return obj->translatePrivate(source, locale);
}

bool Locales::setLocale(const QLocale &locale, bool force) {
    auto obj = instance();
    return obj->setLocalePrivate(locale, force);
}

bool Locales::init(const QList<QLocale> &locales, const QSet<QString> &location) {
    auto obj = instance();
    return obj->initPrivate(locales, location);
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

bool Locales::initPrivate(const QList<QLocale> &locales, const QSet<QString> &locations) {
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    auto defaultTr = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
    auto defaultTr = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#endif
    _locations = locations;
    if (!_locations.contains(defaultTr)) {
        _locations += defaultTr;
    }

    for (const auto& locale: locales) {
        if (!setLocalePrivate(locale, false, false)) {
            return false;
        }
    }

    return true;
}

void Locales::clearCache(const QLocale &locale) {
    for (const auto & tr :std::as_const(_translations[locale])) {
        QCoreApplication::removeTranslator(tr);
        delete tr;
    }

    _translations[locale].clear();
}

void Locales::clearCache() {
    for (auto it = _translations.keyBegin(); it != _translations.keyEnd(); it = std::next(it)) {
        clearCache(*it);
    }
    _translations.clear();
}

Locales *Locales::instance() {
    static auto instance = new Locales();
    return instance;
}

void Locales::removeOldTranslation(const QLocale &locale) {
    for (const auto & tr :std::as_const(_translations[locale])) {
        QCoreApplication::removeTranslator(tr);
    }
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
    clearCache();
}
