#include "treeface/scene/visualitem.h"
#include "treeface/scene/material.h"
#include "treeface/scene/geometry.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include <treejuce/DynamicObject.h>
#include <treejuce/Holder.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct VisualItem::Impl
{
    Holder<Material> material = nullptr;
    Holder<Geometry> geometry = nullptr;
    Holder<VertexArray> vertex_array = nullptr;
};

VisualItem::VisualItem(): m_impl(new Impl())
{
}

VisualItem::~VisualItem()
{
    if (m_impl)
        delete m_impl;
}


treejuce::Result VisualItem::build(Geometry* geom, Material* mat) NOEXCEPT
{
    m_impl->geometry = geom;
    m_impl->material = mat;
    m_impl->vertex_array = new VertexArray();

    return m_impl->vertex_array->build(geom->get_buffer(), geom->get_vertex_template(), mat->get_program());
}

Material* VisualItem::get_material() NOEXCEPT
{
    return m_impl->material;
}

Geometry* VisualItem::get_geometry() NOEXCEPT
{
    return m_impl->geometry;
}

VertexArray* VisualItem::get_vertex_array() NOEXCEPT
{
    return m_impl->vertex_array;
}

TREEFACE_NAMESPACE_END
