/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SETTINGSTEST_H
#define SETTINGSTEST_H

#include "test.h"

#include <QtTest>

namespace QuasarAppUtils {
class Settings;
}

/**
 * @brief The SettingsTest class test the QuasarAppUtils::Settings and
 *  QuasarAppUtils::ISettings classes (service initialisation, value
 *  storage, the valueChanged signals, the save modes, the default
 *  values and the settings listeners).
 */
class SettingsTest: public Test
{
public:
    SettingsTest();
    ~SettingsTest() override;

    void test() override;

protected:
    void testInitService();
    void testValueStorage();
    void testSignals();
    void testSaveModes();
    void testDefaultValues();
    void testBoolOptions();
    void testSettingsListener();
    void testDeinitService();

private:
    /// @brief _testSettings This is the pointer to the test settings service object.
    QuasarAppUtils::Settings *_testSettings = nullptr;

};

#endif // SETTINGSTEST_H
