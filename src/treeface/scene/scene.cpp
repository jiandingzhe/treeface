#include "treeface/scene/scene.h"
#include "treeface/guts/scene_guts.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/scenenodemanager.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/nedbase.h"
#include "treeface/imagemanager.h"
#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include <treejuce/DynamicObject.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/MemoryBlock.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Scene::Scene()
    : m_guts(new Guts())
{
    m_guts->geo_mgr = new GeometryManager();
    m_guts->mat_mgr = new MaterialManager();
    m_guts->node_mgr = new SceneNodeManager(m_guts->geo_mgr, m_guts->mat_mgr);

}

Scene::Scene(GeometryManager* geo_mgr, MaterialManager* mat_mgr)
    : m_guts(new Guts())
{
    m_guts->geo_mgr = geo_mgr;
    m_guts->mat_mgr = mat_mgr;
    m_guts->node_mgr = new SceneNodeManager(m_guts->geo_mgr, m_guts->mat_mgr);
}

Scene::~Scene()
{
    if (m_guts)
        delete m_guts;
}

#define KEY_GLOBAL_LIGHT_DIRECTION "global_light_direction"
#define KEY_GLOBAL_LIGHT_COLOR     "global_light_color"
#define KEY_GLOBAL_LIGHT_AMB       "global_light_ambient"
#define KEY_NODES                  "nodes"

class ScenePropertyValidator: public PropertyValidator
{
public:
    ScenePropertyValidator()
    {
        add_item(KEY_GLOBAL_LIGHT_DIRECTION, PropertyValidator::ITEM_ARRAY, false);
        add_item(KEY_GLOBAL_LIGHT_COLOR, PropertyValidator::ITEM_ARRAY, false);
        add_item(KEY_GLOBAL_LIGHT_AMB, PropertyValidator::ITEM_ARRAY, false);
        add_item(KEY_NODES, PropertyValidator::ITEM_ARRAY, true);
    }

    virtual ~ScenePropertyValidator() {}

    juce_DeclareSingleton(ScenePropertyValidator, false)
};

juce_ImplementSingleton(ScenePropertyValidator)

treejuce::Result Scene::build(const treejuce::String& name) NOEXCEPT
{
    // get JSON source
    MemoryBlock json_src;
    {
        Result re = PackageManager::getInstance()->get_item_data(name, json_src);
        if (!re)
            return re;
    }

    // parse JSON
    var json_data;
    {
        Result re = JSON::parse((char*) json_src.getData(), json_data);
        if (!re)
            return re;
    }

    // build scene from JSON content
    {
        Result re = build(json_data);
        if (!re)
            return Result::fail("failed to build scene using JSON content "+name+"\n"+
                                re.getErrorMessage() + "\n\n" +
                                String("==== JSON source ====\n\n")+
                                (const char*) json_src.getData() + String("\n") +
                                String("==== end of JSON source ====\n"));
    }

    return Result::ok();
}

treejuce::Result Scene::build(const treejuce::var& root) NOEXCEPT
{
    //
    // validate root node
    //
    if (!root.isObject())
        return Result::fail("Scene: root node is not KV");

    const NamedValueSet& root_kv = root.getDynamicObject()->getProperties();
    {
        Result re = ScenePropertyValidator::getInstance()->validate(root_kv);
        if (!re)
            return Result::fail("Scene: property validation failed\n"+
                                re.getErrorMessage());
    }

    //
    // load properties
    //

    // direction of global light
    if (root_kv.contains(KEY_GLOBAL_LIGHT_DIRECTION))
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_DIRECTION].getArray();
        if (values->size() != 4)
            return Result::fail("Scene: global light direction is not an array of 4 values");
        m_guts->global_light_direction.set(float(values->getReference(0)),
                                           float(values->getReference(1)),
                                           float(values->getReference(2)),
                                           float(values->getReference(3))
                                           );
    }

    // color of global light
    if (root_kv.contains(KEY_GLOBAL_LIGHT_COLOR))
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_COLOR].getArray();
        if (values->size() != 4)
            return Result::fail("Scene: global light color is not an array of 4 values");
        m_guts->global_light_color.set(float(values->getReference(0)),
                                       float(values->getReference(1)),
                                       float(values->getReference(2)),
                                       float(values->getReference(3))
                                       );
    }

    // global light intensities
    if (root_kv.contains(KEY_GLOBAL_LIGHT_AMB))
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_AMB].getArray();
        if (values->size() != 4)
            return Result::fail("Scene: global light ambient is not an array of 4 values");
        m_guts->global_light_ambient.set(float(values->getReference(0)),
                                       float(values->getReference(1)),
                                       float(values->getReference(2)),
                                       float(values->getReference(3))
                                       );
    }

    // scene graph
    Array<var>* scenenode_nodes = root_kv[KEY_NODES].getArray();
    for (int i = 0; i < scenenode_nodes->size(); i++)
    {
        const var& scenenode_data = scenenode_nodes->getReference(i);
        SceneNode* node = nullptr;

        Result re = m_guts->node_mgr->add_nodes(scenenode_data, &node);
        if (!re)
            return re;

        m_guts->root_node->add_child(node);
    }

    return Result::ok();
}

SceneNode* Scene::get_root_node() NOEXCEPT
{
    return m_guts->root_node.get();
}

SceneNode* Scene::get_node(const treejuce::String& name) NOEXCEPT
{
    return m_guts->node_mgr->get_node(name);
}

GeometryManager* Scene::get_geometry_manager() NOEXCEPT
{
    return m_guts->geo_mgr.get();
}

MaterialManager* Scene::get_material_manager() NOEXCEPT
{
    return m_guts->mat_mgr.get();
}

const Vec4f& Scene::get_global_light_color() const NOEXCEPT
{
    return m_guts->global_light_color;
}

void Scene::set_global_light_color(float r, float g, float b, float a) NOEXCEPT
{
    m_guts->global_light_color.set(r, g, b, a);
}

void Scene::set_global_light_color(const Vec4f& value) NOEXCEPT
{
    m_guts->global_light_color = value;
}

const Vec4f& Scene::get_global_light_direction() const NOEXCEPT
{
    return m_guts->global_light_direction;
}

void Scene::set_global_light_direction(float x, float y, float z) NOEXCEPT
{
    m_guts->global_light_direction.set(x, y, z, 0);
    m_guts->global_light_direction.normalize();
}

void Scene::set_global_light_direction(const Vec4f& value) NOEXCEPT
{
    m_guts->global_light_direction = value;
    m_guts->global_light_direction.normalize();
}

const Vec4f& Scene::get_global_light_ambient() const NOEXCEPT
{
    return m_guts->global_light_ambient;
}

void Scene::set_global_light_ambient(float r, float g, float b, float a) NOEXCEPT
{
    m_guts->global_light_ambient.set(r, g, b, a);
}

void Scene::set_global_light_ambient(const Vec4f& value) NOEXCEPT
{
    m_guts->global_light_ambient = value;
}

TREEFACE_NAMESPACE_END
