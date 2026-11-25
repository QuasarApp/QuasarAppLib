/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "configTest.h"

#include "Config/config.h"

#include <QCommandLineOption>
#include <QHash>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

using namespace QuasarAppUtils;

void ConfigTest::test()
{
    testConstructor();
    testPriorityOrder();
    testCommandlineValues();
    testConfigFileValues();
    testDefaultValues();
    testTypeConversion();
    testValueSource();
    testHasCommandlineValue();
    testHasConfigFileValue();
    testHasDefaultValue();
    testIsCommandlineValue();
    testIsConfigFileValue();
    testIsDefaultValue();
    testSetValue();
    testGetParamsMap();
    testSupportedParams();
    testHelpText();
    testDefaultValueStorage();
}

void ConfigTest::testConstructor()
{
    // The Config object with the empty parameters list
    QList<QCommandLineOption> emptyParams;
    Config config(0, nullptr, emptyParams);
    QVERIFY(config.isValid());

    // The Config object with the parameters list
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"verbose", "Enable verbose logging", "level", "0"};
    params << QCommandLineOption{"output", "Output file", "path", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config2(argc, argv, params);
    QVERIFY(config2.isValid());
}

void ConfigTest::testPriorityOrder()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"value", "Test value", "val", "default"};

    // The value is provided on the command line
    const int argc = 3;
    const char* argv[] = {"test", "--value", "commandlineValue"};

    Config config(argc, argv, params);

    // The command-line value has the highest priority
    QCOMPARE(config.value("value"), QVariant("commandlineValue"));
    QVERIFY(config.isCommandlineValue("value"));
    QVERIFY(config.hasCommandlineValue("value"));

    // A value saved in the config file doesn't override the command-line value
    config.setValue("value", QString("configValue"));
    QCOMPARE(config.getConfigFileValue("value"), QString("configValue"));
    QVERIFY(config.hasConfigFileValue("value"));
    QCOMPARE(config.value("value"), QVariant("commandlineValue"));
    QVERIFY(config.isCommandlineValue("value"));

    // Without the command-line value the config file value
    // wins over the default value
    const int argc2 = 1;
    const char* argv2[] = {"test"};

    Config config2(argc2, argv2, params);
    QCOMPARE(config2.value("value"), QVariant("configValue"));
    QVERIFY(config2.isConfigFileValue("value"));
}

void ConfigTest::testCommandlineValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"option", "Test option", "opt", "default"};

    const int argc = 3;
    const char* argv[] = {"test", "--option", "cliValue"};

    Config config(argc, argv, params);

    QCOMPARE(config.getCommandlineValue("option"), QString("cliValue"));
    QVERIFY(config.hasCommandlineValue("option"));
}

void ConfigTest::testConfigFileValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"config", "Config value", "cfg", "default"};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    // The value is saved in the config file
    config.setValue("config", QString("fileValue"));

    QCOMPARE(config.getConfigFileValue("config"), QString("fileValue"));
    QVERIFY(config.hasConfigFileValue("config"));
}

void ConfigTest::testDefaultValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"default", "Default value", "def", "default"};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    // The option default value is used as the default value of the setting
    QCOMPARE(config.value("default"), QVariant("default"));
    QVERIFY(config.isDefaultValue("default"));
    QVERIFY(config.hasDefaultValue("default"));

    // A new default value overrides the option default value
    QHash<QString, QVariant> newDefaults;
    newDefaults.insert("default", QVariant("newDefault"));
    config.setDefaultValues(newDefaults);
    QCOMPARE(config.value("default"), QVariant("newDefault"));
}

void ConfigTest::testTypeConversion()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"int", "Int value", "i", "0"};
    params << QCommandLineOption{"bool", "Bool value", "b", "false"};
    params << QCommandLineOption{"double", "Double value", "d", "0.0"};

    const int argc = 7;
    const char* argv[] = {"test", "--int", "42", "--bool", "true", "--double", "3.14"};

    Config config(argc, argv, params);

    QCOMPARE(config.valueInt("int"), 42);
    QCOMPARE(config.valueBool("bool"), true);
    QCOMPARE(config.valueDouble("double"), 3.14);
}

