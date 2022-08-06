//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef HUMANREADABLEOBJECT_H
#define HUMANREADABLEOBJECT_H

#include <QString>

namespace QuasarAppUtils {

/**
 * @brief The HumanReadableObject interface This is simple class  that add one virtula method toString.
 * All childs object should be override this method.
 */
class HumanReadableObject
{
public:
    /**
     * @brief toString This method convert this object to human readable string.
     * @return human readable string of this object.
     */
    virtual QString toString() const = 0;

protected:
    HumanReadableObject();
};

/**
 * @brief iHRO This is short abriviature of the HumanReadableObject class.
 */
typedef HumanReadableObject iHRO;

}
#endif // HUMANREADABLEOBJECT_H
