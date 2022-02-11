/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "qaglobalutils.h"

template<class NUM>
void gen(int size, QByteArray &result) {
    for (; size >= 0; size -= sizeof(NUM)) {
        NUM random = (rand() * rand()) % std::numeric_limits<NUM>::max();
        result.insert(0, reinterpret_cast<char*>(&random), sizeof(random));
    }
}

void randomArray(int size, QByteArray &result) {
    if (size % sizeof(unsigned long long) == 0) {
        gen<unsigned long long>(size, result);
    } else if (size % sizeof(unsigned int) == 0) {
        gen<unsigned int>(size, result);
    } else if (size % sizeof(unsigned short) == 0) {
        gen<unsigned short>(size, result);
    } else {
        gen<unsigned char>(size, result);
    }
}
