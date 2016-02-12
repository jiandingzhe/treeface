#include "treeface/scene/Geometry.h"

#include "treeface/gl/Errors.h"
#include "treeface/gl/VertexArray.h"
#include "treeface/gl/VertexAttrib.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"
#include "treeface/misc/StringCast.h"

#include "treeface/scene/guts/Geometry_guts.h"

#include <treecore/Array.h>
#include <treecore/ArrayRef.h>
#include <treecore/CriticalSection.h>
#include <treecore/DynamicObject.h>
#include <treecore/HashSet.h>
#include <treecore/NamedValueSet.h>
#include <treecore/RefCountHolder.h>
#include <treecore/RefCountSingleton.h>
#include <treecore/Result.h>
#include <treecore/ScopedLock.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

Geometry::Geometry( const VertexTemplate& vtx_temp, GLPrimitive primitive, bool is_dynamic )
    : m_impl( new Guts( vtx_temp, primitive, is_dynamic ) )
{}

Geometry::~Geometry()
{
    if (m_impl)
        delete m_impl;
}

#define KEY_ATTR "attributes"
#define KEY_VTX  "vertices"
#define KEY_IDX  "indices"
#define KEY_PRIM "primitive"

class GeometryPropertyValidator: public PropertyValidator, public treecore::RefCountSingleton<GeometryPropertyValidator>
{
public:
    GeometryPropertyValidator()
    {
        add_item( KEY_ATTR, PropertyValidator::ITEM_ARRAY,  true );
        add_item( KEY_VTX,  PropertyValidator::ITEM_ARRAY,  true );
        add_item( KEY_IDX,  PropertyValidator::ITEM_ARRAY,  true );
        add_item( KEY_PRIM, PropertyValidator::ITEM_SCALAR, true );
    }
};

Geometry::Geometry( const treecore::var& geom_root_node )
{
    if ( !geom_root_node.isObject() )
        throw ConfigParseError( "geometry root node is not a object" );

    const NamedValueSet& geom_root_kv = geom_root_node.getDynamicObject()->getProperties();
    {
        Result re = GeometryPropertyValidator::getInstance()->validate( geom_root_kv );
        if (!re)
            throw ConfigParseError( "geometry JSON node is invalid: " + re.getErrorMessage() );
    }

    //
    // build geometry
    //

    // load geometric primitive type
    {
        GLPrimitive primitive;
        if ( !fromString( geom_root_kv[KEY_PRIM], primitive ) )
            throw ConfigParseError( "failed to parse OpenGL primitive enum from: " + geom_root_kv[KEY_PRIM].toString() );

        // load vertex attribute template
        m_impl = new Guts( VertexTemplate( geom_root_kv[KEY_ATTR] ), primitive, false );
    }

    //
    // load vertices
    //
    const Array<var>* vtx_nodes = geom_root_node[KEY_VTX].getArray();
    {
        size_t vtx_size   = m_impl->vtx_temp.vertex_size();
        int    n_vtx_elem = m_impl->vtx_temp.n_elems();
        //        DBG( "geometry vertex size: " + String( vtx_size ) + ", elems: " + String( n_vtx_elem ) );

        m_impl->host_data_vtx.resize( vtx_nodes->size() );
        int8* vtx_data = (int8*) m_impl->host_data_vtx.get_raw_data_ptr();

        for (int i_vtx = 0; i_vtx < vtx_nodes->size(); i_vtx++)
        {
            // get and validate vertex node
            //            DBG( "vertex " + String( i_vtx ) );
            const var& vtx_node = (*vtx_nodes)[i_vtx];

            if ( !vtx_node.isArray() )
                throw ConfigParseError( "vertex node at " + String( i_vtx ) + " is not an array" );

            const Array<var>* vtx_elems = vtx_node.getArray();

            if (vtx_elems->size() != n_vtx_elem)
                throw ConfigParseError( "vertex template specified " + String( n_vtx_elem ) + " elements, but vertex node " + String( i_vtx ) + " has only " + String( vtx_elems->size() ) + " elements" );

            // fill data
            for (int i_elem = 0; i_elem < n_vtx_elem; i_elem++)
            {
                //                DBG( "  vertex element " +  String( i_elem ) + ": " + (*vtx_elems)[i_elem].toString() );
                m_impl->vtx_temp.set_value_at( vtx_data, i_elem, (*vtx_elems)[i_elem] );
            }

            // move forward pointer of current vertex data
            vtx_data += vtx_size;
        }
    }

    //
    // load vertex indices
    //
    const Array<var>* idx_nodes = geom_root_node[KEY_IDX].getArray();

    for (int i_idx = 0; i_idx < idx_nodes->size(); i_idx++)
    {
        int idx = int( (*idx_nodes)[i_idx] );
        if ( idx >= vtx_nodes->size() )
            throw ConfigParseError( "vertex amount is " + String( vtx_nodes->size() ) + ", but got index " + String( idx ) );
        m_impl->host_data_idx.add( idx );
    }

    // mark data change
    mark_dirty();
}

bool Geometry::is_dirty() const noexcept { return m_impl->dirty; }

void Geometry::mark_dirty() noexcept { m_impl->dirty = true; }

GLPrimitive Geometry::get_primitive() const noexcept { return m_impl->primitive; }

int32 Geometry::get_num_index() const noexcept { return m_impl->num_idx; }

const VertexTemplate& Geometry::get_vertex_template() const noexcept
{
    return m_impl->vtx_temp;
}

GLBuffer* Geometry::get_vertex_buffer() noexcept
{
    return m_impl->buf_vtx;
}

GLBuffer* Geometry::get_index_buffer() noexcept
{
    return m_impl->buf_idx;
}

bool Geometry::get_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept
{
    UniformMap::ConstIterator i( m_impl->uniforms );
    if ( m_impl->uniforms.select( name, i ) )
    {
        result = i.value();
        return true;
    }
    else
    {
        return false;
    }
}

void Geometry::set_uniform_value( const treecore::Identifier& name, const UniversalValue& value )
{
    m_impl->uniforms.set( name, value );
}

bool Geometry::has_uniform( const treecore::Identifier& name ) const noexcept
{
    return m_impl->uniforms.contains( name );
}

int32 Geometry::collect_uniforms( UniformMap& store ) const
{
    UniformMap::Iterator i_result( store );
    int32 num_got = 0;

    for (UniformMap::ConstIterator i( m_impl->uniforms ); i.next(); )
    {
        if ( store.insertOrSelect( i.key(), i.value(), i_result ) )
            num_got++;
    }

    return num_got;
}

void Geometry::host_draw_begin()
{
    m_impl->drawing = true;
}

void Geometry::add_vertex_by_ptr( const void* vtx_data )
{
    m_impl->dirty = true;
    m_impl->host_data_vtx.add( vtx_data );
}

void Geometry::add_index( IndexType idx )
{
    m_impl->dirty = true;

}

Geometry::HostVertexCache& Geometry::get_host_vertex_cache() noexcept
{
    jassert( m_impl->drawing );
    return m_impl->host_data_vtx;
}

treecore::Array<IndexType>& Geometry::get_host_index_cache() noexcept
{
    jassert( m_impl->drawing );
    return m_impl->host_data_idx;
}

void Geometry::host_draw_end()
{
    m_impl->drawing = false;
    m_impl->dirty   = true;
}

void Geometry::host_draw_end_no_change()
{
    m_impl->drawing = false;
    m_impl->dirty   = false;
}

void Geometry::upload_data() { m_impl->upload_data(); }

} // namespace treeface
