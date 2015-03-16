#ifndef TREEFACE_ATOMIC_GCC_H
#define TREEFACE_ATOMIC_GCC_H

#include "treeface/atomic/template.h"

TREEFACE_NAMESPACE_BEGIN

// get and set
template<typename T>
inline T atomic_fetch(T* store) noexcept
{
    return __sync_fetch_and_add(store, 0);
}

template<typename T>
inline T atomic_fetch_set(T* store, T value) noexcept
{
    return __sync_lock_test_and_set(store, value);
}


// get value before modify
template<typename T>
inline T atomic_fetch_add(T* store, T value) noexcept
{
    return __sync_fetch_and_add(store, value);
}

template<typename T>
inline T atomic_fetch_sub(T* store, T value) noexcept
{
    return __sync_fetch_and_sub(store, value);
}

template<typename T>
inline T atomic_fetch_or(T* store, T value) noexcept
{
    return __sync_fetch_and_or(store, value);
}

template<typename T>
inline T atomic_fetch_and(T* store, T value) noexcept
{
    return __sync_fetch_and_and(store, value);
}

template<typename T>
inline T atomic_fetch_xor(T* store, T value) noexcept
{
    return __sync_fetch_and_xor(store, value);
}

template<typename T>
inline T atomic_fetch_nand(T* store, T value) noexcept
{
    return __sync_fetch_and_nand(store, value);
}

// get modified value
template<typename T>
inline T atomic_add_fetch(T* store, T value) noexcept
{
    return __sync_add_and_fetch(store, value);
}

template<typename T>
inline T atomic_sub_fetch(T* store, T value) noexcept
{
    return __sync_sub_and_fetch(store, value);
}

template<typename T>
inline T atomic_or_fetch(T* store, T value) noexcept
{
    return __sync_or_and_fetch(store, value);
}

template<typename T>
inline T atomic_and_fetch(T* store, T value) noexcept
{
    return __sync_and_and_fetch(store, value);
}

template<typename T>
inline T atomic_xor_fetch(T* store, T value) noexcept
{
    return __sync_xor_and_fetch(store, value);
}

template<typename T>
inline T atomic_nand_fetch(T* store, T value) noexcept
{
    return __sync_nand_and_fetch(store, value);
}

// cas
template<typename T>
inline bool atomic_cas_bool(T* store, T expect, T value) noexcept
{
    return __sync_bool_compare_and_swap(store, expect, value);
}

template<typename T>
inline T atomic_cas_value(T* store, T expect, T value) noexcept
{
    return __sync_val_compare_and_swap(store, expect, value);
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_ATOMIC_GCC_H
