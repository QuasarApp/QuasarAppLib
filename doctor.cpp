/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include <ipill.h>

#include "doctor.h"

namespace QuasarAppUtils {

Doctor::Doctor(const QList<QSharedPointer<iPill> > &base) {
    _pillsData = base;
}

void Doctor::diagnostic(bool fix) const {

    QList<QSharedPointer<iPill> > failed;
    QList<QSharedPointer<iPill> > detected;
    QList<QSharedPointer<iPill> > fixedSuccessful;

    for (const auto &pill: _pillsData) {
        if (pill->diagnostic()) {
            if (fix) {
                if (!pill->fix()) {
                    failed.push_back(pill);
                } else {
                    fixedSuccessful.push_back(pill);
                }
            } else {
                detected.push_back(pill);
            }
        }
    }

    if (failed.size()) {
        emit sigFixesFailed(failed);
    }

    if (detected.count()) {
        emit sigTroubleDetected(detected);
    }

    if (fixedSuccessful.count()) {
        emit sigFixesFinishedSuccessful(fixedSuccessful);
    }
}

void Doctor::fix(const QList<QSharedPointer<iPill> > &pills) const {

    QList<QSharedPointer<iPill> > failed;
    QList<QSharedPointer<iPill> > fixedSuccessful;

    for (const auto &pill: pills) {
        if (pill->diagnostic()) {
            if (!pill->fix()) {
                failed.push_back(pill);
            } else {
                fixedSuccessful.push_back(pill);
            }
        }
    }

    if (failed.size()) {
        emit sigFixesFailed(failed);
    }

    if (fixedSuccessful.count()) {
        emit sigFixesFinishedSuccessful(fixedSuccessful);
    }
}

void Doctor::addPill(const QSharedPointer<iPill> &pill) {
    _pillsData.push_back(pill);
}

}
