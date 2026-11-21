/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include <QtTest>

#include "globalUtilsTest.h"
#include "localesTest.h"
#include "optionDataTest.h"
#include "paramsTest.h"
#include "settingsTest.h"

#define TestCase(name, testClass) \
    void name() { \
    initTest(new testClass()); \
    }

/**
 * @brief The testQuasarApp class is the main test runner of the
 *  QuasarAppLib unit test suite. Each slot starts one unit test class
 *  from the units directory.
 */
class testQuasarApp : public QObject
{
    Q_OBJECT
public:
    testQuasarApp();

    ~testQuasarApp();

private slots:
    // BEGIN TESTS CASES

    TestCase(globalUtilsTest, GlobalUtilsTest);
    TestCase(optionDataTest, OptionDataTest);
    TestCase(paramsTest, ParamsTest);
    TestCase(settingsTest, SettingsTest);
    TestCase(localesTest, LocalesTest);

    // END TEST CASES

private:
    void initTest(Test* test);

    QCoreApplication *_app = nullptr;

};

testQuasarApp::testQuasarApp() {

    int argc = 0;
    char * argv[] = {nullptr};

    _app = new QCoreApplication(argc, argv);
    QCoreApplication::setApplicationName("QuasarAppLibTest");
    QCoreApplication::setOrganizationName("QuasarApp");

}

testQuasarApp::~testQuasarApp() {
    delete _app;
}

void testQuasarApp::initTest(Test *test) {
    QTimer::singleShot(0, this, [this, test]() {
        test->test();
        delete test;
        _app->exit(0);
    });

    _app->exec();
}

QTEST_APPLESS_MAIN(testQuasarApp)

#include "tst_quasarapp.moc"
