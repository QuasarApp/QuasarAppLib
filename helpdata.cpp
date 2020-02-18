#include "helpdata.h"
#include <iostream>

int MAX_LENGTH = 80;
void QuasarAppUtils::Help::print(const QuasarAppUtils::Help::Options &charter) {
    for (auto line = charter.begin(); line != charter.end(); ++line) {
        print(line.key(), line.value());
    }
}

void QuasarAppUtils::Help::print(const QuasarAppUtils::Help::Charters &help) {
    for (auto line = help.begin(); line != help.end(); ++line) {
        std::string expander('-', MAX_LENGTH);

        std::cout << line.key().toStdString()<< std::endl;
        std::cout << expander << std::endl;
        print(line.value());
        std::cout << expander << std::endl;
    }
}

void QuasarAppUtils::Help::print(const QString &key, const QString &value) {

    int keyLength = key.size() + 2;
    std::cout << key.toStdString() << ": ";

    std::string expander(' ', keyLength);
    auto words = value.split(" ");

    int currentLength = keyLength;
    for (const auto& word : words) {
        if (currentLength < MAX_LENGTH) {
            std::cout << word.toStdString();
            currentLength += 2 + word.size();
        } else {
            std::cout << std::endl << expander << ": ";
            currentLength = keyLength;
        }
    }
}

void QuasarAppUtils::Help::setLineLength(int newLength) {
    MAX_LENGTH = newLength;
}
