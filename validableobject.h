//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VALIDABLEOBJECT_H
#define VALIDABLEOBJECT_H

namespace QuasarAppUtils{

/**
 * @brief The ValidableObject class is Base interface for all object that can be checked to valid.
 */
class ValidableObject
{
public:
    ValidableObject();

    /**
     * @brief isValid This method return true if the object is valid else false.
     * @return true if the object is valid else false.
     */
    virtual bool isValid() const = 0;
};

/**
 * @brief iVO This is short abriviature of the ValidableObject class.
 */
typedef ValidableObject iVO;

}
#endif // VALIDABLEOBJECT_H
