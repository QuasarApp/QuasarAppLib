/*
 * Copyright (C) 2024-2026 QuasarApp.
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
 * @brief The iPlatformUtils class bas interface for get access platform dependent functions and constants.
 */
class QUASARAPPSHARED_EXPORT PlatformUtils
{
public:
    /**
     * @brief PlatformUtils This is the default constructor of the PlatformUtils object.
     */
    PlatformUtils();

    /**
     * @brief isSnap This method check distribution type. true if the application packed as a snap.
     * @return true if the application packaged as a snap else false.
     */
    static bool isSnap();

    /**
     * @brief snapRootFS returns root file system of main OS (available read only)
     * @return path ro main os root.
     */
    static QString snapRootFS();

    /**
     * @brief transportPathToSnapRoot change path to snap available.
     * @param path source path of main OS.
     * @return readable path location for snap package.
     * @note this method is not a fast, so not invoke it too often.
     */
    static QString transportPathToSnapRoot(const QString &path);

    /**
     * @brief checkSystemBackupSnapInterface This method check access to backup permission of snap package.
     *  This permission required to get access to rootfs of main OS.
     * @return true if the app have this access.
     */
    static bool checkSystemBackupSnapInterface();
};
}
#endif // IPLATFORMUTILS_H
