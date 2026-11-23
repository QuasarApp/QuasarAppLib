/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "configTest.h"

#include <QStringList>
#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "Config/config.h"

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
    // Test with empty parameters
    QList<QCommandLineOption> emptyParams;
    Config config(0, nullptr, emptyParams);
    QVERIFY(config.isValid());
    
    // Test with parameters
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-verbose", "Enable verbose logging", "level", "0"};
    params << QCommandLineOption{"-output", "Output file", "path", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config2(argc, argv, params);
    QVERIFY(config2.isValid());
}

void ConfigTest::testPriorityOrder()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-value", "Test value", "val", "default"};
    
    int argc = 3;
    char *argv[] = {"test", "-value", "commandlineValue"};
    
    Config config(argc, argv, params);
    
    // Command-line value should have highest priority
    QCOMPARE(config.value("value"), QString("commandlineValue"));
    QVERIFY(config.isCommandlineValue("value"));
    
    // Set config file value (same as command-line)
    config.setValue("value", "configValue");
    QCOMPARE(config.value("value"), QString("configValue"));
    QVERIFY(config.isConfigFileValue("value"));
}

void ConfigTest::testCommandlineValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-option", "Test option", "opt", "default"};
    
    int argc = 3;
    char *argv[] = {"test", "-option", "cliValue"};
    
    Config config(argc, argv, params);
    
    QCOMPARE(config.getCommandlineValue("option"), QString("cliValue"));
    QVERIFY(config.hasCommandlineValue("option"));
}

void ConfigTest::testConfigFileValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-config", "Config value", "cfg", "default"};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    // Set a config file value
    config.setValue("config", "fileValue");
    
    QCOMPARE(config.getConfigFileValue("config"), QString("fileValue"));
    QVERIFY(config.hasConfigFileValue("config"));
}

void ConfigTest::testDefaultValues()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-default", "Default value", "def", "default"};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    // Initially should use default value
    QCOMPARE(config.value("default"), QString("default"));
    QVERIFY(config.isDefaultValue("default"));
    
    // Set a new default
    config.setDefaultValues({{"default", QVariant::fromValue("newDefault")}});
    QCOMPARE(config.value("default"), QString("newDefault"));
}

void ConfigTest::testValueSource()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-source", "Source test", "s", "default"};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    // Initially no value source
    QVERIFY(!config.hasCommandlineValue("source"));
    QVERIFY(!config.hasConfigFileValue("source"));
    QVERIFY(!config.hasDefaultValue("source"));
}

void ConfigTest::testHasCommandlineValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-has-cli", "Has CLI", "h", ""};
    
    int argc = 3;
    char *argv[] = {"test", "-has-cli", "yes"};
    
    Config config(argc, argv, params);
    
    QVERIFY(config.hasCommandlineValue("has-cli"));
}

void ConfigTest::testHasConfigFileValue()
{
    QVERIFY(config.hasDefaultValue("has-default"));
}

void ConfigTest::testIsCommandlineValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-cli", "CLI test", "x", ""};
    
    int argc = 3;
    char *argv[] = {"test", "-cli", "cliValue"};
    
    Config config(argc, argv, params);
    
    QVERIFY(config.isCommandlineValue("cli"));
}

void ConfigTest::testIsConfigFileValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-config", "Config test", "y", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    config.setValue("config", "fileValue");
    QVERIFY(config.isConfigFileValue("config"));
}

void ConfigTest::testIsDefaultValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-default", "Default test", "z", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    config.setDefaultValues({{"default", QVariant::fromValue("defaultValue")}});
    QVERIFY(config.isDefaultValue("default"));
}

void ConfigTest::testSetValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-set", "Set test", "s", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    config.setValue("set", QVariant::fromValue("testValue"));
    QCOMPARE(config.value("set"), QString("testValue"));
    
    config.setValue("set", 42);
    QCOMPARE(config.value("set").toInt(), 42);
}

void ConfigTest::testGetParamsMap()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-param", "Param test", "p", ""};
    
    int argc = 3;
    char *argv[] = {"test", "-param", "paramValue"};
    
    Config config(argc, argv, params);
    
    auto paramsMap = config.getParamsMap();
    QVERIFY(paramsMap.contains("param"));
    QCOMPARE(paramsMap["param"], QString("paramValue"));
}

void ConfigTest::testSupportedParams()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-supported", "Supported test", "s", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    auto supported = config.getSupportedParams();
    QCOMPARE(supported.size(), 1);
    QCOMPARE(supported.first().names().first(), QString("supported"));
}

void ConfigTest::testHelpText()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-help", "Help option", "h", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    QString help = config.getHelp();
    QVERIFY(!help.isEmpty());
}

void ConfigTest::testDefaultValueStorage()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-storage", "Storage test", "st", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    config.setDefaultValues({
        {"theme", QVariant::fromValue("dark")},
        {"language", QVariant::fromValue("en")}
    });
    
    QCOMPARE(config.getDefaultValues()["theme"], QVariant::fromValue("dark"));
    QCOMPARE(config.getDefaultValues()["language"], QVariant::fromValue("en"));
}

void ConfigTest::testHasDefaultValue()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-has-default", "Has Default", "d", ""};
    
    int argc = 1;
    char *argv[] = {"test"};
    
    Config config(argc, argv, params);
    
    config.setDefaultValues({{"has-default", QVariant::fromValue("default")}});
    QVERIFY(config.hasDefaultValue("has-default"));
}


void ConfigTest::testTypeConversion()
{
    QList<QCommandLineOption> params;
    params << QCommandLineOption{"-int", "Int value", "i", "0"};
    params << QCommandLineOption{"-bool", "Bool value", "b", "false"};
    params << QCommandLineOption{"-double", "Double value", "d", "0.0"};
    
    int argc = 3;
    char *argv[] = {"test", "-int", "42", "-bool", "true", "-double", "3.14"};
    
    Config config(argc, argv, params);
    
    QCOMPARE(config.valueInt("int"), 42);
    QCOMPARE(config.valueBool("bool"), true);
    QCOMPARE(config.valueDouble("double"), 3.14);
}