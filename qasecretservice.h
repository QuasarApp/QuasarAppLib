/*
 * Copyright (C) 2024-2026 QuasarApp.
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
    /**
     * @brief QASecretService This is the default constructor of the QASecretService object.
     */
    QASecretService();

#ifdef Q_OS_LINUX

    /**
     * @brief getByAlias This method returns the secret data by alias from the QASecretService tool.
     * @param alias This is the alias of the required secret.
     * @return secret data of the @a alias or an empty QByteArray if the secret was not found.
     */
    static QByteArray getByAlias(const QString& alias);
    /**
     * @brief getByHash This method returns the secret data by hash from the QASecretService tool.
     * @param hash This is the hash of the required secret.
     * @return secret data of the @a hash or an empty QByteArray if the secret was not found.
     */
    static QByteArray getByHash(const QByteArray& hash);
#endif
};

}
#endif // QASECRETSERVICE_H
