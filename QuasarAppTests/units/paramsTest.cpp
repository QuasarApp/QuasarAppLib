/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "paramsTest.h"

#include <QStringList>

#include "params.h"

using namespace QuasarAppUtils;

void ParamsTest::test() {
    testVerboseParsing();
    testCustomOptions();
    testMissingValue();
    testDeprecatedOption();
    testRemovedOption();
    testSetGetArg();
    testSetEnable();
    testAvailableArguments();
    testExecutablePath();
    testHelp();
    testClearParsedData();

    // The unit tests of the parseParams method (QCommandLineParser based).
    testParseParamsEmptyInput();
    testParseParamsPositionalArguments();
    testParseParamsDoubleDashSyntax();
    testParseParamsArgcArgvOverloads();
    testParseParamsUnknownOption();
    testParseParamsMissingValue();
    testParseParamsValueLookingLikeOption();
    testParseParamsDuplicateOption();
    testParseParamsAliasNames();
    testParseParamsEndOfOptions();
}

void ParamsTest::testVerboseParsing() {
    QVERIFY(Params::parseParams(QStringList{"-verbose", "3"}));
    QCOMPARE(Params::getArg("verbose"), QString("3"));
    QCOMPARE(Params::getVerboseLvl(), VerboseLvl::Debug);
    QVERIFY(Params::isDebug());
    QCOMPARE(Params::size(), 1);
    QCOMPARE(Params::getUserParamsMap().value("verbose"), QString("3"));

    QVERIFY(Params::parseParams(QStringList{"-verbose", "1"}));
    QCOMPARE(Params::getVerboseLvl(), VerboseLvl::Warning);
    QVERIFY(!Params::isDebug());
}

void ParamsTest::testCustomOptions() {
    OptionsDataList options{{"Test Options",
                             OptionData{{"-myOption"}, "(value)", "Test option"}}};

    QVERIFY(Params::parseParams(QStringList{"-myOption", "hello"}, options));
    QCOMPARE(Params::getArg("myOption"), QString("hello"));
    QVERIFY(Params::isEndable("myOption"));

    // the option name and the value can be joined with the equal sign.
    QVERIFY(Params::parseParams(QStringList{"-myOption=hello2"}, options));
    QCOMPARE(Params::getArg("myOption"), QString("hello2"));
}

void ParamsTest::testMissingValue() {
    // an option without a value at the end of the argument list must fail.
    QVERIFY(!Params::parseParams(QStringList{"-verbose"}));
}

void ParamsTest::testDeprecatedOption() {
    OptionsDataList options{{"Test Options",
                             OptionData{{"-depOption"}, "(value)",
                                        "Deprecated option", "",
                                        "This option is deprecated."}}};

    // a deprecated option is still parsed successful.
    QVERIFY(Params::parseParams(QStringList{"-depOption", "value"}, options));
    QCOMPARE(Params::getArg("depOption"), QString("value"));
}

void ParamsTest::testRemovedOption() {
    OptionsDataList options{{"Test Options",
                             OptionData{{"-removedOption"}, "(value)",
                                        "Removed option", "",
                                        "This option was removed.", true}}};

    // a removed option must make the parse fail.
    QVERIFY(!Params::parseParams(QStringList{"-removedOption", "value"}, options));
}

void ParamsTest::testSetGetArg() {
    Params::setArg("testKey", "testValue");
    QCOMPARE(Params::getArg("testKey"), QString("testValue"));

    // the default value is returned if the key does not exist.
    QCOMPARE(Params::getArg("notExistsKey", "defaultValue"), QString("defaultValue"));
    QCOMPARE(Params::getArg("notExistsKey"), QString(""));
}

void ParamsTest::testSetEnable() {
    Params::setEnable("enableKey", true);
    QVERIFY(Params::isEndable("enableKey"));

    Params::setEnable("enableKey", false);
    QVERIFY(!Params::isEndable("enableKey"));
}

void ParamsTest::testAvailableArguments() {
    auto arguments = Params::availableArguments();
    QVERIFY(arguments.size() > 0);

    QStringList names;
    for (const auto &option : arguments) {
        names.append(option.names());
    }

    QVERIFY(names.contains("-verbose"));
    QVERIFY(names.contains("-fileLog"));
}

void ParamsTest::testExecutablePath() {
    QVERIFY(Params::parseParams(QStringList{"-verbose", "2"}));
    QVERIFY(!Params::getCurrentExecutable().isEmpty());
    QVERIFY(!Params::getCurrentExecutableDir().isEmpty());
}

void ParamsTest::testHelp() {
    QVERIFY(Params::parseParams(QStringList{"-verbose", "2"}));

    // the help must contain the base options group.
    QVERIFY(Params::getHelp().size() > 0);

    // after parsing of user options the input options help section
    // must be available.
    OptionsDataList options{{"Test Options",
                             OptionData{{"-helpOption1"}, "(v)", "Option 1"}},
                            {"Test Options",
                             OptionData{{"-helpOption2"}, "(v)", "Option 2"}}};

    QVERIFY(Params::parseParams(QStringList{"-helpOption1", "1", "-helpOption2", "2"}, options));
    QVERIFY(Params::getHelpOfInputOptions().size() > 0);

    // showHelp must not crash (prints the help to the console).
    Params::showHelp();
}

