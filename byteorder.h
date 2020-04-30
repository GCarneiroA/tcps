#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <cstdio>
#include <type_traits>

template<class T, unsigned I>
struct ByteOrder
{
    enum
    {
        R_SHIFT = (I - 1) * 8,
        L_SHIFT = (sizeof(T) - I) * 8
    };
    constexpr static auto swap(T x) -> T
    {
        return (((x >> R_SHIFT) & 0xff) << L_SHIFT) | ByteOrder<T, I - 1>::swap(x);
    }
};

template<class T>
struct ByteOrder<T, 0u>
{
    constexpr static auto swap(T x) -> T
    {
        return 0;
    }
};

template<class T>
constexpr auto swap_bytes(T x) -> T
{
    static_assert(std::is_integral<T>::value, "");
    return ByteOrder<T, sizeof(T)>::swap(x);
}

#endif
