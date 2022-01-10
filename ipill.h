/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef IPILL_H
#define IPILL_H

#include <QString>


namespace QuasarAppUtils {

/**
 * @brief The iPill class is base interface for the pill object.
 * The Pill object is independet tool that must be fix one issue for runned application.
 *
 * ## For Example:
 *
 * You has a sume bug, but you don't known about this bug.
 *  This big broken the database for your users, but you still don't known about this.
 * You found this bug and create new patch for fix it, but databae of your users alredy broken, and you users can't fix this issues yasterself.
 *  So you can create a pill for fix broked database and delive to your users for fix thes issue.
 *
 * The Pill object structure should be has:
 * * The Action that should be check if contains this issue or not ...
 * * The Action taht should be fix issue.
 * * Description of the issue.
 * * Name of the issue.
 *
 * * @see iPill
 */
class iPill
{
public:
    iPill();

    /**
     * @brief name This method should be return name of this pill.
     * @return name of this pill.
     */
    virtual QString name() const = 0;

    /**
     * @brief description This method should be return dital description of the issue that this pill will fix after execute (accept).
     * @return string valeu with description.
     */
    virtual QString description() const = 0;

protected:

    /**
     * @brief diagnostick This method execute action that should be check if exits issues or not.
     * @note This method will executed only on the Doctor object.
     * @return true if the issues is detected.
     */
    virtual bool diagnostick() const = 0;

    /**
     * @brief fix This method should be fix detected issue.
     * @return true if the issue fixes successful else false
     */
    virtual bool fix() const = 0;


    friend class Doctor;
};

}
#endif // IPILL_H
