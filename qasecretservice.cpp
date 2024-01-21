#include "qasecretservice.h"
#include <QProcess>

QByteArray QuasarAppUtils::QASecretService::getByAlias(const QString &alias) {
    QProcess proc;
    proc.setProgram("qasecretservice");
    proc.setArguments(QStringList() << "get" << "-alias" << alias);

    proc.start();

    proc.waitForFinished(2000);

    return proc.readAllStandardOutput();
}

QByteArray QuasarAppUtils::QASecretService::getByHash(const QByteArray &hash)
{
    QProcess proc;
    proc.setProgram("qasecretservice");
    proc.setArguments(QStringList() << "get" << "-hash" << hash);

    proc.start();

    proc.waitForFinished(2000);

    return proc.readAllStandardOutput();
}
