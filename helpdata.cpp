#include "helpdata.h"
#include <iostream>

static int MAX_LENGTH = 80;
static int SectionMargin = 3;

#define EXPANDER(COUNT, ITEM) QString(COUNT, ITEM).toStdString()
#define SPACES(X) EXPANDER(X, ' ')
#define SECTION_MARGIN SPACES(SectionMargin)

void QuasarAppUtils::Help::print(const QuasarAppUtils::Help::Options &charter) {
    int maxLength = 0;
    for (auto line = charter.begin(); line != charter.end(); ++line) {
        if (line.key().size() > maxLength)
            maxLength = line.key().size();
    }

    for (auto line = charter.begin(); line != charter.end(); ++line) {
        print(line.key(), line.value(), maxLength + SectionMargin);
        std::cout << std::endl;
    }
}

void QuasarAppUtils::Help::print(const QuasarAppUtils::Help::Charters &help) {
    for (auto line = help.begin(); line != help.end(); ++line) {
        QString expander(MAX_LENGTH, '-');

        std::cout << line.key().toStdString()<< std::endl;
        std::cout << expander.toStdString() << std::endl;
        print(line.value());
        std::cout << std::endl << expander.toStdString() << std::endl;
    }
}

void QuasarAppUtils::Help::print(const QString &key, const QString &value, int keyLength) {

    auto diffExpander = QString(keyLength - key.size(), ' ');
    std::cout << SECTION_MARGIN << key.toStdString() << diffExpander.toStdString() << ":";

    QString expander(keyLength + SectionMargin, ' ');
    auto words = value.split(" ");

    int currentLength = keyLength;
    for (const auto& word : words) {
        if (currentLength + 2 + word.size() < MAX_LENGTH) {
            std::cout << " " << word.toStdString();
            currentLength += 2 + word.size();
        } else {
            std::cout << std::endl << expander.toStdString() << ":";
            currentLength = keyLength;
        }
    }
}

void QuasarAppUtils::Help::setLineLength(int newLength) {
    MAX_LENGTH = newLength;
}
