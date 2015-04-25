#include "treeface/misc/propertyvalidator.h"

#include <treejuce/Array.h>
#include <treejuce/HashMap.h>
#include <treejuce/Identifier.h>
#include <treejuce/String.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct PropertyValidator::Impl
{
    Array<Identifier> required_keys;
    HashMap<String, uint32> key_types;
};

PropertyValidator::PropertyValidator()
    : m_impl(new Impl())
{

}

PropertyValidator::~PropertyValidator()
{
    if (m_impl)
        delete m_impl;
}

void PropertyValidator::add_item(const treejuce::String& key, treejuce::uint32 type, bool required)
{
    if (m_impl->key_types.contains(key))
        return;

    m_impl->key_types.set(key, type);

    if (required)
        m_impl->required_keys.add(Identifier(key));
}

treejuce::Result PropertyValidator::validate(const treejuce::NamedValueSet& kv) const NOEXCEPT
{
    // check if all required keys exist
    for (int i = 0; i < m_impl->required_keys.size(); i++)
    {
        const Identifier& key = m_impl->required_keys.getReference(i);
        if (!kv.contains(key))
            return Result::fail("required property \""+key.toString()+"\" is not found");
    }

    // check all keys
    for (int i = 0; i < kv.size(); i++)
    {
        Identifier key = kv.getName(i);
        const String& key_str = key.toString();

        if (!m_impl->key_types.contains(key_str))
            return Result::fail("unknown property \""+key_str+"\"");

        uint32 type = m_impl->key_types[key_str];

        const var& value = kv.getValueAt(i);
        if (value.isArray() && !(type & ITEM_ARRAY))
            return Result::fail("property \""+key_str+"\" is array, which is unexpected");
        else if (value.isObject() && !(type & ITEM_HASH))
            return Result::fail("property \""+key_str+"\" is object, which is unexpected");
        else if ((value.isBool() || value.isInt() || value.isDouble() || value.isUndefined())
                 &&
                 !(type & ITEM_SCALAR))
            return Result::fail("property \""+key_str+"\" is scalar, which is unexpected");
    }
    return Result::ok();
}

TREEFACE_NAMESPACE_END
