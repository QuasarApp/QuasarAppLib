#include "qasecretservice.h"

#ifdef Q_OS_LINUX
#include <QProcess>

QByteArray QuasarAppUtils::QASecretService::getByAlias(const QString &alias) {
    QProcess proc;
    proc.setProgram("qasecretservice");
    proc.setArguments(QStringList() << "get" << "-alias" << alias);

    proc.start();

    if (!proc.waitForFinished(2000)) {
        return "";
    }

    if (proc.exitCode()) {
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

    if (!proc.waitForFinished(2000)) {
        return "";
    }

    if (proc.exitCode()) {
        return "";
    }

    QByteArray result = proc.readAllStandardOutput();

    // drop \n
    return result.left(result.size() - 1);
}
#endif
