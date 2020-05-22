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

template <class IntegerType>
constexpr IntegerType static_mul(const char* str, unsigned char depth, unsigned char index = 0) noexcept {
    return (str[index] && (depth > index))? str[index] * static_mul<IntegerType>(str, depth, index + 1) : 1;
}

template <class IntegerType>
constexpr IntegerType static_hash(const char* str, unsigned char index = 0) noexcept {
    return (index && index % sizeof(IntegerType) == 0)?
                static_hash<IntegerType>(str, index + 1) % static_mul<IntegerType>(str, index , index - sizeof(IntegerType)):
                (str[index])?
                    static_hash<IntegerType>(str, index + 1):
                    static_mul<IntegerType>(str, index, index - (index % sizeof(IntegerType)));
}

template<class T>
u_int64_t static_type_hash_64() noexcept {
    return static_hash<u_int64_t>(typeid (T).name());
};

template<class T>
u_int32_t static_type_hash_32() noexcept {
    return typeid (T).hash_code();
};

template<class T>
u_int16_t static_type_hash_16() noexcept {
    return typeid (T).hash_code() % 0xFFFF;
};

template<class T>
u_int8_t static_type_hash_8() noexcept {
    return typeid (T).hash_code() % 0xFF;
};

#define H_8 static_type_hash_8
#define H_16 static_type_hash_16
#define H_32 static_type_hash_32

#endif // GLOBAL_H
