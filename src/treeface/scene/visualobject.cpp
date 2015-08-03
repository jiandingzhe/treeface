#include "treeface/scene/visualobject.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include <treecore/DynamicObject.h>
#include <treecore/Holder.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct VisualObject::Impl
{
    Holder<SceneGraphMaterial> material = nullptr;
    Holder<Geometry> geometry = nullptr;
    Holder<VertexArray> vertex_array = nullptr;
};

VisualObject::VisualObject(): m_impl(new Impl())
{
}

VisualObject::~VisualObject()
{
    if (m_impl)
        delete m_impl;
}


treecore::Result VisualObject::build(Geometry* geom, SceneGraphMaterial* mat) NOEXCEPT
{
    if (!geom)
        return Result::fail("geometry is null while building visual object");
    if (!mat)
        return Result::fail("material is null while building visual object");

    m_impl->geometry = geom;
    m_impl->material = mat;
    m_impl->vertex_array = new VertexArray();

    return m_impl->vertex_array->build(geom->get_buffer(), geom->get_vertex_template(), mat->get_program());
}

SceneGraphMaterial* VisualObject::get_material() NOEXCEPT
{
    return m_impl->material;
}

Geometry* VisualObject::get_geometry() NOEXCEPT
{
    return m_impl->geometry;
}

VertexArray* VisualObject::get_vertex_array() NOEXCEPT
{
    return m_impl->vertex_array;
}

} // namespace treeface
