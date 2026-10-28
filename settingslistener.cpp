/*
 * Copyright (C) 2018-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isettings.h"
#include "settingslistener.h"

namespace QuasarAppUtils {

SettingsListener::SettingsListener() {

    auto settings = ISettings::instance();
    if (settings) {

        auto listener = [this](QString key, QVariant val){
            this->handleSettingsChanged(key, val);
        };

        _listenerConnection = QObject::connect(settings,
                                              &ISettings::valueChanged,
                                              listener);
    }
}

SettingsListener::~SettingsListener() {
    QObject::disconnect(_listenerConnection);
}

}
