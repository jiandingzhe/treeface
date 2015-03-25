#ifndef TREEJUCE_ATOMIC_GCC_H
#define TREEJUCE_ATOMIC_GCC_H

#include "treejuce/Atomic/template.h"
#include "treejuce/Common.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

// get and set
template<typename T>
inline T atomic_load(T* store) noexcept
{
    return __atomic_load_n(store, __ATOMIC_SEQ_CST);
}

template<typename T>
inline void atomic_store(T* store, T value) noexcept
{
    __atomic_store_n(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_set(T* store, T value) noexcept
{
    return __atomic_exchange_n(store, value, __ATOMIC_SEQ_CST);
}


// get value before modify
template<typename T>
inline T atomic_fetch_add(T* store, T value) noexcept
{
    return __atomic_fetch_add(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_sub(T* store, T value) noexcept
{
    return __atomic_fetch_sub(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_or(T* store, T value) noexcept
{
    return __atomic_fetch_or(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_and(T* store, T value) noexcept
{
    return __atomic_fetch_and(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_xor(T* store, T value) noexcept
{
    return __atomic_fetch_xor(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_fetch_nand(T* store, T value) noexcept
{
    return __atomic_fetch_nand(store, value, __ATOMIC_SEQ_CST);
}

// get modified value
template<typename T>
inline T atomic_add_fetch(T* store, T value) noexcept
{
    return __atomic_add_fetch(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_sub_fetch(T* store, T value) noexcept
{
    return __atomic_sub_fetch(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_or_fetch(T* store, T value) noexcept
{
    return __atomic_or_fetch(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_and_fetch(T* store, T value) noexcept
{
    return __atomic_and_fetch(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_xor_fetch(T* store, T value) noexcept
{
    return __atomic_xor_fetch(store, value, __ATOMIC_SEQ_CST);
}

template<typename T>
inline T atomic_nand_fetch(T* store, T value) noexcept
{
    return __atomic_nand_fetch(store, value, __ATOMIC_SEQ_CST);
}

// cas
template<typename T>
inline bool atomic_cas(T* store, T expect, T value) noexcept
{
    return __atomic_compare_exchange_n(store, &expect, value, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_ATOMIC_GCC_H
