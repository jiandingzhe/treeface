#include "treeface/nedbase.h"

#include "nedmalloc.h"

TREEFACE_NAMESPACE_BEGIN

void* NedBase::operator new(std::size_t size)
{
    void* ptr = nedalloc::nedmalloc2(size, 32);
    if (!ptr)
        throw std::bad_alloc();
    return ptr;
}

void* NedBase::operator new(std::size_t size, const std::nothrow_t& nothrow_value) NOEXCEPT
{
    void* ptr = nedalloc::nedmalloc2(size, 32);
    return ptr;
}

void NedBase::operator delete(void* ptr)
{
    nedalloc::nedfree2(ptr);
}

void NedBase::operator delete(void* ptr, const std::nothrow_t& tag)
{
    nedalloc::nedfree2(ptr);
}

TREEFACE_NAMESPACE_END
