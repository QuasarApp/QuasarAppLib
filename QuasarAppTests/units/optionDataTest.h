/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef OPTIONDATATEST_H
#define OPTIONDATATEST_H

#include "test.h"

#include <QtTest>

/**
 * @brief The OptionDataTest class test the QuasarAppUtils::OptionData class
 *  (option data storage, validity, deprecated and removed states and the
 *  help line generation).
 */
class OptionDataTest: public Test
{
public:
    OptionDataTest() = default;
    ~OptionDataTest() override = default;

    void test() override;

protected:
    void testAccessors();
    void testValidity();
    void testDeprecatedAndRemoved();
    void testToHelp();

};

#endif // OPTIONDATATEST_H
