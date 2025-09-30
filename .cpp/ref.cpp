
#include ".hpp/ref.hpp"

template <typename T>
ref<T>::ref(T* type)
{
    internal_data = type;
    refs = new size_t(0);
}

template <typename T>
T* ref<T>::operator->()
{
    return internal_data;
}

template <typename T>
ref<T>& ref<T>::operator=(const ref<T>& other)
{
    internal_data = other.internal_data;

    refs = other.refs;

    ++*refs;

    return *this;
}

template <typename T> ref<T>::~ref()
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

template <typename T> T ref<T>::operator*()
{
    return *internal_data;
}
