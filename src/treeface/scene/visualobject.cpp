#include "treeface/scene/visualobject.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/guts/utils.h"

#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include <treecore/DynamicObject.h>
#include <treecore/RefCountHolder.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct VisualObject::Impl
{
    RefCountHolder<SceneGraphMaterial> material     = nullptr;
    RefCountHolder<Geometry>           geometry     = nullptr;
    RefCountHolder<VertexArray>        vertex_array = nullptr;
    UniformMap uniforms;
};

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

SceneGraphMaterial* VisualObject::get_material() noexcept
{
    return m_impl->material;
}

Geometry* VisualObject::get_geometry() noexcept
{
    return m_impl->geometry;
}

VertexArray* VisualObject::get_vertex_array() noexcept
{
    return m_impl->vertex_array;
}

void VisualObject::render() noexcept
{
    jassert( !m_impl->geometry->is_dirty() );
    m_impl->vertex_array->draw( m_impl->geometry->get_primitive(), m_impl->geometry->get_num_index() );
}

} // namespace treeface
