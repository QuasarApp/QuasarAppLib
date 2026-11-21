/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef LOCALESTEST_H
#define LOCALESTEST_H

#include "test.h"

#include <QtTest>

/**
 * @brief The LocalesTest class test the QuasarAppUtils::Locales class
 *  (translations loading, the locale switching and the tr method).
 * The test translation file (en.qm) is generated in the build directory
 * by the CMake build step from the en.ts source file.
 */
class LocalesTest: public Test
{
public:
    LocalesTest() = default;
    ~LocalesTest() override = default;

    void test() override;

protected:
    void testInitWithTranslations();
    void testLocaleSwitching();
    void testTr();

};

#endif // LOCALESTEST_H