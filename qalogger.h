/*
 * Copyright (C) 2024-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QALOGGER_H
#define QALOGGER_H

#include "quasarapp_global.h"

#include <QFile>
#include <QList>

namespace QuasarAppUtils {

/**
 * @brief The QALogger class is logger handler for app.
 * This class allow to log all message from app to file.
 *
 * Example:
 * @code
 * #include <qalogger.h>
 *
 * QuasarAppUtils::QALogger logger;
 *
 *   QuasarAppUtils::Params::setEnable("fileLog", true); //force enable file writing using efault file location - located in localApplication data.
 *
 * QuasarAppUtils::Params::parseParams(argc, argv);
 *
 * logger.init();
 * @endcode
 *
 *
 * Standart file log locations per platforms :
 * - Windows: %APPDATA%/OrganisationName/YourAppName/YourAppName.log
 * - Linux: $HOME/.config/OrganisationName/YourAppName/YourAppName.log
 * - Mac: $HOME/Library/Application Support/OrganisationName/YourAppName/YourAppName.log
 * - Android: /data/data/com.organisationName.yourAppName/files/YourAppName.log
 * - iOS: /var/mobile/Applications/Data/YourAppName/YourAppName.log
 *
 * you can overiwite this location by setting "fileLog" option in Params.
 */
class QUASARAPPSHARED_EXPORT QALogger
{
public:
    QALogger();
    ~QALogger();

    /**
     * @brief init This method initialize logging of all qt message into file.
     * @note This function should be invokae after  parsing arguments.
     *  if you invoke this before parsing arguments, verbose level of logs will not created correct.
     */
    void init();

    /**
     * @brief setVerboseLevel This method set verbose level of the logger.
     * @param lvl This is new verbose level.
     */
    static QString getLogFilePath();

};
}
#endif // QALOGGER_H
