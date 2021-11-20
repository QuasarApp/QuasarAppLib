/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isettings.h"
#include "settingslistner.h"
#include "params.h"

namespace QuasarAppUtils {

SettingsListner::SettingsListner() {
}

SettingsListner::~SettingsListner() {
    QObject::disconnect(_listnerConnection);
}

bool SettingsListner::handleSettings() {

    auto settings = ISettings::instance();
    if (!settings) {
        Params::log("Settings object is not initialised!"
                    "please invoke ISettings::init before subscribe",
                    Error);
        return false;
    }

    auto listner = [this](QString key, QVariant val){
        this->handleSettingsChanged(key, val);
    };

    QObject * thiz = dynamic_cast<QObject*>(this);

    if (!thiz) {
        Params::log("Object that use the SettingsListner should be QObject."
                    "The handleSettingsChanged method will not invoked when settings has changed",
                    Error);
        return false;
    }

    _listnerConnection = thiz->connect(settings,
                                       &ISettings::valueChanged,
                                       thiz, listner,
                                       Qt::QueuedConnection);

    return true;
}

}
