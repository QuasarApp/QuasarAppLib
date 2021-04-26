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

/**
 * @brief Help namespace contains functions for printing help in to console. All Print fucntions automaticly calc width of the console and aligns the text to fit the window.
 */
namespace Help {
/**
 * @brief Options this is list of @bold key-descriptions pairs of help.
 *  The @bold key is name of the available argument and @bold description is description of the available argument.
 *
 * @bold Example:
 *
 * @code{cpp}
 * Options myOptionsList = {{"argument1", "This is test argumetn1 of my application."},
 *                          {"argument1", "This is test argumetn1 of my application."}};
 *
 * @endcode
 */
typedef QMultiMap<QString, QString> Options;

/**
 * @brief Section This is list of the help Sections. The one section it is Title of the section and Help::Options list.
 *
 * @bold Example:
 * @code{cpp}
 * Options myOptionsList = {{"argument1", "This is test argumetn1 of my application."},
 *                          {"argument1", "This is test argumetn1 of my application."}};
 * Section mySections = {{"This Is main section of the help", myOptionsList}};
 * QuasarAppUtils::Help::print(mySections);
 * @endcode
 */
typedef QMultiMap<QString, Options> Section;

/**
 * @brief Charters is wraper of the Section type.
 * @warning This type is depricated. Use the Help:Section type.
 */
typedef Section Charters;

/**
 * @brief width This method return current width of the cosole window.
 * @return width in pxels of the cosole window.
 */
int width();

/**
 * @brief print This method print a one options list.
 * @param oprionsList This is options list.
 */
void QUASARAPPSHARED_EXPORT print(const Options& oprionsList);

/**
 * @brief print This method print all sections of the help.
 * @note This is main method for printing helps.
 * @param help This is sections list.
 */
void QUASARAPPSHARED_EXPORT print(const Section& help);

/**
 * @brief setLineLength sets new length of the help line (width of the console window). If you set this into -1 then the window width will be selected automatically.
 * @param newLength This is a new size of the console window.
 */
void QUASARAPPSHARED_EXPORT setLineLength(int newLength);
}
}

#endif // HELPDATA_H
