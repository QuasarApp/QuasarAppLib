/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isettings.h"
#include "settingslistner.h"

namespace QuasarAppUtils {

SettingsListner::SettingsListner() {

    auto settings = ISettingsService::instance();
    if (settings) {

        auto listner = [this](QString key, QVariant val){
            this->handleSettingsChanged(key, val);
        };

        _listnerConnection = QObject::connect(settings,
                                              &ISettings::valueChanged,
                                              listner);
    }
}

SettingsListner::~SettingsListner() {
    QObject::disconnect(_listnerConnection);
}

}
