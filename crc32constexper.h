/*
 * Copyright (C) 2025-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef CRC32CONSTEXPER_H
#define CRC32CONSTEXPER_H


#include <array>
#include <cstddef>
#include <cstdint>

namespace QuasarAppUtils {

/**
 * @brief generateCrc32Table This method generate crc32 table for fast calculate crc32 hash.
 * @param polynomial This is polynomial for crc32 hash.
 * @return return table for fast calculate crc32 hash.
 * @note This method is constexpr and available in compile time.
 */
constexpr std::array<uint32_t, 256> generateCrc32Table(uint32_t polynomial = 0xEDB88320) {
    std::array<uint32_t, 256> table{};
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ (-(crc & 1) & polynomial);
        }
        table[i] = crc;
    }
    return table;
}

/**
 * @brief crc32Table This is crc32 table for fast calculate crc32 hash.
 * @note This table is used for fast calculate crc32 hash.
 */
constexpr std::array<uint32_t, 256> crc32Table = generateCrc32Table();

/**
 * @brief calculateCrc32 This method calculate crc32 hash for data.
 * @param data This is data for calculate crc32 hash.
 * @param size This is size of data.
 * @param initialCrc This is initial crc value.
 * @return return crc32 hash for data.
 * @note This method is constexpr and available in compile time.
 */
constexpr uint32_t calculateCrc32(const char* data, size_t size, uint32_t initialCrc = 0xFFFFFFFF) {
    uint32_t crc = initialCrc;
    for (size_t i = 0; i < size; ++i) {
        crc = crc32Table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

/**
 * @brief calculateCrc16 This method calculate crc16 hash for data.
 * @param data This is data for calculate crc16 hash.
 * @param size This is size of data.
 * @param initialCrc This is initial crc value.
 * @return return crc16 hash for data.
 * @note This method is constexpr and available in compile time.
 */
constexpr uint16_t calculateCrc16(const char* data, size_t size, uint32_t initialCrc = 0xFFFFFFFF) {
    return calculateCrc32(data, size, initialCrc) % 0xFFFF;
}

/**
 * @brief calculateCrc8 This method calculate crc8 hash for data.
 * @param data This is data for calculate crc8 hash.
 * @param size This is size of data.
 * @param initialCrc This is initial crc value.
 * @return return crc8 hash for data.
 * @note This method is constexpr and available in compile time.
 */
constexpr uint16_t calculateCrc8(const char* data, size_t size, uint32_t initialCrc = 0xFFFFFFFF) {
    return calculateCrc32(data, size, initialCrc) % 0xFF;
}

#define LITIRAL_CRC32(str) \
    (QuasarAppUtils::calculateCrc32(str, sizeof(str) - 1))
}

#endif // CRC32CONSTEXPER_H
