/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef TEST_H
#define TEST_H

/**
 * @brief The Test class is the base class of all unit tests of the
 *  QuasarAppLib test suite. Each test class must implement the test method.
 */
class Test
{
public:
    Test() = default;
    virtual ~Test() = default;
    virtual void test() = 0;
};

#endif // TEST_H
