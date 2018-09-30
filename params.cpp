/*
 * Copyright (C) 2018 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "params.h"
#include <QVariantMap>
#include <QDebug>

using namespace QuasarAppUtils;

static QVariantMap params = QVariantMap();


bool Params::isEndable(const QString& key) {
    return params.contains(key);
}

bool Params::parseParams(int argc, char *argv[]) {
    params.clear();
    params ["appPath"] =  argv[0];

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
