
#pragma once

template<typename T> class REF
{
public:
    T* internal_data = nullptr;

    size_t* refs = nullptr;

    REF() = default;

    REF(T* type);

    constexpr REF<T>& operator=(const REF<T>& other);

    ~REF();
};
