
#pragma once

#include <cstdarg>
#include <cstddef>
#include <limits>

#include "load_save_requirements.hpp"

template <VAR T>
inline constexpr T get_lowest(const size_t& count, T low, ...)
{
    T lowest = std::numeric_limits<T>::max();

    va_list args;
    va_start(args, low);

    for (size_t x = 0; x < count; ++x)
    {
        T new_low = va_arg(args, T);

        if (new_low < lowest) lowest = new_low;
    }

    va_end(args);

    return lowest;
}


template <VAR T>
inline constexpr T get_largest(const size_t& count, T large, ...)
{
    T largest = -std::numeric_limits<T>::max();

    va_list args;
    
    va_start(args, large);

    for (size_t x = 0; x < count; ++x)
    {
        T new_large = va_arg(args, T);

        if (new_large > largest) largest = new_large;
    }

    va_end(args);

    return largest;
}