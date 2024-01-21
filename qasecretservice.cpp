#include "qasecretservice.h"
#include <QProcess>

QByteArray QuasarAppUtils::QASecretService::getByAlias(const QString &alias) {
    QProcess proc;
    proc.setProgram("qasecretservice");
    proc.setArguments(QStringList() << "get" << "-alias" << alias);

    proc.start();

    proc.waitForFinished(2000);

    if (proc.error() || proc.exitCode()) {
        return "";
    }

    QByteArray result = proc.readAllStandardOutput();

    // drop \n
    return result.left(result.size() - 1);
}

QByteArray QuasarAppUtils::QASecretService::getByHash(const QByteArray &hash)
{
    QProcess proc;
    proc.setProgram("qasecretservice");
    proc.setArguments(QStringList() << "get" << "-hash" << hash);

    proc.start();

    proc.waitForFinished(2000);
    QByteArray result = proc.readAllStandardOutput();

    if (proc.error() || proc.exitCode()) {
        return "";
    }

    // drop \n
    return result.left(result.size() - 1);
}
