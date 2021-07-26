/*
 * Copyright (C) 2021-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "optiondata.h"
#include "global.h"

namespace QuasarAppUtils{

OptionData::OptionData(const QString& name,
                       const QString& arguments,
                       const QString& description,
                       const QString& example,
                       const QString& depricatedMsg,
                       bool removed) {

    setName(name);
    setArguments(arguments);
    setDescription(description);
    setExample(example);
    setDepricatedMsg(depricatedMsg);
    _removed = removed;
}

const QString &OptionData::name() const {
    return _name;
}

void OptionData::setName(const QString &newName) {
    _name = newName;
}

const QString &OptionData::description() const {
    return _description;
}

void OptionData::setDescription(const QString &newDescription) {
    _description = newDescription;
}

const QString &OptionData::example() const {
    return _example;
}

void OptionData::setExample(const QString &newExample) {
    _example = newExample;
}

const QString &OptionData::arguments() const {
    return _arguments;
}

void OptionData::setArguments(const QString &newArguments) {
    _arguments = newArguments;
}

const QString &OptionData::depricatedMsg() const {
    return _depricatedMsg;
}

void OptionData::setDepricatedMsg(const QString &newDepricatedMsg) {
    _depricatedMsg = newDepricatedMsg;
}

bool OptionData::isRemoved() const {
    return _removed;
}

bool OptionData::isDepricated() const {
    return depricatedMsg().size();
}

Help::Options OptionData::toHelp() const {
    return {{{name() + " " + arguments()}, {description() + " " + example()}}};
}
}
