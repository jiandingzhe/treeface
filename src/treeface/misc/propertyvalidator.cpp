#include "treeface/misc/propertyvalidator.h"

#include <treecore/Array.h>
#include <treecore/HashMap.h>
#include <treecore/Identifier.h>
#include <treecore/String.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct PropertyValidator::Impl
{
    Array<Identifier> required_keys;
    HashMap<Identifier, uint32> key_types;
};

PropertyValidator::PropertyValidator()
    : m_impl( new Impl() )
{}

PropertyValidator::~PropertyValidator()
{
    if (m_impl)
        delete m_impl;
}

void PropertyValidator::add_item( const treecore::String& key, treecore::uint32 type, bool required )
{
    if ( m_impl->key_types.contains( key ) )
        return;

    m_impl->key_types.set( key, type );

    if (required)
        m_impl->required_keys.add( Identifier( key ) );
}

treecore::Result PropertyValidator::validate( const treecore::NamedValueSet& kv ) const noexcept
{
    // check if all required keys exist
    for (int i = 0; i < m_impl->required_keys.size(); i++)
    {
        const Identifier& key = m_impl->required_keys[i];
        if ( !kv.contains( key ) )
            return Result::fail( "required property \"" + key.toString() + "\" (" + String( pointer_sized_int( key.getPtr() ) ) + ") is not found" );
    }

    // check all keys
    treecore::NamedValueSet::MapType::ConstIterator i_kv( kv.getValues() );
    while ( i_kv.next() )
    {
        Identifier    key     = i_kv.key();
        const String& key_str = key.toString();

        if ( !m_impl->key_types.contains( key_str ) )
            return Result::fail( "unknown property \"" + key_str + "\"" );

        uint32 type = m_impl->key_types[key_str];

        const var& value = i_kv.value();
        if ( value.isArray() )
        {
            if ( !(type & ITEM_ARRAY) )
                return Result::fail( "property \"" + key_str + "\" is array, which is unexpected" );
        }
        else if ( value.isObject() )
        {
            if ( !(type & ITEM_HASH) )
                return Result::fail( "property \"" + key_str + "\" is object, which is unexpected" );
        }
        else if (value.isBool()      ||
                 value.isInt()       ||
                 value.isDouble()    ||
                 value.isUndefined() ||
                 value.isString()
        )
        {
            if ( !(type & ITEM_SCALAR) )
                return Result::fail( "property \"" + key_str + "\" is scalar, which is unexpected" );
        }
    }
    return Result::ok();
}

} // namespace treeface
