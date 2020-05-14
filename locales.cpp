/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "locales.h"

#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include "params.h"

using namespace QuasarAppUtils;


bool Locales::initLocale(QTranslator* translator) {
    return QCoreApplication::installTranslator(translator);
}

bool Locales::setLocale(const QString &prefix, const QString &locale) {
    auto translator = getTranslator();

    if (translator->isEmpty() && !initLocale(translator)) {
        Params::log("init translations fail! ", VerboseLvl::Error);
    }

    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    if(translator->load(QString("%0/%1").arg(prefix, locale))) {
        return true;
    }

    if(translator->load(QString("%0/%1").arg(prefix, defaultLocale))) {
        return true;
    }

    Params::log("set translations fail!", VerboseLvl::Warning);

    return false;
}

QTranslator *Locales::getTranslator() {
    static QTranslator *translator = new QTranslator();
    return translator;
}
