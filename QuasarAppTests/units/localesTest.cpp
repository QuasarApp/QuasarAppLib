/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "localesTest.h"

#include <QLocale>

#include "locales.h"

#ifndef QA_TEST_LANGUAGES_DIR
#define QA_TEST_LANGUAGES_DIR ""
#endif

using namespace QuasarAppUtils;

void LocalesTest::test() {
    testInitWithTranslations();
    testLocaleSwitching();
    testTr();
}

void LocalesTest::testInitWithTranslations() {
    // the test translation file is generated in the build directory by
    // the CMake build step (see the CMakeLists.txt of the tests).
    QSet<QString> locations{QString::fromUtf8(QA_TEST_LANGUAGES_DIR)};
    QVERIFY(Locales::init(QLocale(QLocale::English, QLocale::AnyCountry), locations));

    QVERIFY(Locales::instance() != nullptr);
    QCOMPARE(Locales::currentLocate(), QLocale(QLocale::English, QLocale::AnyCountry));
}

void LocalesTest::testLocaleSwitching() {
    // the force flag reloads the translations of the same locale.
    QVERIFY(Locales::setLocale(QLocale(QLocale::English, QLocale::AnyCountry), true));
    QCOMPARE(Locales::currentLocate(), QLocale(QLocale::English, QLocale::AnyCountry));

    // a locale without an available translation file must return false.
    QVERIFY(!Locales::setLocale(QLocale(QLocale::German, QLocale::Germany)));
}

void LocalesTest::testTr() {
    // the test qm file contains one translation for the
    // "QuasarAppUtils::Locales" context.
    QCOMPARE(Locales::tr("Test translation source", QLocale(QLocale::English, QLocale::AnyCountry)),
             QString("Test translation result"));

    // a locale without translations must return the source string.
    QCOMPARE(Locales::tr("Test translation source", QLocale(QLocale::French, QLocale::France)),
             QString("Test translation source"));
}
