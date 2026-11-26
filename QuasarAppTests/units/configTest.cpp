/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "configTest.h"

#include "OptionData/optiondata.h"
#include "Params/params.h"
#include "Settings/config.h"
#include "Settings/settings.h"

#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QSignalSpy>

using namespace QuasarAppUtils;

namespace {
/**
 * @brief configOptions This function returns the list of the test
 *  options for the Config constructor.
 */
OptionsDataList configOptions() {
    OptionsDataList options;
    options.insert("Config Test Options",
                   OptionData{{"-testValue"}, "(value)", "The test value option"});
    options.insert("Config Test Options",
                   OptionData{{"-testInt"}, "(value)", "The test int option"});
    options.insert("Config Test Options",
                   OptionData{{"-testBool"}, "(value)", "The test bool option"});
    options.insert("Config Test Options",
                   OptionData{{"-testDouble"}, "(value)", "The test double option"});
    return options;
}

/**
 * @brief The TestConfig class is a class exclusively for the testing
 *  purposes. It is a subclass of the Config class that publicly
 *  exposes the protected QSettings backend of the Settings base class
 *  (the Settings::getSettings method is not a part of the public API),
 *  so the tests can check the values that are stored in the settings
 *  file directly.
 */
class TestConfig: public Config
{
public:
    TestConfig(int argc, const char* argv[], const OptionsDataList& options = {})
        : Config(argc, argv, options) {}

    QSettings* settings() { return getSettings(); }
};
}

