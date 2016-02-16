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

VisualObject::VisualObject( Geometry* geom, SceneGraphMaterial* mat ): m_impl( new Impl( geom, mat ) )
{}

VisualObject::~VisualObject()
{
    if (m_impl)
        delete m_impl;
}

void VisualObject::set_uniform_value( const treecore::Identifier& name, const UniversalValue& value )
{
    m_impl->uniforms.set( name, value );
    m_impl->uniform_cache_dirty = true;
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
