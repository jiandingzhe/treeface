#ifndef TREEFACE_NED_OBJECT_H
#define TREEFACE_NED_OBJECT_H

#include "treeface/common.h"

#include <treejuce/StandardHeader.h>
#include <treejuce/IntTypes.h>

#include <cstddef>
#include <new>

TREEFACE_NAMESPACE_BEGIN

class NedBase
{
public:
    void* operator new(std::size_t size);
    void* operator new(std::size_t size, const std::nothrow_t& nothrow_value) NOEXCEPT;
    void* operator new(std::size_t size, void* ptr) NOEXCEPT
    {
        jassert(treejuce::pointer_sized_int(ptr) % 32 == 0);
        return ptr;
    }

    void operator delete(void* ptr);
    void operator delete(void* ptr, const std::nothrow_t& tag);
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_NED_OBJECT_H
