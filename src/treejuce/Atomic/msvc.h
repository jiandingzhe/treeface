#ifndef TREEFACE_ATOMIC_MSVC_H
#define TREEFACE_ATOMIC_MSVC_H

#include "treeface/atomic/template.h"

TREEFACE_NAMESPACE_BEGIN

#error "MSVC atomic not implemented"

template<typename T, int SZ>
struct _msvc_atomic_impl_ {};

template<typename T>
struct _msvc_atomic_impl_<T, 4>
{
    T load(T* store) noexcept {}

    void store(T* store, T value) noexcept {}

    T fetch_set(T* store, T value) noexcept {}

    // get value before modify

    T fetch_add(T* store, T value) noexcept {}

    T fetch_sub(T* store, T value) noexcept {}

    T fetch_or(T* store, T value) noexcept {}

    T fetch_and(T* store, T value) noexcept {}

    T fetch_xor(T* store, T value) noexcept {}

    T fetch_nand(T* store, T value) noexcept {}

    // get modified value
    T add_fetch(T* store, T value) noexcept {}

    T sub_fetch(T* store, T value) noexcept {}

    T or_fetch(T* store, T value) noexcept {}

    T and_fetch(T* store, T value) noexcept {}

    T xor_fetch(T* store, T value) noexcept {}

    T nand_fetch(T* store, T value) noexcept {}

    // cas
    bool cas(T* store, T expect, T value) noexcept {}
};

template<typename T>
struct _msvc_atomic_impl_<T, 8>
{
    T load(T* store) noexcept {}

    void store(T* store, T value) noexcept {}

    T fetch_set(T* store, T value) noexcept {}

    // get value before modify

    T fetch_add(T* store, T value) noexcept {}

    T fetch_sub(T* store, T value) noexcept {}

    T fetch_or(T* store, T value) noexcept {}

    T fetch_and(T* store, T value) noexcept {}

    T fetch_xor(T* store, T value) noexcept {}

    T fetch_nand(T* store, T value) noexcept {}

    // get modified value
    T add_fetch(T* store, T value) noexcept {}

    T sub_fetch(T* store, T value) noexcept {}

    T or_fetch(T* store, T value) noexcept {}

    T and_fetch(T* store, T value) noexcept {}

    T xor_fetch(T* store, T value) noexcept {}

    T nand_fetch(T* store, T value) noexcept {}

    // cas
    bool cas(T* store, T expect, T value) noexcept {}
};


// get and set
template<typename T>
inline T atomic_load(T* store) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::load(store);
}

template<typename T>
void atomic_store(T* store, T value) noexcept
{
    _msvc_atomic_impl_<T, sizeof(T)>::store(store, value);
}


template<typename T>
inline T atomic_fetch_set(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_set(store, value);
}

// get value before modify
template<typename T>
inline T atomic_fetch_add(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_add(store, value);
}

template<typename T>
inline T atomic_fetch_sub(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_sub(store, value);
}

template<typename T>
inline T atomic_fetch_or(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_or(store, value);
}

template<typename T>
inline T atomic_fetch_and(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_and(store, value);
}

template<typename T>
inline T atomic_fetch_xor(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_xor(store, value);
}

template<typename T>
inline T atomic_fetch_nand(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_nand(store, value);
}

// get modified value
template<typename T>
inline T atomic_add_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::add_fetch(store_value);
}

template<typename T>
inline T atomic_sub_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::sub_fetch(store, value);
}

template<typename T>
inline T atomic_or_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::or_fetch(store, value);
}

template<typename T>
inline T atomic_and_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::and_fetch(store, value);
}

template<typename T>
inline T atomic_xor_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::xor_fetch(store, value);
}

template<typename T>
inline T atomic_nand_fetch(T* store, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::nand_fetch(store_value);
}

// cas
template<typename T>
inline bool atomic_cas(T* store, T expect, T value) noexcept
{
    return _msvc_atomic_impl_<T, sizeof(T)>::cas(store, except, value);
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_ATOMIC_MSVC_H
