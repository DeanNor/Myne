
#include "ref.hpp"

template <typename T> REF<T>::REF(T* type)
{
    internal_data = type;
    refs = new size_t(0);
}

template <typename T> constexpr REF<T>::REF<T>& operator=(const REF<T>& other)
{
    internal_data = other.internal_data;

    refs = other.refs;

    ++*refs;

    return *this;
}

template <typename T> REF<T>::~REF()
{
    if (*refs == 0)
    {
        delete refs;
        delete internal_data;
    }

    else
    {
        --*refs;
    }
}
