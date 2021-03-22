/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <cstdlib>
#include <type_traits>
#include <string>
#include <typeinfo>


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


// TO DO need to testing. I am think this is does not work.
//template <class IntegerType>
//constexpr IntegerType static_mul(const char* str, unsigned char depth, unsigned char index = 0) noexcept {
//    return (str[index] && (depth > index))? str[index] * static_mul<IntegerType>(str, depth, index + 1) : 1;
//}

//template <class IntegerType>
//constexpr IntegerType static_hash(const char* str, unsigned char index = 0) noexcept {
//    return (index && index % sizeof(IntegerType) == 0)?
//                static_hash<IntegerType>(str, index + 1) % static_mul<IntegerType>(str, index , index - sizeof(IntegerType)):
//                (str[index])?
//                    static_hash<IntegerType>(str, index + 1):
//                    static_mul<IntegerType>(str, index, index - (index % sizeof(IntegerType)));
//}

//template<class T>
//uint64_t static_type_hash_64() noexcept {
//    return static_hash<uint64_t>(typeid (T).name());
//};

template<class T>
uint32_t static_type_hash_32() noexcept {
    return typeid (T).hash_code();
};

template<class T>
uint16_t static_type_hash_16() noexcept {
    return typeid (T).hash_code() % 0xFFFF;
};

template<class T>
uint8_t static_type_hash_8() noexcept {
    return typeid (T).hash_code() % 0xFF;
};

template<class T>
uint32_t static_type_hash_32(T& object) noexcept {
    return typeid (object).hash_code();
};

template<class T>
uint16_t static_type_hash_16(T& object) noexcept {
    return typeid (object).hash_code() % 0xFFFF;
};

template<class T>
uint8_t static_type_hash_8(T& object) noexcept {
    return typeid (object).hash_code() % 0xFF;
};

#define H_8 static_type_hash_8
#define H_16 static_type_hash_16
#define H_32 static_type_hash_32


#ifdef RELEASE_BUILD
    // The debug_assert it is assert that abort application only in debug mode.
    // In the release mode This assert prin Error message only.
    #define debug_assert(condition, msg) \
        if (!condition) \
            QuasarAppUtils::Params::log(msg, QuasarAppUtils::Error);

#else
    // The debug_assert it is assert that abort application only in debug mode.
    // In the release mode This assert prin Error message only.
    #define debug_assert(condition, msg) assert(condition && msg)
#endif

#endif // GLOBAL_H
