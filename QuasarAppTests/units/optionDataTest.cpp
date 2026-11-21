/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "optionDataTest.h"

#include <optiondata.h>

using namespace QuasarAppUtils;

void OptionDataTest::test() {
    testAccessors();
    testValidity();
    testDeprecatedAndRemoved();
    testToHelp();
}

void OptionDataTest::testAccessors() {
    OptionData option({"-testOption", "-t"},
                      "(path to file)",
                      "Test option description",
                      "-testOption /some/file");

    QCOMPARE(option.names().size(), 2);
    QCOMPARE(option.names().at(0), QString("-testOption"));
    QCOMPARE(option.names().at(1), QString("-t"));
    QCOMPARE(option.arguments(), QString("(path to file)"));
    QCOMPARE(option.description(), QString("Test option description"));
    QCOMPARE(option.example(), QString("-testOption /some/file"));
}

void OptionDataTest::testValidity() {
    // an option with at least one name is valid.
    OptionData validOption({"-validOption"});
    QVERIFY(validOption.isValid());

    // an option without names is invalid.
    OptionData invalidOption({});
    QVERIFY(!invalidOption.isValid());
}

void OptionDataTest::testDeprecatedAndRemoved() {
    // a plain option is not deprecated and not removed.
    OptionData plainOption({"-plainOption"});
    QVERIFY(!plainOption.isDeprecated());
    QVERIFY(!plainOption.isRemoved());

    // an option with a deprecation message is deprecated.
    OptionData deprecatedOption({"-depOption"},
                                "(value)",
                                "Deprecated option",
                                "",
                                "This option is deprecated.");
    QVERIFY(deprecatedOption.isDeprecated());
    QCOMPARE(deprecatedOption.deprecatedMsg(),
             QString("This option is deprecated."));
    QVERIFY(!deprecatedOption.isRemoved());

    // an option with the removed flag is removed.
    OptionData removedOption({"-rmOption"},
                             "(value)",
                             "Removed option",
                             "",
                             "This option was removed.",
                             true);
    QVERIFY(removedOption.isRemoved());
    QVERIFY(removedOption.isDeprecated());
}

void OptionDataTest::testToHelp() {
    OptionData option({"-a", "-b"}, "(arg)", "desc", "ex");
    auto help = option.toHelp();

    QCOMPARE(help.size(), 1);
    auto it = help.begin();
    QCOMPARE(it.key(), QString("-a / -b (arg)"));
    QCOMPARE(it.value(), QString("desc Example: ex"));

    // the example part is skipped if the example is empty.
    OptionData optionWithoutExample({"-c"}, "(arg)", "desc only");
    auto helpWithoutExample = optionWithoutExample.toHelp();
    QCOMPARE(helpWithoutExample.size(), 1);
    QCOMPARE(helpWithoutExample.begin().value(), QString("desc only"));
}
