/*
 * Copyright (C) 2021-2023 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef OPTIONDATA_H
#define OPTIONDATA_H

#include "quasarapp_global.h"
#include "helpdata.h"

namespace QuasarAppUtils{

/**
 * @brief The OptionData class contains information about one option.
 */
class QUASARAPPSHARED_EXPORT OptionData
{
public:

    /**
     * @brief OptionData This is main constructor
     * @param names This is names list of the option. It is a required argument and cannot be empty.
     * @param arguments This is input arguments of this option or help meesage about arguments.
     * @param description This is description message of this option.
     * @param example This is example of use string.
     * @param depricatedMsg This is a message that will be printed as a warning if user will use this option. If you set this argument to empty value then warning message will be ignored and option not be marked asa a depricated. An option will be marked as a depricated when this arguments will not equal empty string.
     * @param removed This option show depricatedMsg as a error and force the parseParams method return false. This option will be ignored if the depricatedMsg will be empty.
     */
    OptionData(const QStringList& names,
               const QString& arguments = "",
               const QString& description = "",
               const QString& example = "",
               const QString& depricatedMsg = "",
               bool removed = false);

    /**
     * @brief name This is name of the option. It is a required argument and cannot be empty.
     * @return return name of this option.
     */
    const QStringList &names() const;

    /**
     * @brief description  This is description message of this option.
     * @return return description of this option.
     */
    const QString &description() const;

    /**
     * @brief example This is example of use string.
     * @return return example if use of this option.
     */
    const QString &example() const;

    /**
     * @brief arguments This is input arguments of this option or help meesage about arguments.
     * @return return arguments of this option.
     */
    const QString &arguments() const;

    /**
     * @brief depricatedWarning This is a message that will be printed as a warning if user will use this option. If you set this argument to empty value then warning message will be ignored and option not be marked asa a depricated. An option will be marked as a depricated when this arguments will not equal empty string.
     * @note if this value is empty the option are not depricated.
     * @return warning meessage of the depricated options.
     */
    const QString &depricatedMsg() const;

    /**
     * @brief toHelp This method convert this option into help line.
     * The help line have next structure :
     *
     * Key: {name [arguments]}
     * Value: {description examples}
     * @return The Help::Options set with one line.
     */
    Help::Options toHelp() const;

    /**
     * @brief isValid This method return true if the option is valid. If option do not contain a name then it is invalid.
     * @return true if the option valid else false.
     */
    bool isValid() const;

    /**
     * @brief isRemoved This method return true if the option is removed.
     * @return true if option is removed else false.
     */
    bool isRemoved() const;

    /**
     * @brief isDepricated This method return true if this option is depricated.
     * @return true if options is depricated else false
     */
    bool isDepricated() const;

protected:
    /**
     * @brief setNames This method sets new value of the option name.
     * @param newNames This is a new value of the options name.
     * @note see the OptionData::name method.
     */
    void setNames(const QStringList &newNames);

    /**
     * @brief setDescription This method sets new description of this options.
     * @param newDescription This is new value of optons description
     * @note See the OptionData::description method.

     */
    void setDescription(const QString &newDescription);

    /**
     * @brief setExample This method sets a new example value of this option
     * @param newExample This is a new value of the options example.
     * @note See the OptionData::example method.

     */
    void setExample(const QString &newExample);

    /**
     * @brief setArguments This method sets new value of the options arguments
     * @param newArguments new value of the options arguments
     * @note See the OptionData::arguments method.

     */
    void setArguments(const QString &newArguments);

    /**
     * @brief setDepricatedMsg This method sets new value of the depricated warnning message of this option.
     * @param newDepricatedMsg This is new value of the depricated message.
     * @note See the OptionData::depricatedMsg method.
     */
    void setDepricatedMsg(const QString &newDepricatedMsg);

private:
    QStringList _name;
    QString _description;
    QString _example;
    QString _arguments;
    QString _depricatedMsg;
    bool _removed;
};

/**
 * @brief OptionsList is Hash map of the OptionData items. Where the key it is group name and value it is option data.
 */
typedef QMultiHash<QString, OptionData> OptionsDataList;
}
#endif // OPTIONDATA_H
