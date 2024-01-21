/*
 * Copyright (C) 2024-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef PLATFORMUTILS_H
#define PLATFORMUTILS_H

#include <QString>
#include "quasarapp_global.h"

namespace QuasarAppUtils {

/**
 * @brief The iPlatformUtils class bas interface for get access platform dependet functions and constants.
 */
class QUASARAPPSHARED_EXPORT PlatformUtils
{
public:
    PlatformUtils();

    /**
     * @brief isSnap This method check destribution type. true if the application packed as a snap.
     * @return true if the application packaged as a snap else false.
     */
    static bool isSnap();

    /**
     * @brief snapRootFS returns root file system of main OS (available read only)
     * @return path ro main os root.
     */
    static QString snapRootFS();

    /**
     * @brief transportPathToSnapRoot change path to snap avaialble.
     * @param path source path of main OS.
     * @return readable path location for snap pacakge.
     * @note this method is not a fast, so not invoke it too offten.
     */
    static QString transportPathToSnapRoot(const QString &path);

    /**
     * @brief checkSystemBakupSnapInterface This method check access to bakcup permision of snap pacakge.
     *  This permision required to get access to rootfs of main OS.
     * @return true if the app have this accesss.
     */
    static bool checkSystemBakupSnapInterface();
};
}
#endif // IPLATFORMUTILS_H
