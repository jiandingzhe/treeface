#include "treeface/scene/scenenodemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/visualitem.h"

#include "treeface/gl/vertexarray.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/private/node_private.h"

#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include <treejuce/CriticalSection.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/HashMap.h>
#include <treejuce/JSON.h>
#include <treejuce/MemoryBlock.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct SceneNodeManager::Impl
{
    Holder<GeometryManager> geo_mgr;
    Holder<MaterialManager> mat_mgr;

    HashMap<String, Holder<SceneNode> > nodes;
};

SceneNodeManager::SceneNodeManager(GeometryManager* geo_mgr, MaterialManager* mat_mgr)
    : m_impl(new Impl())
{
    m_impl->geo_mgr = geo_mgr;
    m_impl->mat_mgr = mat_mgr;
}

SceneNodeManager::~SceneNodeManager()
{
    if (m_impl)
        delete m_impl;
}

treejuce::Result SceneNodeManager::add_nodes(const treejuce::var& data)
{
    SceneNode* root_node = new SceneNode();
    return build_node(data, root_node);
}

treejuce::Result SceneNodeManager::add_nodes(const treejuce::String& data_name)
{
    MemoryBlock json_src;
    Result item_re = PackageManager::getInstance()->get_item_data(data_name, json_src);
    if (!item_re)
        return Result::fail("NodeManager: failed to get nodes:\n" +
                            item_re.getErrorMessage());

    var data;
    Result json_re = JSON::parse((char*)json_src.getData(), data);
    if (!json_re)
        return Result::fail("NodeManager: failed to parse JSON content:\n" +
                            json_re.getErrorMessage() + "\n\n" +
                            String("==== JSON source ====\n\n") +
                            String((char*)json_src.getData()) + "\n" +
                            String("==== end of JSON source ====\n")
                            );

    return add_nodes(data);
}

SceneNode* SceneNodeManager::get_node(const treejuce::String& name)
{
    if (m_impl->nodes.contains(name))
    {
        return m_impl->nodes[name].get();
    }

    return nullptr;
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

treejuce::Result SceneNodeManager::build_visual_item(const var &data, VisualItem *visual_item)
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
    return visual_item->build(geom, dynamic_cast<SceneGraphMaterial*>(mat));
}

#define KEY_ID     "id"
#define KEY_TRANS  "transform"
#define KEY_CHILD  "children"
#define KEY_VISUAL "visual_items"

Result _validate_node_(const NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_ID,     PropertyValidator::ITEM_SCALAR, false);
        validator->add_item(KEY_TRANS,  PropertyValidator::ITEM_ARRAY,  false);
        validator->add_item(KEY_CHILD,  PropertyValidator::ITEM_ARRAY,  false);
        validator->add_item(KEY_VISUAL, PropertyValidator::ITEM_ARRAY,  false);
    }
    return validator->validate(kv);
}

treejuce::Result SceneNodeManager::build_node(const treejuce::var& data, SceneNode* node)
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
            SceneNode* child = new SceneNode();
            Result re = build_node(child_array->getReference(i), child);
            if (!re)
                return re;

            child->m_impl->parent = node;
            child->m_impl->global_dirty = true;
            node->m_impl->child_nodes.add(child);
        }
    }

    //
    // if has ID, store it
    //
    if (data_kv.contains(KEY_ID))
    {
        m_impl->nodes.set(data_kv[KEY_ID].toString(), node);
    }

    return Result::ok();
}

TREEFACE_NAMESPACE_END
