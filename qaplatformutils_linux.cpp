/*
 * Copyright (C) 2024-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "qaplatformutils.h"

#if defined(Q_OS_LINUX)
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#elif defined(Q_OS_ANDROID)
#elif defined(Q_OS_WIN32)
#elif defined(Q_OS_MACOS)
#elif defined(Q_OS_IOS)
#endif

namespace QuasarAppUtils {

#if  defined(Q_OS_LINUX)
bool PlatformUtils::isSnap() {
    return QProcessEnvironment::systemEnvironment().value("SNAP").size();
}

QString PlatformUtils::snapRootFS() {
    return "/var/lib/snapd/hostfs";
}

QString PlatformUtils::transportPathToSnapRoot(const QString &path) {
    if (isSnap() && checkSystemBakupSnapInterface()) {

        if(QFileInfo(path).isWritable()) {
            return path;
        }

        if (path.size() && path[0] != QString("/")) {
            auto absalutPath = QProcessEnvironment::systemEnvironment().value("PWD") + "/" + path;
            if (!absalutPath.contains(snapRootFS())) {
                return snapRootFS() + "/" + absalutPath;
            }
        }

        if (!path.contains(snapRootFS())) {
            return snapRootFS() + "/" + path;
        }
    }

    return path;
}

bool PlatformUtils::checkSystemBakupSnapInterface() {
    return QDir(snapRootFS()).entryList(QDir::AllEntries | QDir::NoDotAndDotDot).size();
}

#elif defined(Q_OS_ANDROID)

bool PlatformUtils::isSnap() { return false;}

QString PlatformUtils::snapRootFS() { return "";}

QString PlatformUtils::transportPathToSnapRoot(const QString &path) { return path; }

bool PlatformUtils::checkSystemBakupSnapInterface() { return false; }

#elif defined(Q_OS_WIN32)

bool PlatformUtils::isSnap() { return false;}

QString PlatformUtils::snapRootFS() { return "";}

QString PlatformUtils::transportPathToSnapRoot(const QString &path) { return path; }

bool PlatformUtils::checkSystemBakupSnapInterface() { return false; }

#elif defined(Q_OS_MACOS)

bool PlatformUtils::isSnap() { return false;}

QString PlatformUtils::snapRootFS() { return "";}

QString PlatformUtils::transportPathToSnapRoot(const QString &path) { return path; }

bool PlatformUtils::checkSystemBakupSnapInterface() { return false; }

#elif defined(Q_OS_IOS)

bool PlatformUtils::isSnap() { return false;}

QString PlatformUtils::snapRootFS() { return "";}

QString PlatformUtils::transportPathToSnapRoot(const QString &path) { return path; }

bool PlatformUtils::checkSystemBakupSnapInterface() { return false; }

#endif

}
