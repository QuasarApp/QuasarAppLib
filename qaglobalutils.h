/*
 * Copyright (C) 2018-2025 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QU_GLOBAL_UTILS_H
#define QU_GLOBAL_UTILS_H

#include <type_traits>
#include <typeinfo>
#include <QByteArray>
#include "QtGlobal"
#include "quasarapp_global.h"

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

template<class T>
/**
 * @brief static_type_hash_32 This function return hash code of the class T.
 *  For get more information see the std [documentation](https://en.cppreference.com/w/cpp/types/type_info/hash_code).
 * @note This method will create a T object on stack, so if you have a alredy created object use the static_type_hash_32(const  T&) function.
 * @return uint32_t hash code of the class T
 */
uint32_t static_type_hash_32() noexcept {
    return typeid (T).hash_code();
};

template<class T>
/**
 * @brief static_type_hash_16 This function return hash code of the class T.
 *  For get more information see the std [documentation](https://en.cppreference.com/w/cpp/types/type_info/hash_code).
 * @note This method will create a T object on stack, so if you have a alredy created object use the static_type_hash_16(const  T&) function.
 * @return uint16_t hash code of the class T
 */
uint16_t static_type_hash_16() noexcept {
    return typeid (T).hash_code() % 0xFFFF;
};

template<class T>
/**
 * @brief static_type_hash_8 This function return hash code of the class T.
 *  For get more information see the std [documentation](https://en.cppreference.com/w/cpp/types/type_info/hash_code).
 * @note This method will create a T object on stack, so if you have a alredy created object use the static_type_hash_8(const  T&) function.
 * @return uint8_t hash code of the class T
 */
uint8_t static_type_hash_8() noexcept {
    return typeid (T).hash_code() % 0xFF;
};

template<class T>
/**
 * @brief static_type_hash_32 This function return hash code of a T type using @a object.
 * @param object This is object of the T type using for generate hash.
 * @return uint32_t hash code of the class T
 */
uint32_t static_type_hash_32(T& object) noexcept {
    return typeid (object).hash_code();
};

template<class T>
/**
 * @brief static_type_hash_16 This function return hash code of a T type using @a object.
 * @param object This is object of the T type using for generate hash.
 * @return uint16_t hash code of the class T
 */
uint16_t static_type_hash_16(T& object) noexcept {
    return typeid (object).hash_code() % 0xFFFF;
};

template<class T>
/**
 * @brief static_type_hash_8 This function return hash code of a T type using @a object.
 * @param object This is object of the T type using for generate hash.
 * @return uint8_t hash code of the class T
 */
uint8_t static_type_hash_8(T& object) noexcept {
    return typeid (object).hash_code() % 0xFF;
};

/// @brief H_8 This is short wraper of the  static_type_hash_8 fucntion.
#define H_8 static_type_hash_8

/// @brief H_16 This is short wraper of the  static_type_hash_16 fucntion.
#define H_16 static_type_hash_16

/// @brief H_32 This is short wraper of the  static_type_hash_32 fucntion.
#define H_32 static_type_hash_32


#define debug_assert(C, M) Q_ASSERT(C && M)

/**
 * @brief randomArray This function return random arrat with size @a size
 * @param size This is size of needed array.
 * @param result This is result value of generated array.
*/
void QUASARAPPSHARED_EXPORT randomArray(int size, QByteArray &result);

#endif // GLOBAL_H
