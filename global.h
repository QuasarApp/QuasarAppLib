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


template<class T, class Hash>
/**
 * @brief static_type_hash_base
 * @return hash of type (on compiller time)
 */
constexpr Hash static_type_hash_base() {

    auto name = typeid(T).name();

    Hash hash = 0;
    unsigned char index = 0;
    Hash tmp = 1;
    while (name[index] && index < 0xff) {
        tmp *= name[index];
        if (index % sizeof(Hash) == 0) {
            hash = hash % name[index];
            tmp = 1;
        }
    }
    return hash;
}

template<class T>
constexpr uint64_t static_type_hash_64() {
    return static_type_hash_base<T, uint64_t>();
};

template<class T>
constexpr uint32_t static_type_hash_32() {
    return static_type_hash_base<T, uint32_t>();
};

template<class T>
constexpr uint16_t static_type_hash_16() {
    return static_type_hash_base<T, uint16_t>();
};

template<class T>
constexpr unsigned char static_type_hash_8() {
    return static_type_hash_base<T, unsigned char>();
};


#define H_8 static_type_hash_8
#define H_16 static_type_hash_16
#define H_32 static_type_hash_32
#define H_64 static_type_hash_64

#endif // GLOBAL_H
