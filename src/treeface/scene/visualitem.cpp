#include "treeface/scene/visualitem.h"
#include "treeface/scene/material.h"
#include "treeface/scene/geometry.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/geometrymanager.h"
#include "treeface/materialmanager.h"

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

treejuce::Result VisualItem::build(const treejuce::String& geom_name, const treejuce::String& mat_name) NOEXCEPT
{
    Geometry* geom = nullptr;
    Result re_geom = GeometryManager::getInstance()->get_geometry(geom_name, &geom);
    if (!re_geom)
        return re_geom;

    Material* mat = nullptr;
    Result re_mat = MaterialManager::getInstance()->get_material(mat_name, &mat);
    if (!re_mat)
        return re_mat;

    return build(geom, mat);
}

#define KEY_MAT "material"
#define KEY_GEO "geometry"
Result _validate_(const treejuce::NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_MAT, PropertyValidator::ITEM_SCALAR, true);
        validator->add_item(KEY_GEO, PropertyValidator::ITEM_SCALAR, true);
    }

    return validator->validate(kv);
}

treejuce::Result VisualItem::build(const treejuce::var& node) NOEXCEPT
{
    if (!node.isObject())
        return Result::fail("visual item node is not KV");

    const NamedValueSet& node_kv = node.getDynamicObject()->getProperties();
    {
        Result re = _validate_(node_kv);
        if (!re)
            return re;
    }

    String name_mat = node_kv[KEY_MAT].toString();
    String name_geo = node_kv[KEY_GEO].toString();

    return build(name_geo, name_mat);
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
