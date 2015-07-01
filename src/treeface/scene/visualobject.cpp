#include "treeface/scene/visualobject.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include <treejuce/DynamicObject.h>
#include <treejuce/Holder.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

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


treejuce::Result VisualObject::build(Geometry* geom, SceneGraphMaterial* mat) NOEXCEPT
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

TREEFACE_NAMESPACE_END
