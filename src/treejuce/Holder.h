#ifndef TREEFACE_OJBECT_HOLDER_H
#define TREEFACE_OJBECT_HOLDER_H

#include "treeface/common.h"

class TestFramework;

TREEFACE_NAMESPACE_BEGIN

template<typename T>
class Holder
{
    friend class ::TestFramework;

    template<typename T1, typename T2>
    friend bool operator == (const Holder<T1>& a, const Holder<T2>& b);

    template<typename T1, typename T2>
    friend bool operator < (const Holder<T1>& a, const Holder<T2>& b);

public:
    Holder() {}

    Holder(const Holder& other): ms_ptr(other.ms_ptr)
    {
        if (ms_ptr)
            smart_ref(ms_ptr);
    }

    Holder(Holder&& other): ms_ptr(other.ms_ptr)
    {
        other.ms_ptr = nullptr;
    }

    Holder(T* other): ms_ptr(other)
    {
        if (ms_ptr)
            smart_ref(ms_ptr);
    }

    ~Holder()
    {
        if (ms_ptr)
            smart_unref(ms_ptr);
    }

    Holder& operator = (const Holder& other)
    {
        if (ms_ptr)
            smart_unref(ms_ptr);

        ms_ptr = other.ms_ptr;

        if (ms_ptr)
            smart_ref(ms_ptr);

        return *this;
    }

    Holder& operator = (const T* other)
    {
        if (ms_ptr)
            smart_unref(ms_ptr);

        ms_ptr = other;

        if(ms_ptr)
            smart_ref(ms_ptr);

        return *this;
    }

    operator bool () const
    {
        return ms_ptr != nullptr;
    }

    T* get() const
    {
        return ms_ptr;
    }

    T& operator * () const
    {
        return *ms_ptr;
    }

    T* operator -> () const
    {
        return ms_ptr;
    }

private:
    T* ms_ptr = nullptr;
}; // class ObjectHolder

template<typename T1, typename T2>
bool operator == (const Holder<T1>& a, const Holder<T2>& b)
{
    return a.ms_ptr == b.ms_ptr;
}

template<typename T1, typename T2>
bool operator < (const Holder<T1>& a, const Holder<T2>& b)
{
    return a.ms_ptr < b.ms_ptr;
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_OJBECT_HOLDER_H
