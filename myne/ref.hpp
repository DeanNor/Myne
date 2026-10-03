
#pragma once

// TODO do I really need this?

#include <cstddef>

template<typename T> struct ref
{
public:
    T* internal_data = nullptr;

    size_t* refs = 0;

    ref() = default;

    ref(T* type);

    T* operator->();

    ref<T>& operator=(const ref<T>& other);

    ~ref();

    T operator *();
};
