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
 */
class QUASARAPPSHARED_EXPORT QALogger
{
public:
    QALogger();
    ~QALogger();

    /**
     * @brief init This method initialize logging of all qt message into file.
     */
    void init();

};
}
#endif // QALOGGER_H
