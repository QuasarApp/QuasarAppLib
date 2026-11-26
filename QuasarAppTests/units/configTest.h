/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#ifndef CONFIGTEST_H
#define CONFIGTEST_H

#include "test.h"

#include <QtTest>

namespace QuasarAppUtils {
class Config;
}

/**
 * @brief The ConfigTest class tests the QuasarAppUtils::Config class:
 *  the command-line parsing through the Params class, the Settings
 *  base class functionality, and the value priority resolution
 *  (command line > config file > default value).
 */
class ConfigTest: public Test
{
public:
    ConfigTest();
    ~ConfigTest() override;

    void test() override;

protected:
    void testConstructor();
    void testCommandLineParsing();
    void testPriorityOrder();
    void testConfigFileValues();
    void testDefaultValues();
    void testTypeConversion();
    void testSetValue();
    void testSettingsInheritance();
};

#endif // CONFIGTEST_H