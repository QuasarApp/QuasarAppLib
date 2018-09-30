/*
 * Copyright (C) 2018 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "locales.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>

using namespace QuasarAppUtils;

bool Locales::initLocale(const QString &locale, QCoreApplication *app, QTranslator *translator){

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
