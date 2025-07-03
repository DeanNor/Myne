
#pragma once

#include <concepts>
#include <type_traits>

template<typename T>
concept VAR = std::is_scalar_v<T>;

template<typename T>
concept CLS = std::is_class_v<T>;

template<typename T>
concept CLS_PTR = CLS<T> && std::is_pointer_v<T>;