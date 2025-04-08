
#pragma once

#include <cstddef>

template<typename T> class ref
{
public:
    T* internal_data = nullptr;

    size_t* refs = nullptr;

    ref() = default;

    ref(T* type);

    constexpr ref<T>& operator=(const ref<T>& other);

    ~ref();

    T operator *();
};
