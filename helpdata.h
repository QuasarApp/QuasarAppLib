/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef HELPDATA_H
#define HELPDATA_H

#include <QMap>
namespace QuasarAppUtils{

namespace Help {
/**
 * @brief Options - this is list of key - descriptions pairs of help.
 */
typedef QMultiMap<QString, QString> Options;

/**
 * @brief Charters = ths is list of charters.
 */
typedef QMultiMap<QString, Options> Charters;

int width();

/**
 * @brief print - line of help
 * @param key - option name
 * @param value - description of option
 */
void print(const QString& key, const QString& value, int keyLength);

/**
 * @brief print = help Charter
 * @param charter - charter of help
 */
void print(const Options& charter);

/**
 * @brief print - all help
 * @param help - help for printing
 */
void print(const Charters& help);

/**
 * @brief setLineLength - sets new length of helps line
 * @param newLength - new size
 */
void setLineLength(int newLength);
}
}

#endif // HELPDATA_H