void ParamsTest::testClearParsedData() {
    Params::setArg("tempKey", "tempValue");
    QVERIFY(Params::size() > 0);

    Params::clearParsedData();
    QCOMPARE(Params::size(), 0);
    QCOMPARE(Params::getArg("tempKey"), QString(""));
}

void ParamsTest::testParseParamsEmptyInput() {
    // an empty parameter list is parsed successful and stores nothing.
    QVERIFY(Params::parseParams(QStringList{}));
    QCOMPARE(Params::size(), 0);
    QVERIFY(Params::getUserParamsMap().isEmpty());
}

void ParamsTest::testParseParamsPositionalArguments() {
    // an argument without the leading dash is stored as a positional
    // argument with the empty value.
    QVERIFY(Params::parseParams(QStringList{"hello"}));
    QVERIFY(Params::isEndable("hello"));
    QCOMPARE(Params::getArg("hello"), QString(""));
    QCOMPARE(Params::size(), 1);

    // the positional arguments can be mixed with the options.
    QVERIFY(Params::parseParams(QStringList{"file.txt", "-verbose", "1"}));
    QCOMPARE(Params::getArg("verbose"), QString("1"));
    QVERIFY(Params::isEndable("file.txt"));
    QCOMPARE(Params::size(), 2);
}

void ParamsTest::testParseParamsDoubleDashSyntax() {
    // the standard double dash (long option) syntax of the
    // QCommandLineParser is supported as well.
    QVERIFY(Params::parseParams(QStringList{"--verbose", "3"}));
    QCOMPARE(Params::getArg("verbose"), QString("3"));

    QVERIFY(Params::parseParams(QStringList{"--verbose=2"}));
    QCOMPARE(Params::getArg("verbose"), QString("2"));
}

void ParamsTest::testParseParamsArgcArgvOverloads() {
    // the const C-style overload reads everything after the executable name.
    const char *constArgv[] = {"test", "-verbose", "3"};
    QVERIFY(Params::parseParams(3, constArgv));
    QCOMPARE(Params::getArg("verbose"), QString("3"));

    // the mutable C-style overload must behave the same way.
    char *argv[] = {const_cast<char *>("test"),
                    const_cast<char *>("-verbose"),
                    const_cast<char *>("2")};
    QVERIFY(Params::parseParams(3, argv));
    QCOMPARE(Params::getArg("verbose"), QString("2"));
}

void ParamsTest::testParseParamsUnknownOption() {
    // the unknown options are rejected by the QCommandLineParser
    // in the single dash and the double dash forms.
    QVERIFY(!Params::parseParams(QStringList{"-unknownOption", "value"}));
    QVERIFY(!Params::parseParams(QStringList{"--unknownOption", "value"}));
}

void ParamsTest::testParseParamsMissingValue() {
    // the base option without a value at the end of the list must fail.
    QVERIFY(!Params::parseParams(QStringList{"-fileLog"}));

    // the user option without a value must fail as well.
    OptionsDataList options{{"Test Options",
                             OptionData{{"-myOption"}, "(value)", "Test option"}}};
    QVERIFY(!Params::parseParams(QStringList{"-myOption"}, options));
}

void ParamsTest::testParseParamsValueLookingLikeOption() {
    // the QCommandLineParser consumes the next argument as the option value
    // as it is, a value can look like an option. The "-verbose" value is a
    // registered option name, so it is mapped to the double dash form by
    // the parser path.
    QVERIFY(Params::parseParams(QStringList{"-fileLog", "-verbose"}));
    QCOMPARE(Params::getArg("fileLog"), QString("--verbose"));

    // the same for the equals form of the option.
    QVERIFY(Params::parseParams(QStringList{"-fileLog=-verbose"}));
    QCOMPARE(Params::getArg("fileLog"), QString("-verbose"));

    // a negative number can be used as an option value.
    QVERIFY(Params::parseParams(QStringList{"-verbose", "-1"}));
    QCOMPARE(Params::getArg("verbose"), QString("-1"));
}

void ParamsTest::testParseParamsDuplicateOption() {
    // an option can be repeated, the last value wins.
    QVERIFY(Params::parseParams(QStringList{"-verbose", "1", "-verbose", "3"}));
    QCOMPARE(Params::getArg("verbose"), QString("3"));
    QCOMPARE(Params::size(), 1);
}

void ParamsTest::testParseParamsAliasNames() {
    // one option can have several names, each name is parsed to
    // its own key in the params map.
    OptionsDataList options{{"Test Options",
                             OptionData{{"-alias1", "-alias2"}, "(value)",
                                        "Option with two names"}}};

    QVERIFY(Params::parseParams(QStringList{"-alias2", "value"}, options));
    QCOMPARE(Params::getArg("alias2"), QString("value"));

    QVERIFY(Params::parseParams(QStringList{"-alias1", "value2"}, options));
    QCOMPARE(Params::getArg("alias1"), QString("value2"));
}

void ParamsTest::testParseParamsEndOfOptions() {
    // the double dash ends the options parsing, everything after it
    // is treated as a positional argument (not converted to the option form).
    QVERIFY(Params::parseParams(QStringList{"first", "--", "-second", "third"}));
    QVERIFY(Params::isEndable("first"));
    QVERIFY(Params::isEndable("-second"));
    QVERIFY(Params::isEndable("third"));
    QCOMPARE(Params::size(), 3);
}
