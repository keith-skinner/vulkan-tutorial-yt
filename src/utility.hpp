# pragma once 
#include <type_traits>
#include <concepts>

template<class T>
requires std::integral<T>
auto to_unsigned(T value) -> std::make_unsigned_t<T>
{
    return static_cast<std::make_unsigned_t<T>>(value);
}

template<class T>
requires std::integral<T>
auto to_signed(T value)
{
    return static_cast<std::make_unsigned_t<T>>(value);
}