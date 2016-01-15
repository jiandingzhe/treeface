#include "treeface/scene/visualobject.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"

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
    m_impl->vertex_array->draw( m_impl->geometry->get_primitive(), m_impl->geometry->get_num_index() );
}

} // namespace treeface
