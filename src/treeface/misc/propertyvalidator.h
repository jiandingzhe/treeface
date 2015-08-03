#ifndef TREEFACE_PROPERTY_VALIDATOR
#define TREEFACE_PROPERTY_VALIDATOR

#include "treeface/common.h"

#include <treecore/IntTypes.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Object.h>
#include <treecore/Result.h>

namespace treeface {

class PropertyValidator: public treecore::Object
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

    void add_item(const treecore::String& key, treecore::uint32 type, bool required);

    treecore::Result validate(const treecore::NamedValueSet& kv) const NOEXCEPT;

private:
    struct Impl;
    Impl* m_impl;
};

} // namespace treeface

#endif // TREEFACE_PROPERTY_VALIDATOR
