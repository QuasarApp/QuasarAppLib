/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PARAMSTEST_H
#define PARAMSTEST_H

#include "test.h"

#include <QtTest>

/**
 * @brief The ParamsTest class test the QuasarAppUtils::Params class
 *  (command line parameters parsing, the verbose level, the option
 *  storage and the help generation).
 */
class ParamsTest: public Test
{
public:
    ParamsTest() = default;
    ~ParamsTest() override = default;

    void test() override;

protected:
    void testVerboseParsing();
    void testCustomOptions();
    void testMissingValue();
    void testDeprecatedOption();
    void testRemovedOption();
    void testSetGetArg();
    void testSetEnable();
    void testAvailableArguments();
    void testExecutablePath();
    void testHelp();
    void testClearParsedData();

    // The unit tests of the parseParams method (QCommandLineParser based).
    void testParseParamsEmptyInput();
    void testParseParamsPositionalArguments();
    void testParseParamsDoubleDashSyntax();
    void testParseParamsArgcArgvOverloads();
    void testParseParamsUnknownOption();
    void testParseParamsMissingValue();
    void testParseParamsValueLookingLikeOption();
    void testParseParamsDuplicateOption();
    void testParseParamsAliasNames();
    void testParseParamsEndOfOptions();

};

#endif // PARAMSTEST_H
