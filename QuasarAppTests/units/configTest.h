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
 * @brief The ConfigTest class tests the QuasarAppUtils::Config class
 *  (command-line parameter parsing, settings functionality, value
 *  priority resolution, and help generation).
 */
class ConfigTest: public Test
{
public:
    ConfigTest();
    ~ConfigTest() override = default;

    void test() override;

protected:
    void testConstructor();
    void testPriorityOrder();
    void testCommandlineValues();
    void testConfigFileValues();
    void testDefaultValues();
    void testTypeConversion();
    void testValueSource();
    void testHasCommandlineValue();
    void testHasConfigFileValue();
    void testHasDefaultValue();
    void testIsCommandlineValue();
    void testIsConfigFileValue();
    void testIsDefaultValue();
    void testSetValue();
    void testGetParamsMap();
    void testSupportedParams();
    void testHelpText();
    void testDefaultValueStorage();
};

#endif // CONFIGTEST_H