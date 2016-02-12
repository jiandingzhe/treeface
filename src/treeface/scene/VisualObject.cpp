#include "treeface/scene/guts/VisualObject_guts.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/SceneGraphMaterial.h"

#include "treeface/gl/VertexArray.h"

#include "treeface/misc/PropertyValidator.h"

#include <treecore/DynamicObject.h>
#include <treecore/RefCountHolder.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

VisualObject::VisualObject( Geometry* geom, SceneGraphMaterial* mat ): m_impl( new Impl() )
{
    m_impl->geometry     = geom;
    m_impl->material     = mat;
    m_impl->vertex_array = new VertexArray( geom->get_vertex_buffer(), geom->get_index_buffer(), geom->get_vertex_template(), mat->get_program() );
}

VisualObject::~VisualObject()
{
    if (m_impl)
        delete m_impl;
}

void VisualObject::set_uniform_value( const treecore::Identifier& name, const UniversalValue& value )
{
    m_impl->uniforms.set( name, value );
}

bool VisualObject::get_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept
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

bool VisualObject::has_uniform( const treecore::Identifier& name ) const noexcept
{
    return m_impl->uniforms.contains( name );
}

int32 VisualObject::collect_uniforms( UniformMap& store ) const
{
    UniformMap::Iterator i_result( store );
    int32 num_got = 0;

    for (UniformMap::ConstIterator i( m_impl->uniforms ); i.next(); )
    {
        if ( store.insertOrSelect( i.key(), i.value(), i_result ) )
            num_got++;
    }

    num_got += m_impl->geometry->collect_uniforms( store );

    return num_got;
}

SceneGraphMaterial* VisualObject::get_material() const noexcept
{
    return m_impl->material;
}

Geometry* VisualObject::get_geometry() const noexcept
{
    return m_impl->geometry;
}

VertexArray* VisualObject::get_vertex_array() const noexcept
{
    return m_impl->vertex_array;
}

void VisualObject::render() noexcept
{
    jassert( !m_impl->geometry->is_dirty() );
    m_impl->vertex_array->draw( m_impl->geometry->get_primitive(), m_impl->geometry->get_num_index() );
}

} // namespace treeface
