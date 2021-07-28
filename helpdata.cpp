/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "helpdata.h"
#include <iostream>

#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#endif

namespace QuasarAppUtils {
namespace Help {

static int MAX_LENGTH = -1;
static int SectionMargin = 2;

#define EXPANDER(COUNT, ITEM) QString(COUNT, ITEM).toStdString()
#define SPACES(X) EXPANDER(X, ' ')
#define SECTION_MARGIN SPACES(SectionMargin)
#define WIDTH ((MAX_LENGTH > 10)? MAX_LENGTH: width())


/*
 * @brief print This method prints the one line of the help.
 * @param key This is Option name.
 * @param value This is Description of option.
 * @param keyLength  This is length of the current line.
 * This is private method of the QuasarAppLibrary.
 */
void print(const QString& key, const QString& value, int keyLength) {

    auto diffExpander = QString(keyLength - key.size(), ' ');
    std::cout << SECTION_MARGIN << key.toStdString() << diffExpander.toStdString() << ":";

    QString expander(keyLength + SectionMargin, ' ');
    auto words = value.split(" ");

    int currentLength = std::max(keyLength, key.size());
    for (const auto& word : words) {
        if (currentLength + 2 + word.size() < WIDTH) {
            std::cout << " " << word.toStdString();
            currentLength += 2 + word.size();

        } else {
            std::cout << std::endl << expander.toStdString() << ":";
            currentLength = keyLength;

            std::cout << " " << word.toStdString();
            currentLength += 2 + word.size();
        }
    }
}

void print(const QuasarAppUtils::Help::Options &oprionsList) {
    int maxLength = 0;
    for (auto line = oprionsList.begin(); line != oprionsList.end(); ++line) {
        if (line.key().size() > maxLength)
            maxLength = line.key().size();
    }

    maxLength = std::min(WIDTH / 3, maxLength);

    for (auto line = oprionsList.begin(); line != oprionsList.end(); ++line) {
        print(line.key(), line.value(), maxLength + SectionMargin);
        std::cout << std::endl;
    }
}

void print(const Section &help) {
    for (auto line = help.begin(); line != help.end(); ++line) {
        QString expander(WIDTH, '-');

        std::cout << line.key().toStdString() << std::endl;
        std::cout << expander.toStdString() << std::endl;
        print(line.value());
        std::cout << std::endl << expander.toStdString() << std::endl;
    }
}

void setLineLength(int newLength) {
    MAX_LENGTH = newLength;
}

int width() {

#ifdef Q_OS_WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}


}

}
