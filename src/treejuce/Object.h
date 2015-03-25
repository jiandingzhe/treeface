#ifndef TREEFACE_OBJECT_H
#define TREEFACE_OBJECT_H

#include "treeface/common.h"
#include "treeface/atomic.h"

#include <cstdint>

class TestFramework;

TREEFACE_NAMESPACE_BEGIN

class Object
{
    friend class ::TestFramework;
public:
    Object()                    { atomic_fetch_set(&ms_count, 0u); }
    Object(const Object& other) { atomic_fetch_set(&ms_count, 0u); }

    virtual ~Object() {}

    Object& operator = (const Object& other) {}

    void ref() const noexcept
    {
        atomic_fetch_add(&ms_count, 1u);
    }

    void unref() const noexcept
    {
        if (atomic_fetch_sub(&ms_count, 1u) == 1)
            delete this;
    }

    std::uint32_t get_ref_count() const noexcept
    {
        return atomic_fetch(&ms_count);
    }

private:
    mutable std::uint32_t ms_count;
}; // class Object

inline void smart_ref(const Object* obj)
{ obj->ref(); }

inline void smart_unref(const Object* obj)
{ obj->unref(); }

TREEFACE_NAMESPACE_END

#endif // TREEFACE_OBJECT_H
