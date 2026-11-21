/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "settingsTest.h"

#include <QCoreApplication>
#include <QFile>
#include <QHash>
#include <QSettings>

#include "isettings.h"
#include "settings.h"
#include "settingslistener.h"

using namespace QuasarAppUtils;

namespace {
/**
 * @brief The TestSettings class is a test implementation of the Settings
 *  class with default values and the bool options support.
 */
class TestSettings: public Settings
{
public:
    TestSettings() = default;

    bool isBool(const QString &key) const override {
        return boolOptions().contains(key);
    }

    QHash<QString, QVariant> defaultSettings() override {
        return {{"defaultKey", "defaultValue"}};
    }
};

/**
 * @brief The TestSettingsListener class is a test implementation of the
 *  SettingsListener class that collects all changed keys.
 */
class TestSettingsListener: public SettingsListener
{
public:
    QStringList changedKeys;
    QVariant lastValue;

protected:
    void handleSettingsChanged(const QString &key, const QVariant &value) override {
        changedKeys.append(key);
        lastValue = value;
    }
};
}

SettingsTest::SettingsTest() {
    // remove the settings file of a previous test run.
    QSettings settings(QSettings::IniFormat, QSettings::Scope::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    QFile::remove(settings.fileName());
}

SettingsTest::~SettingsTest() {
    // remove the settings file that was created by the tests.
    QSettings settings(QSettings::IniFormat, QSettings::Scope::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    QFile::remove(settings.fileName());
}

void SettingsTest::test() {
    testInitService();
    testValueStorage();
    testSignals();
    testSaveModes();
    testDefaultValues();
    testBoolOptions();
    testSettingsListener();
    testDeinitService();
}

void SettingsTest::testInitService() {
    auto uniqueSettings = std::make_unique<TestSettings>();
    _testSettings = uniqueSettings.get();
    QVERIFY(ISettings::initService(std::move(uniqueSettings)));

    auto instance = ISettings::instance();
    QVERIFY(instance != nullptr);

    // the autoInstance method must return the already initialized service.
    QCOMPARE(Settings::autoInstance(), static_cast<ISettings*>(instance));

    // a second initialisation must return false and keep the current instance.
    QVERIFY(!Settings::initService());
    QVERIFY(ISettings::instance() == instance);
}

void SettingsTest::testValueStorage() {
    auto instance = ISettings::instance();

    instance->setStrValue("stringKey", "stringValue");
    QCOMPARE(instance->getStrValue("stringKey"), QString("stringValue"));
    QCOMPARE(instance->getValue("stringKey").toString(), QString("stringValue"));

    instance->setValue("intKey", 42);
    QCOMPARE(instance->getValue("intKey").toInt(), 42);

    // the default value is returned if the key was not set.
    QCOMPARE(instance->getStrValue("notSetKey", "defValue"), QString("defValue"));
}

void SettingsTest::testSignals() {
    auto instance = ISettings::instance();

    QString caughtKey;
    QVariant caughtValue;
    QString caughtStrValue;

    auto valueConnection = QObject::connect(instance, &ISettings::valueChanged,
                                            [&](const QString &key, const QVariant &value) {
        caughtKey = key;
        caughtValue = value;
    });

    auto strConnection = QObject::connect(instance, &ISettings::valueStrChanged,
                                          [&](const QString &key, const QString &value) {
        caughtStrValue = value;
        Q_UNUSED(key);
    });

    instance->setValue("signalKey", "signalValue");

    QCOMPARE(caughtKey, QString("signalKey"));
    QCOMPARE(caughtValue.toString(), QString("signalValue"));
    QCOMPARE(caughtStrValue, QString("signalValue"));

    // the lambdas capture the local variables of this method by reference,
    // so the connections must be removed before the method returns.
    QObject::disconnect(valueConnection);
    QObject::disconnect(strConnection);
}

void SettingsTest::testSaveModes() {
    auto instance = ISettings::instance();
    QCOMPARE(instance->getMode(), SettingsSaveMode::Auto);

    instance->setMode(SettingsSaveMode::Manual);
    QCOMPARE(instance->getMode(), SettingsSaveMode::Manual);

    instance->setStrValue("manualKey", "manualValue");

    // in the manual mode the value must not be saved on disk until the sync call.
    QSettings backend(QSettings::IniFormat, QSettings::Scope::UserScope,
                      QCoreApplication::organizationName(),
                      QCoreApplication::applicationName());
    QVERIFY(backend.value("manualKey").isNull());

    instance->sync();

    // reload the backend from disk before the check, because the QSettings
    // object was created before the sync call.
    backend.sync();
    QCOMPARE(backend.value("manualKey").toString(), QString("manualValue"));

    instance->setMode(SettingsSaveMode::Auto);
}

void SettingsTest::testDefaultValues() {
    auto instance = ISettings::instance();

    // the default value from the defaultSettings map must be returned
    // if the key was not set yet.
    QCOMPARE(instance->getStrValue("defaultKey"), QString("defaultValue"));

    instance->setStrValue("defaultKey", "changedValue");
    QCOMPARE(instance->getStrValue("defaultKey"), QString("changedValue"));

    // the resetToDefault method must restore the default value.
    instance->resetToDefault();
    QCOMPARE(instance->getStrValue("defaultKey"), QString("defaultValue"));
}

void SettingsTest::testBoolOptions() {
    QVERIFY(_testSettings != nullptr);

    QVERIFY(_testSettings->boolOptions().isEmpty());
    _testSettings->setBoolOptions({"boolKey"});
    QVERIFY(_testSettings->boolOptions().contains("boolKey"));

    // the service instance writes the value into the QSettings backend (auto mode).
    ISettings::instance()->setValue("boolKey", 1);
    QVERIFY(ISettings::instance()->getValue("boolKey").toBool());

    // a new settings object with the same storage must read the value as bool,
    // because the key is registered in the boolOptions map.
    TestSettings localSettings;
    localSettings.setBoolOptions({"boolKey"});
    QVERIFY(localSettings.getValue("boolKey").toBool());

    // now the stored value must be read as false.
    ISettings::instance()->setValue("boolKey", 0);

    TestSettings localSettings2;
    localSettings2.setBoolOptions({"boolKey"});
    QVERIFY(!localSettings2.getValue("boolKey").toBool());
}

void SettingsTest::testSettingsListener() {
    TestSettingsListener listener;

    ISettings::instance()->setStrValue("listenerKey", "listenerValue");

    QVERIFY(listener.changedKeys.contains("listenerKey"));
    QCOMPARE(listener.lastValue.toString(), QString("listenerValue"));
}

void SettingsTest::testDeinitService() {
    ISettings::deinitService();
    QVERIFY(ISettings::instance() == nullptr);
}