void ConfigTest::testValueSource()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"source", "Source test", "s", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    // The value is not set in any of the sources
    QVERIFY(!config.hasCommandlineValue("source"));
    QVERIFY(!config.hasConfigFileValue("source"));
    QVERIFY(!config.hasDefaultValue("source"));
    QVERIFY(!config.isCommandlineValue("source"));
    QVERIFY(!config.isConfigFileValue("source"));
    QVERIFY(!config.isDefaultValue("source"));
}

void ConfigTest::testHasCommandlineValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"has-cli", "Has CLI", "h", ""};

    const int argc = 3;
    const char* argv[] = {"test", "--has-cli", "yes"};

    Config config(argc, argv, params);

    QVERIFY(config.hasCommandlineValue("has-cli"));
    QVERIFY(!config.hasCommandlineValue("has-cli-missing"));
}

void ConfigTest::testHasConfigFileValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"has-file", "Has file value", "f", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    QVERIFY(!config.hasConfigFileValue("has-file"));

    config.setValue("has-file", QString("fileValue"));
    QVERIFY(config.hasConfigFileValue("has-file"));
}

void ConfigTest::testHasDefaultValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"has-default", "Has Default", "d", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    QVERIFY(!config.hasDefaultValue("has-default"));

    config.setDefaultValue("has-default", QVariant("default"));
    QVERIFY(config.hasDefaultValue("has-default"));
}

void ConfigTest::testIsCommandlineValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"cli", "CLI test", "x", ""};

    const int argc = 3;
    const char* argv[] = {"test", "--cli", "cliValue"};

    Config config(argc, argv, params);

    QVERIFY(config.isCommandlineValue("cli"));
}

void ConfigTest::testIsConfigFileValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"is-config", "Config test", "y", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    config.setValue("is-config", QString("fileValue"));
    QVERIFY(config.isConfigFileValue("is-config"));
    QVERIFY(!config.isCommandlineValue("is-config"));
}

void ConfigTest::testIsDefaultValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"is-default", "Default test", "z", "defaultValue"};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    // The option default value is the default value of the setting
    QVERIFY(config.isDefaultValue("is-default"));
    QCOMPARE(config.value("is-default"), QVariant("defaultValue"));
}

void ConfigTest::testSetValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"set", "Set test", "s", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    config.setValue("set", QVariant("testValue"));
    QCOMPARE(config.value("set"), QVariant("testValue"));

    config.setValue("set", 42);
    QCOMPARE(config.value("set").toInt(), 42);
}

void ConfigTest::testGetParamsMap()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"param", "Param test", "p", ""};

    const int argc = 3;
    const char* argv[] = {"test", "--param", "paramValue"};

    Config config(argc, argv, params);

    QMap<QString, QString> paramsMap = config.getParamsMap();
    QVERIFY(paramsMap.contains("param"));
    QCOMPARE(paramsMap.value("param"), QString("paramValue"));
}

void ConfigTest::testSupportedParams()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"supported", "Supported test", "s", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    QList<QCommandLineOption> supported = config.getSupportedParams();
    QCOMPARE(supported.size(), 1);
    QCOMPARE(supported.first().names().value(0), QString("supported"));
}

void ConfigTest::testHelpText()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"help-text", "Help option", "h", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    QString help = config.getHelp();
    QVERIFY(!help.isEmpty());
    QVERIFY(help.contains("help-text"));
}

void ConfigTest::testDefaultValueStorage()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"storage", "Storage test", "st", ""};

    const int argc = 1;
    const char* argv[] = {"test"};

    Config config(argc, argv, params);

    QHash<QString, QVariant> defaults;
    defaults.insert("theme", QVariant("dark"));
    defaults.insert("language", QVariant("en"));
    config.setDefaultValues(defaults);

    QHash<QString, QVariant> stored = config.getDefaultValues();
    QCOMPARE(stored.value("theme"), QVariant("dark"));
    QCOMPARE(stored.value("language"), QVariant("en"));
}