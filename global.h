#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdlib>
#include <type_traits>
#include <string>

template <typename T>
constexpr inline T operator | (T lhs, T rhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

template <typename T>
constexpr inline T operator & (T lhs, T rhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

template <typename T>
constexpr inline T operator >> (T lhs, T rhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(static_cast<int>(lhs) >> static_cast<int>(rhs));
}

template <typename T>
constexpr inline T operator << (T lhs, T rhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(static_cast<int>(lhs) << static_cast<int>(rhs));
}

template <typename T>
constexpr inline T operator ~ (T lhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(~static_cast<int>(lhs));
}

template <typename T>
constexpr inline T operator ^ (T lhs, T rhs)
{

    static_assert(std::is_enum<T>::value,
                  "template parameter is not an enum type");

    return static_cast<T>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}


constexpr uint32_t static_hash(const char* str) {
    uint32_t hash = 0;
    unsigned char index = 0;
    uint32_t tmp = 1;
    while (str[index] && index < 0xff) {
        tmp *= str[index];
        if (index % 4 == 0) {
            hash = hash % str[index];
            tmp = 1;
        }
    }
    return hash;
}

#endif // GLOBAL_H
