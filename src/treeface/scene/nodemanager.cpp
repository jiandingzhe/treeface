#include "treeface/scene/nodemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/material.h"
#include "treeface/scene/materialmanager.h"
#include "treeface/scene/node.h"
#include "treeface/scene/visualitem.h"

#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/private/node_private.h"

#include <treejuce/CriticalSection.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/HashMap.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct NodeManager::Impl
{
    Holder<GeometryManager> geo_mgr;
    Holder<MaterialManager> mat_mgr;

    HashMap<String, Holder<Node> > nodes;
};

NodeManager::NodeManager(GeometryManager* geo_mgr, MaterialManager* mat_mgr)
    : m_impl(new Impl())
{
    m_impl->geo_mgr = geo_mgr;
    m_impl->mat_mgr = mat_mgr;
}

#define KEY_VISUAL_MAT "material"
#define KEY_VISUAL_GEO "geometry"
Result _validate_visual_item_(const treejuce::NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_VISUAL_MAT, PropertyValidator::ITEM_SCALAR, true);
        validator->add_item(KEY_VISUAL_GEO, PropertyValidator::ITEM_SCALAR, true);
    }

    return validator->validate(kv);
}

treejuce::Result NodeManager::build_visual_item(const var &data, VisualItem *visual_item)
{
    // validate node
    if (!data.isObject())
        return Result::fail("visual item node is not KV");

    const NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = _validate_visual_item_(data_kv);
        if (!re)
            return re;
    }

    // get material and geometry
    String name_mat  = data_kv[KEY_VISUAL_MAT].toString();
    Material* mat = nullptr;
    Result mat_re = m_impl->mat_mgr->get_material(name_mat, &mat);
    if (!mat_re)
        return Result::fail("failed to build visual item:\n" + mat_re.getErrorMessage());

    String name_geom = data_kv[KEY_VISUAL_GEO].toString();
    Geometry* geom = nullptr;
    Result geom_re = m_impl->geo_mgr->get_geometry(name_geom, &geom);
    if (!geom_re)
        return Result::fail("failed to build visual item:\n" + geom_re.getErrorMessage());

    // do build
    return visual_item->build(geom, mat);
}

#define KEY_TRANS "transform"
#define KEY_CHILD "children"
#define KEY_VISUAL "visual_items"

Result _validate_node_(const NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_TRANS, PropertyValidator::ITEM_ARRAY, false);
        validator->add_item(KEY_CHILD, PropertyValidator::ITEM_ARRAY, false);
        validator->add_item(KEY_VISUAL, PropertyValidator::ITEM_ARRAY, false);
    }
    return validator->validate(kv);
}

treejuce::Result NodeManager::build_node(const treejuce::var& data, Node* node)
{
    if (!data.isObject())
        return Result::fail("root node for Node is not KV");

    const NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = _validate_node_(data_kv);
        if (!re)
            return Result::fail("node data validation failed:\n" + re.getErrorMessage());
    }

    //
    // node transform
    //
    if (data_kv.contains(KEY_TRANS))
    {
        const Array<var>* trans_array = data_kv[KEY_TRANS].getArray();
        if (trans_array->size() != 16)
            return Result::fail("transform is not an array of 16 numbers");

        Mat4f mat(float(trans_array->getReference(0)), float(trans_array->getReference(1)), float(trans_array->getReference(2)), float(trans_array->getReference(3)),
                  float(trans_array->getReference(4)), float(trans_array->getReference(5)), float(trans_array->getReference(6)), float(trans_array->getReference(7)),
                  float(trans_array->getReference(8)), float(trans_array->getReference(9)), float(trans_array->getReference(10)), float(trans_array->getReference(11)),
                  float(trans_array->getReference(12)), float(trans_array->getReference(13)), float(trans_array->getReference(14)), float(trans_array->getReference(15)));
        node->m_impl->trans = mat;
        node->m_impl->trans_dirty = true;
    }

    //
    // visual items
    //
    if (data_kv.contains(KEY_VISUAL))
    {
        const Array<var>* visual_array = data_kv[KEY_VISUAL].getArray();
        for (int i = 0; i < visual_array->size(); i++)
        {
            VisualItem* item = new VisualItem();
            Result re = build_visual_item(visual_array->getReference(i), item);
            if (!re)
                return re;
            node->m_impl->visual_items.add(item);
        }
    }

    //
    // child nodes
    //
    if (data_kv.contains(KEY_CHILD))
    {
        const Array<var>* child_array = data_kv[KEY_CHILD].getArray();
        for (int i = 0; i < child_array->size(); i++)
        {
            Node* child = new Node();
            Result re = build_node(child_array->getReference(i), child);
            if (!re)
                return re;

            child->m_impl->parent = node;
            child->m_impl->global_dirty = true;
            node->m_impl->child_nodes.add(child);
        }
    }

    return Result::ok();
}

TREEFACE_NAMESPACE_END
