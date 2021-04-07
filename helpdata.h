/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef HELPDATA_H
#define HELPDATA_H

#include <QMap>
#include "quasarapp_global.h"

namespace QuasarAppUtils{

namespace Help {
/**
 * @brief Options this is list of key - descriptions pairs of help.
 */
typedef QMultiMap<QString, QString> Options;

/**
 * @brief Charters ths is list of charters.
 */
typedef QMultiMap<QString, Options> Charters;

int width();

/**
 * @brief print Line of help.
 * @param key Option name.
 * @param value Description of option.
 */
void QUASARAPPSHARED_EXPORT print(const QString& key, const QString& value, int keyLength);

/**
 * @brief print Help Charter.
 * @param charter Charter of help.
 */
void QUASARAPPSHARED_EXPORT print(const Options& charter);

/**
 * @brief print All help.
 * @param help Help for printing.
 */
void QUASARAPPSHARED_EXPORT print(const Charters& help);

/**
 * @brief setLineLength sets new length of helps line.
 * @param newLength New size.
 */
void QUASARAPPSHARED_EXPORT setLineLength(int newLength);
}
}

#endif // HELPDATA_H
