#ifndef TREEFACE_PROPERTY_VALIDATOR
#define TREEFACE_PROPERTY_VALIDATOR

#include "treeface/common.h"

#include <treejuce/IntTypes.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>

TREEFACE_NAMESPACE_BEGIN

class PropertyValidator: public treejuce::Object
{
public:
    typedef enum
    {
        ITEM_SCALAR = 1,
        ITEM_ARRAY = 1<<1,
        ITEM_HASH = 1<<2
    } ItemType;

    PropertyValidator();
    virtual ~PropertyValidator();

    void add_item(const treejuce::String& key, treejuce::uint32 type, bool required);

    treejuce::Result validate(const treejuce::NamedValueSet& kv) const NOEXCEPT;

private:
    struct Impl;
    Impl* m_impl;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_PROPERTY_VALIDATOR
