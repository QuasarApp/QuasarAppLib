/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "globalUtilsTest.h"

#include <QHash>
#include <QString>

#include "qaglobalutils.h"

namespace {
/**
 * @brief The TestFlags enum is a simple flag enum for test the enum
 *  bit operators of the QuasarAppUtils namespace.
 */
enum TestFlags {
    FlagNone = 0x0,
    FlagA    = 0x1,
    FlagB    = 0x2,
    FlagC    = 0x4,
};
}

void GlobalUtilsTest::test() {
    testRandomArray();
    testHashOperations();
    testEnumOperators();
    testTypeHashes();
}

void GlobalUtilsTest::testRandomArray() {
    const int sizes[] = {1, 2, 3, 7, 8, 16, 24, 64, 100, 256};
    for (auto size : sizes) {
        QByteArray result;
        randomArray(size, result);
        QCOMPARE(result.size(), size);
    }
}

void GlobalUtilsTest::testHashOperations() {
    // the subtract operation removes the right hash keys from the left hash.
    {
        QHash<QString, int> left{{"a", 1}, {"b", 2}, {"c", 3}};
        QHash<QString, int> right{{"b", 9}};
        subtract(left, right);
        QCOMPARE(left.size(), 2);
        QVERIFY(!left.contains("b"));
        QVERIFY(left.contains("a"));
        QVERIFY(left.contains("c"));
    }

    // the subtract operation clears the left hash when left and right
    // share the same data. The clear operation detaches the shared data
    // (copy on write), so the other copy keeps its values.
    {
        QHash<QString, int> left{{"a", 1}, {"b", 2}};
        QHash<QString, int> right = left;
        QVERIFY(left.isSharedWith(right));
        subtract(left, right);
        QVERIFY(left.isEmpty());
        QCOMPARE(right.size(), 2);
    }

    // the unite operation inserts the right hash values into the left hash.
    {
        QHash<QString, int> left{{"a", 1}};
        QHash<QString, int> right{{"a", 3}, {"b", 2}};
        unite(left, right);
        QCOMPARE(left.size(), 2);
        QCOMPARE(left.value("a"), 3);
        QCOMPARE(left.value("b"), 2);
    }

    // the unite operation is skipped when left and right share the same data.
    {
        QHash<QString, int> left{{"a", 1}};
        QHash<QString, int> right = left;
        QVERIFY(left.isSharedWith(right));
        unite(left, right);
        QCOMPARE(left.size(), 1);
        QCOMPARE(right.size(), 1);
    }

    // the match operation keeps only the keys that exists in the right hash.
    {
        QHash<QString, int> left{{"a", 1}, {"b", 2}, {"c", 3}};
        QHash<QString, int> right{{"b", 99}, {"c", 99}};
        match(left, right);
        QCOMPARE(left.size(), 2);
        QVERIFY(left.contains("b"));
        QVERIFY(left.contains("c"));
        QVERIFY(!left.contains("a"));
        // the values of the left hash are not changed by the match operation.
        QCOMPARE(left.value("b"), 2);
        QCOMPARE(left.value("c"), 3);
    }
}

void GlobalUtilsTest::testEnumOperators() {
    QCOMPARE(static_cast<int>(FlagA | FlagB), 3);
    QCOMPARE(static_cast<int>(FlagC & (FlagA | FlagC)), 4);
    QCOMPARE(static_cast<int>(FlagA ^ FlagB), 3);
    QCOMPARE(static_cast<int>(~FlagA), -2);
    QCOMPARE(static_cast<int>(FlagA << 1), 2);
    QCOMPARE(static_cast<int>(FlagC >> 1), 2);
}

void GlobalUtilsTest::testTypeHashes() {
    // all type based hashes are derived from the std::type_info hash code
    // of the type. The 32 bit version is the truncated (32 bit) value and
    // the 16/8 bit versions are the remainders of the full hash code.
    const std::size_t typeHash = typeid(int).hash_code();
    QCOMPARE(static_type_hash_32<int>(), static_cast<uint32_t>(typeHash));
    QCOMPARE(static_type_hash_16<int>(), static_cast<uint16_t>(typeHash % 0xFFFF));
    QCOMPARE(static_type_hash_8<int>(), static_cast<uint8_t>(typeHash % 0xFF));

    // the object based hash must be equal to the type based hash.
    int object = 1;
    QCOMPARE(static_type_hash_32(object), static_type_hash_32<int>());
    QCOMPARE(static_type_hash_16(object), static_type_hash_16<int>());
    QCOMPARE(static_type_hash_8(object), static_type_hash_8<int>());

    // different types must have different hashes.
    QVERIFY(static_type_hash_32<int>() != static_type_hash_32<QString>());
}
