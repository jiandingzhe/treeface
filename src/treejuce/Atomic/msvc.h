#ifndef TREEFACE_ATOMIC_MSVC_H
#define TREEFACE_ATOMIC_MSVC_H

#include "treejuce/Atomic/template.h"

#include <intrin.h>
#include <cstdlib>

TREEFACE_JUCE_NAMESPACE_BEGIN

template<typename T, int SZ>
struct _msvc_atomic_impl_ {};

template<typename T>
struct _msvc_atomic_impl_<T, 4>
{
    typedef volatile long* store_type;
    typedef long value_type;

    static T load(T* store) NOEXCEPT
    {
        return _InterlockedExchangeAdd(store_type(store), 0);
    }

    static void store(T* store, T value) NOEXCEPT
    {
        _InterlockedExchange(store_type(store), *((value_type*)&value));
    }

    static T fetch_set(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchange( store_type(store), *((value_type*) &value) );
        return *((T*) &re);
    }

    // get value before modify

    static T fetch_add(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchangeAdd( store_type(store), *((value_type*) &value) );
        return *((T*) &re);
    }

    static T fetch_sub(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchangeAdd( store_type(store), -*((value_type*) &value) );
        return *((T*) &re);
    }

    static T fetch_or(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedOr(store_type(store), *((value_type*) &value));
        return *((T*) &re);
    }

    static T fetch_and(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedAnd(store_type(store), *((value_type*) &value));
        return *((T*) &re);
    }

    static T fetch_xor(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedXor(store_type(store), *((value_type*) &value));
        return *((T*) &re);
    }

    static T fetch_nand(T* store, T value) NOEXCEPT
    {
        abort();
    }

    // get modified value
    static T add_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_add(store, value);
        return old + value;
    }

    static T sub_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_sub(store, value);
        return old - value;
    }

    static T or_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_or(store, value);
        return old | value;
    }

    static T and_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_and(store, value);
        return old & value;
    }

    static T xor_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_xor(store, value);
        return old ^ value;
    }

    static T nand_fetch(T* store, T value) NOEXCEPT
    {
        abort();
    }

    // cas
    static bool cas(T* store, T expect, T value) NOEXCEPT
    {
        return (_InterlockedCompareExchange(store_type(store), *((value_type*)&value), *((value_type*)&expect)) == *((value_type*)&expect));
    }
};

template<typename T>
struct _msvc_atomic_impl_<T, 8>
{
    typedef volatile long long* store_type;
    typedef long long value_type;

    static T load(T* store) NOEXCEPT
    {
        value_type re = _InterlockedExchangeAdd64(store_type(store), 0);
        return *((T*)&re);
    }

    static void store(T* store, T value) NOEXCEPT
    {
        _InterlockedExchange64(store_type(store), *((value_type*)&value));
    }

    static T fetch_set(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchange64(store_type(store), *((value_type*)&value));
        return *((T*)&re);
    }

    // get value before modify
    static T fetch_add(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchangeAdd64(store_type(store), *((value_type*)&value));
        return *((T*)&re);
    }

    static T fetch_sub(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedExchangeAdd64(store_type(store), -*((value_type*)&value));
        return *((T*)&re);
    }

    static T fetch_or(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedOr64(store_type(store), *((value_type*)&value));
        return *((T*)&re);
    }

    static T fetch_and(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedAnd64(store_type(store), *((value_type*)&value));
        return *((T*)&re);
    }

    static T fetch_xor(T* store, T value) NOEXCEPT
    {
        value_type re = _InterlockedXor64(store_type(store), *((value_type*)&value));
        return *((T*)&re);
    }

    static T fetch_nand(T* store, T value) NOEXCEPT
    {
        abort();
    }

    // get modified value
    static T add_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_add(store, value);
        return old + value;
    }

    static T sub_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_sub(store, value);
        return old - value;
    }

    static T or_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_or(store, value);
        return old | value;
    }

    static T and_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_and(store, value);
        return old & value;
    }

    static T xor_fetch(T* store, T value) NOEXCEPT
    {
        T old = fetch_xor(store, value);
        return old ^ value;
    }

    static T nand_fetch(T* store, T value) NOEXCEPT
    {
        abort();
    }

    // cas
    static bool cas(T* store, T expect, T value) NOEXCEPT
    {
        return (_InterlockedCompareExchange64(store_type(store), *((value_type*)&value), *((value_type*)&expect)) == *((value_type*)&expect));
    }
};


// get and set
template<typename T>
inline T atomic_load(T* store) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::load(store);
}

template<typename T>
void atomic_store(T* store, T value) NOEXCEPT
{
    _msvc_atomic_impl_<T, sizeof(T)>::store(store, value);
}


template<typename T>
inline T atomic_fetch_set(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_set(store, value);
}

// get value before modify
template<typename T>
inline T atomic_fetch_add(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_add(store, value);
}

template<typename T>
inline T atomic_fetch_sub(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_sub(store, value);
}

template<typename T>
inline T atomic_fetch_or(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_or(store, value);
}

template<typename T>
inline T atomic_fetch_and(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_and(store, value);
}

template<typename T>
inline T atomic_fetch_xor(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_xor(store, value);
}

template<typename T>
inline T atomic_fetch_nand(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::fetch_nand(store, value);
}

// get modified value
template<typename T>
inline T atomic_add_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::add_fetch(store_value);
}

template<typename T>
inline T atomic_sub_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::sub_fetch(store, value);
}

template<typename T>
inline T atomic_or_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::or_fetch(store, value);
}

template<typename T>
inline T atomic_and_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::and_fetch(store, value);
}

template<typename T>
inline T atomic_xor_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::xor_fetch(store, value);
}

template<typename T>
inline T atomic_nand_fetch(T* store, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::nand_fetch(store_value);
}

// cas
template<typename T>
inline bool atomic_cas(T* store, T expect, T value) NOEXCEPT
{
    return _msvc_atomic_impl_<T, sizeof(T)>::cas(store, expect, value);
}

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEFACE_ATOMIC_MSVC_H
