/*
 * Copyright (C) 2021-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "optiondata.h"

namespace QuasarAppUtils{

OptionData::OptionData(const QStringList& name,
                       const QString& arguments,
                       const QString& description,
                       const QString& example,
                       const QString& depricatedMsg,
                       bool removed) {

    setNames(name);
    setArguments(arguments);
    setDescription(description);
    setExample(example);
    setDepricatedMsg(depricatedMsg);
    _removed = removed;
}

const QStringList &OptionData::names() const {
    return _name;
}

void OptionData::setNames(const QStringList &newName) {
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
    QString left = names().join(" / ") + " " + arguments();

    QString right = description();
    if (example().size()) {
        right += " Example: " + example();
    }

    return {{left, {right}}};
}

bool OptionData::isValid() const {
    return names().size();
}
}
