/*
 * Copyright (C) 2018-2019 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "params.h"
#include <QVariantMap>
#include <QDebug>
#include <QFileInfo>
#ifdef Q_OS_WIN
#include "windows.h"
#else
#include <unistd.h>
#include <limits.h>
#endif

using namespace QuasarAppUtils;

static QVariantMap params = QVariantMap();


bool Params::isEndable(const QString& key) {
    return params.contains(key);
}

void Params::verboseLog(const QString &log, VerboseLvl vLvl) {
    if (isEndable("verbose")) {

        auto lvl = static_cast<VerboseLvl>(getArg("verbose").toInt());

        if (vLvl <= lvl) {

            switch (vLvl) {

            case VerboseLvl::Error: {
                qCritical() << "Error: " + log;
                break;
            }

            case VerboseLvl::Warning: {
                qWarning() << "Warning: " + log;
                break;
            }

            case VerboseLvl::Info: {
                qInfo() << "Info: " + log;
                break;
            }

            default: {
                qDebug() << "Verbose log: " + log ;
                break;
            }
            }
        }
    }
}

bool Params::parseParams(int argc,const char *argv[]) {
    params.clear();

#ifdef Q_OS_WIN
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    params ["appPath"] = QFileInfo(buffer).absolutePath();
#else
    char path[2048];
    if (readlink("/proc/self/exe", path, 2048) < 0) {
        qWarning() << "parseParams can't get self path!" ;
        return false;
    }
    params ["appPath"] =  QFileInfo(path).absolutePath();
#endif

    if (!getStrArg("appPath").size()) {
        return false;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && i ) {
            if (i < (argc - 1) && argv[i + 1][0] != '-') {
                params[&(argv[i][1])] = argv[i + 1];
                i++;
            } else {
                qWarning() << "Missing argument for " + QString(argv[i]) ;
            }
        } else {
            params[argv[i]] = "";
        }
    }

    return true;
}

QString Params::getStrArg(const QString& key) {
    return params.value(key, "").toString();
}

QVariant Params::getArg(const QString& key) {
    return params.value(key, "");
}

void Params::setArg(const QString &key, const QVariant &val) {
    params.insert(key, val);
}

void Params::setEnable(const QString &key, bool enable) {
    if (enable) {
        params.insert(key, "");
    } else {
        params.remove(key);
    }
}
