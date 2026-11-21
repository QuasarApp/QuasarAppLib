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
