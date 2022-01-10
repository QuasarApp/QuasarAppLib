/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef DOCTOR_H
#define DOCTOR_H

#include <QSharedPointer>

namespace QuasarAppUtils {

class iPill;
/**
 * @brief The Doctor class is class that execute pills objects.
 *  The Doctor check issue that will be executed and if the issue realy then execute fix.
 * @warning This is dangerous system because you may create bug in pill and doctor that will execute this pill may broke users app.
 * @see iPill
 */
class Doctor: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Doctor This is base contructor of doctor calss.
     * @param base This is list of known issues.
     */
    Doctor(const QList<QSharedPointer<iPill>>& base);

    /**
     * @brief diagnostick This method run full diagnostick of this application.
     *  If the @a fix is true then doctor try fix the foundet issues.
     *  If the @a fix value if false then the Doctor emit the sigTroubleDetected signal.
     * @param fix set this argument to tru if you want fix all foundet issues.
     */
    void diagnostick(bool fix = false) const;

    /**
     * @brief fix
     * @param pill
     */
    void fix(const QList<QSharedPointer<iPill>>& pill) const;
signals:
    /**
     * @brief sigTroubleDetected This signal will emited when The doctor object found issues in this application.
     * @param issues
     * @see Doctor::diagnostick
     */
    void sigTroubleDetected(QList<QSharedPointer<iPill>> issues);

    /**
     * @brief sigFixesFailed This signal emited when the doctor can't fix foundet issues.
     * @param issues This is list of the unfixable issues.
     */
    void sigFixesFailed(QList<QSharedPointer<iPill>> issues);

    /**
     * @brief sigFixesFinishedSuccessful This signal emited when the doctor fix foundet issues successfull.
     * @param issues This is list of the fixed issues.
     */
    void sigFixesFinishedSuccessful(QList<QSharedPointer<iPill>> issues);


private:
    QHash<QString, QSharedPointer<iPill>> _pillsData;
};


}

#endif // DOCTOR_H
