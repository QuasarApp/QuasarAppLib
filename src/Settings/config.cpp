/*
 * Copyright (C) 2021-2026 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */

#include "Settings/config.h"

#include <QDebug>

namespace QuasarAppUtils {

Config::Config(int argc, const char* argv[], const OptionsDataList& options)
{
    // All the command-line parsing logic is already implemented in the
    // Params class, so the Config does not use the QCommandLineParser
    // directly.
    if (!Params::parseParams(argc, argv, options)) {
        qWarning() << "The Config command-line arguments parse error. "
                     "The command-line values will be ignored.";
        return;
    }

    // The command-line values have the highest priority, so they are
    // stored in the snapshot map and checked first in the
    // getValueImplementation method.
    _paramsMap = Params::getUserParamsMap();
}

const QMap<QString, QString>& Config::commandLineArgs() const {
    return _paramsMap;
}

QVariant Config::getValueImplementation(const QString& key, const QVariant& def)
{
    // 1. The command-line value has the highest priority.
    if (_paramsMap.contains(key)) {
        return _paramsMap.value(key);
    }

    // 2. The value that is saved in the config file (the Settings
    // backend) and 3. the default value are resolved by the Settings
    // base class. The def argument already contains the
    // ISettings::defaultSettings value (see the ISettings::getValue
    // method), so the default value has the lowest priority.
    return Settings::getValueImplementation(key, def);
}

} // namespace QuasarAppUtils