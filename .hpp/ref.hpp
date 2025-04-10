
#pragma once

#include <cstddef>

template<typename T> struct ref
{
public:
    T* internal_data = nullptr;

    size_t* refs = nullptr;

    ref() = default;

    ref(T* type);

    T* operator->();

    constexpr ref<T>& operator=(const ref<T>& other);

    ~ref();

    T operator *();
};
