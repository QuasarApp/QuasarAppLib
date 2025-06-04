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


/*!
 * \brief Simplified subtraction operation for QHash.
 *
 * This function provides a convenient way to remove elements from one `QHash`
 * based on keys present in another `QHash`. It's designed to
 * simplify common `QHash` operations.
 *
 * The function modifies the `left` QHash by removing all key-value pairs
 * whose keys are present in the `right` QHash.
 *
 * If `left` and `right` share the same internal data (i.e., `left.isSharedWith(right)`
 * returns true), the `left` hash is cleared entirely to prevent
 * unpredictable behavior or unintended modifications to the shared data.
 * Otherwise, it iterates through `right` and removes corresponding keys from `left`.
 *
 * \tparam QHashType The type of the QHash (e.g., QHash<QString, int>).
 * \param left A reference to the QHash to be modified. Elements present in `right`
 * will be removed from this hash.
 * \param right A constant reference to the QHash containing keys to be removed from `left`.
 * \return A reference to the modified `left` QHash.
 */
template <class QHashType>
QHashType &subtract(QHashType &left, const QHashType& right) {
    if (left.isSharedWith(right)) {
        left.clear();
    } else {
        for (auto&& it =  right.begin(); it != right.end(); ++it)
            left.remove(it.key());
    }
    return left;
}

/*!
 * \brief Simplified union operation for QHash.
 *
 * This function provides a convenient way to unite the elements of two `QHash` objects,
 * adding all elements from one to the other. It's designed to
 * simplify common `QHash` operations.
 *
 * The function modifies the `left` QHash by inserting all key-value pairs
 * from the `right` QHash. If a key from `right` already exists in `left`,
 * its corresponding value in `left` will be updated with the value from `right`.
 *
 * This operation is skipped if `left` and `right` share the same internal data
 * (i.e., `left.isSharedWith(right)` returns true) to avoid self-modification
 * or infinite loops in cases of shared data.
 *
 * \tparam QHashType The type of the QHash (e.g., QHash<QString, int>).
 * \param left A reference to the QHash to be modified. Elements from `right`
 * will be inserted into this hash.
 * \param right A constant reference to the QHash whose elements will be added to `left`.
 * \return A reference to the modified `left` QHash.
 */
template <class QHashType>
QHashType &unite(QHashType &left, const QHashType& right) {
    if (!left.isSharedWith(right)) {
        for (auto&& it =  right.begin(); it != right.end(); ++it)
            left.insert(it.key(), it.value());
    }
    return left;
}


#endif // GLOBAL_H
