/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef GLOBALUTILSTEST_H
#define GLOBALUTILSTEST_H

#include "test.h"

#include <QtTest>

/**
 * @brief The GlobalUtilsTest class test the global utility functions of the
 *  QuasarAppUtils namespace (randomArray, QHash operations, enum bit
 *  operators and the static type hashes).
 */
class GlobalUtilsTest: public Test
{
public:
    GlobalUtilsTest() = default;
    ~GlobalUtilsTest() override = default;

    void test() override;

protected:
    void testRandomArray();
    void testHashOperations();
    void testEnumOperators();
    void testTypeHashes();

};

#endif // GLOBALUTILSTEST_H
