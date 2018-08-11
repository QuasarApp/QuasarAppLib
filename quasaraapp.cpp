/*
 * Copyright (C) 2018 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "quasaraapp.h"
#include <QVariantMap>
#include <QDebug>
#include <QCoreApplication>
#include <QTranslator>

static QVariantMap params = QVariantMap();

bool QuasaraAppUtils::parseParams(int argc, char *argv[]) {
    params.clear();
    params ["appPath"] =  argv[0];

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && i ) {
            if (i < (argc - 1) && argv[i + 1][0] != '-') {
                params[&(argv[i][1])] = argv[i + 1];
                i++;
            } else {
                qDebug() << "Missing argument for " + QString(argv[i]) ;
            }
        } else {
            params[argv[i]] = "";
        }
    }

    return true;
}

QString QuasaraAppUtils::getStr(const QString& key) {
    return  params.value(key, "").toString();
}

QVariant QuasaraAppUtils::get(const QString& key) {
    return  params.value(key, "");
}

bool QuasaraAppUtils::isEndable(const QString& key) {
    return  params.contains(key);
}

bool initLocale(const QString &locale, QCoreApplication *app, QTranslator *translator){

    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    if(!locale.isEmpty() && translator->load(QString(":/languages/%0").arg(locale))){
        return app->installTranslator(translator);
    }

    if(!translator->load(QString(":/languages/%0").arg(defaultLocale))){
        return false;
    }

    return app->installTranslator(translator);
}