ConfigTest::ConfigTest() {
    // remove the settings file of a previous test run.
    QSettings settings(QSettings::IniFormat, QSettings::Scope::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    QFile::remove(settings.fileName());
}

ConfigTest::~ConfigTest() {
    // remove the settings file that was created by the tests.
    QSettings settings(QSettings::IniFormat, QSettings::Scope::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    QFile::remove(settings.fileName());
}

void ConfigTest::test() {
    testConstructor();
    testCommandLineParsing();
    testPriorityOrder();
    testConfigFileValues();
    testDefaultValues();
    testTypeConversion();
    testSetValue();
    testSettingsInheritance();
}

void ConfigTest::testConstructor() {
    // The Config object with the empty command line.
    {
        const char* argv[] = {"test"};
        Config config(1, argv);
        QVERIFY(config.commandLineArgs().isEmpty());
    }

    // The Config object with the command-line arguments. The first
    // argument (the executable name) is skipped by the Params class.
    {
        const char* argv[] = {"test", "-testValue", "cliValue"};
        Config config(3, argv, configOptions());
        QVERIFY(config.commandLineArgs().contains("testValue"));
        QCOMPARE(config.commandLineArgs().value("testValue"), QString("cliValue"));
    }
}

void ConfigTest::testCommandLineParsing() {
    const char* argv[] = {"test", "-testValue", "cliValue", "-testInt", "42"};
    Config config(5, argv, configOptions());

    // the command line is parsed with the Params class, so the parsed
    // arguments are also available through the Params accessors.
    QCOMPARE(Params::getArg("testValue"), QString("cliValue"));
    QCOMPARE(Params::getArg("testInt"), QString("42"));

    QCOMPARE(config.getValue("testValue"), QVariant("cliValue"));
    QCOMPARE(config.getValue("testInt"), QVariant("42"));

    // the option name and the value can be joined with the equal sign.
    const char* argv2[] = {"test", "-testValue=eqValue"};
    Config config2(2, argv2, configOptions());
    QCOMPARE(config2.getValue("testValue"), QVariant("eqValue"));
}

void ConfigTest::testPriorityOrder() {
    // 1. The command-line value has the highest priority.
    {
        const char* argv[] = {"test", "-testValue", "cliValue"};
        Config config(3, argv, configOptions());
        QCOMPARE(config.getValue("testValue"), QVariant("cliValue"));
    }

    // 2. A value saved in the config file doesn't override the
    //    command-line value, but it is used if the command line
    //    doesn't provide the value.
    {
        const char* argv[] = {"test"};
        Config config(1, argv);
        config.setValue("testValue", QString("fileValue"));
        config.sync();
    }
    {
        const char* argv[] = {"test", "-testValue", "cliValue2"};
        Config config(3, argv, configOptions());
        QCOMPARE(config.getValue("testValue"), QVariant("cliValue2"));
    }
    {
        const char* argv[] = {"test"};
        Config config(1, argv);
        QCOMPARE(config.getValue("testValue"), QVariant("fileValue"));

        // 3. The default value (the def argument) has the lowest
        //    priority: it is used only if the key is not set on the
        //    command line and in the config file.
        QCOMPARE(config.getValue("testValue", QVariant("ignoredDefault")),
                 QVariant("fileValue"));
        QCOMPARE(config.getValue("absentKey", QVariant("fallback")),
                 QVariant("fallback"));
    }
}

void ConfigTest::testConfigFileValues() {
    {
        const char* argv[] = {"test"};
        Config config(1, argv);
        config.setValue("fileOnlyKey", QString("fileValue"));
        config.sync();
    }

    // the value saved in the config file is returned by a new
    // Config object that doesn't have the value on the command line.
    {
        const char* argv[] = {"test"};
        TestConfig config(1, argv);
        QCOMPARE(config.getValue("fileOnlyKey"), QVariant("fileValue"));
        QCOMPARE(config.getStrValue("fileOnlyKey"), QString("fileValue"));
        QVERIFY(config.settings()->contains("fileOnlyKey"));
    }
}

void ConfigTest::testDefaultValues() {
    const char* argv[] = {"test"};
    Config config(1, argv);

    // the def argument of the getValue method is the default value
    // of the setting if the key is not set on the command line
    // and in the config file.
    QCOMPARE(config.getValue("defaultOnlyKey", QVariant("fallback")),
             QVariant("fallback"));
    QCOMPARE(config.getStrValue("defaultOnlyKey2", "strFallback"),
             QString("strFallback"));

    // the empty def argument returns the empty value (the default
    // settings map of the Settings base class is empty).
    QCOMPARE(config.getValue("defaultOnlyKey3").toString(), QString(""));
}

void ConfigTest::testTypeConversion() {
    const char* argv[] = {"test", "-testInt", "42", "-testBool", "true",
                          "-testDouble", "3.14"};
    Config config(7, argv, configOptions());

    QCOMPARE(config.getValue("testInt").toInt(), 42);
    QCOMPARE(config.getValue("testBool").toBool(), true);
    QCOMPARE(config.getValue("testDouble").toDouble(), 3.14);
}

void ConfigTest::testSetValue() {
    const char* argv[] = {"test"};
    Config config(1, argv);

    config.setValue("setKey", QVariant("testValue"));
    QCOMPARE(config.getValue("setKey"), QVariant("testValue"));
    QCOMPARE(config.getStrValue("setKey"), QString("testValue"));

    config.setValue("setKey", 42);
    QCOMPARE(config.getValue("setKey").toInt(), 42);
}

void ConfigTest::testSettingsInheritance() {
    const char* argv[] = {"test"};
    TestConfig config(1, argv);

    // the Config object is a Settings object and an ISettings object.
    Settings* base = &config;
    QVERIFY(base == static_cast<Settings*>(&config));
    ISettings* iface = &config;
    QVERIFY(iface == static_cast<ISettings*>(&config));

    // the Settings base class backend is available.
    QVERIFY(config.settings() != nullptr);

    // the ISettings signals work through the Config object. The
    // signals are resolved on the runtime metaobject (see the
    // settingsTest for the explanation).
    const QMetaObject* mo = config.metaObject();
    const int valueIndex = mo->indexOfSignal("valueChanged(QString,QVariant)");
    const int strIndex = mo->indexOfSignal("valueStrChanged(QString,QString)");
    QVERIFY(valueIndex >= 0);
    QVERIFY(strIndex >= 0);

    QSignalSpy valueSpy(&config, mo->method(valueIndex));
    QSignalSpy strSpy(&config, mo->method(strIndex));

    config.setValue("signalKey", "signalValue");
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(valueSpy.at(0).at(0).toString(), QString("signalKey"));
    QCOMPARE(valueSpy.at(0).at(1).toString(), QString("signalValue"));
    QCOMPARE(strSpy.count(), 1);
    QCOMPARE(strSpy.at(0).at(0).toString(), QString("signalKey"));
    QCOMPARE(strSpy.at(0).at(1).toString(), QString("signalValue"));

    // the Settings base class methods work through the Config object.
    config.setBoolOptions({"boolOpt"});
    QVERIFY(config.boolOptions().contains("boolOpt"));
}
