#include "treeface/scene/GeometryManager.h"
#include "treeface/base/PackageManager.h"

#include "treeface/misc/Errors.h"

#include "treeface/scene/Geometry.h"

#include <treecore/HashMap.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/Result.h>
#include <treecore/ScopedPointer.h>
#include <treecore/String.h>

using namespace treecore;

namespace treeface {

struct GeometryManager::Impl
{
    HashMap<Identifier, RefCountHolder<Geometry> > items;
};

GeometryManager::GeometryManager(): m_impl( new Impl() )
{}

GeometryManager::~GeometryManager()
{
    if (m_impl)
        delete m_impl;
}

Geometry* GeometryManager::get_geometry( const treecore::Identifier& name )
{
    if ( m_impl->items.contains( name ) )
        return m_impl->items[name];

    // get raw data from package manager
    MemoryBlock config_src;
    if ( !PackageManager::getInstance()->get_item_data( name, config_src, true ) )
        return nullptr;

    // create geometry object
    var geom_root_node;
    {
        Result json_re = JSON::parse( (const char*) config_src.getData(), geom_root_node );
        if (!json_re)
            throw ConfigParseError( String( "failed to parse geometry JSON content for \"" ) + name.toString() + String( "\":\n" ) +
                                    json_re.getErrorMessage() + String( "\n" ) +
                                    (const char*) config_src.getData() );
    }

    Geometry* result = new Geometry( geom_root_node );
    m_impl->items.set( name, result );
    return result;
}

bool GeometryManager::geometry_is_cached( const treecore::Identifier& name ) const noexcept
{
    return m_impl->items.contains( name );
}

bool GeometryManager::release_geometry_hold( const treecore::Identifier& name )
{
    if ( m_impl->items.contains( name ) )
    {
        m_impl->items.remove( name );
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace treeface
