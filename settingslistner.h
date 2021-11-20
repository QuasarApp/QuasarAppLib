/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SETTINGSLISTNER_H
#define SETTINGSLISTNER_H

#include <QString>
#include <QVariant>
#include "quasarapp_global.h"


namespace QuasarAppUtils {

/**
 * @brief The SettingsListner class is listner of the ISettings global object.
 * The SettingsListner class is abstrct class and contains only one method for hendling settings changes.
 *
 *
 * ### Example of use :
 *
 * @code{cpp}
 *  class MyClass : protected QuasarAppUtils::SettingsListner {
 *
 *  public:
 *      MyClass(){
 *          handleSettings();
 *      }
 *
 *  protected:
 *      void handleSettingsChanged(const QString& key, const QVariant& value) override {
 *
 *          if (key == "shareName") {
                setSessinon(static_cast<long long >(rand()) * rand());
            }
 *      }
 *
 *  };
 * @endcode
 *
 * @warning For correctly working this handler you should be create a class that inherit of the QObject class else
 *  the SettingsListner::handleSettingsChanged will nit invoked when settings has changed.
 * @see ISettings
 */
class QUASARAPPSHARED_EXPORT SettingsListner
{
public:
    SettingsListner();
    virtual ~SettingsListner();

protected:

    /**
     * @brief handleSettings This method subscribe this class to listning settings changes.
     * @return true if subscribed successful else false.
     * @see SettingsListner::handleSettingsChanged
     */
    bool handleSettings();

    /**
     * @brief handleSettingsChanged This method will be invoked when settings of application has bean changed.
     * @param key This is key of a changed settings.
     * @param value This is a new value of a changed settings.
     */
    virtual void handleSettingsChanged(const QString& key,
                                       const QVariant& value) = 0;
private:
    QMetaObject::Connection _listnerConnection;
};

}
#endif // SETTINGSLISTNER_H
