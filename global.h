#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdlib>
#include <type_traits>

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

#endif // GLOBAL_H
