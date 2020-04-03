#ifndef HELPDATA_H
#define HELPDATA_H

#include <QMap>
namespace QuasarAppUtils{

namespace Help {
/**
 * @brief Options - this is list of key - descriptions pairs of help.
 */
typedef QMap<QString, QString> Options;

/**
 * @brief Charters = ths is list of charters.
 */
typedef QMap<QString, Options> Charters;

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
