/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef DOCTORTEST_H
#define DOCTORTEST_H

#include <QSharedPointer>

namespace QuasarAppUtils {

class iPill;
class Doctor;

/**
 * @brief The DoctorTest class This class contains auto tests for pills objects.
 * @see Doctor class for get more information.
 */
class DoctorTest
{
public:
    DoctorTest();

    /**
     * @brief test This method run simple test for the pill object.
     * **Test algorithm**
     *
     * 1. Doctor run the iPill::diagnostic method.
     * 2. If an @a appIsBroken is false then diagnostic method should return true
     * 3. If an @a appIsBroken is true then diagnostic method should return true.
     * 4. Doctor run the fix and check diagnostic method again.
     * 5. The diagnostic method should return false value.
     *
     * @param pill This is checked pill for solve issue.
     * @param appIsBroken bollean variable that should be true if app is broken else false.
     * @return true if test passed elase false.
     */
    bool test(const QSharedPointer<iPill>& pill, bool appIsBroken) const;

};

}
#endif // DOCTORTEST_H
