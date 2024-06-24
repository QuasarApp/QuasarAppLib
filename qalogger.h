/*
 * Copyright (C) 2024-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QALOGGER_H
#define QALOGGER_H

#include "quasarapp_global.h"

#include <QList>

namespace QuasarAppUtils {

struct QUASARAPPSHARED_EXPORT LogData {
    char type = 0;
    int time = 0;
    QString text;
};

/**
 * @brief The QALogger class is logger handler for app.
 */
class QUASARAPPSHARED_EXPORT QALogger
{
public:
    QALogger();

    void init();


};
}
#endif // QALOGGER_H
