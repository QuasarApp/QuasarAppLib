/*
 * Copyright (C) 2024-2024 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QASECRETSERVICE_H
#define QASECRETSERVICE_H

#include <QByteArray>


namespace QuasarAppUtils {

/**
 * @brief The qasecretservice class This is a simeple provider of QASecretService tool
 * See https://github.com/QuasarApp/SecretService
 * @note Before use this class please install QASecretService to your runtime mashine.
 *
 */
class QASecretService
{
public:
    QASecretService();

    static QByteArray getByAlias(const QString& alias);
    static QByteArray getByHash(const QByteArray& hash);

};

}
#endif // QASECRETSERVICE_H
