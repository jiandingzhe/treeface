#include "treeface/scene/Scene.h"
#include "treeface/scene/guts/Scene_guts.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/GeometryManager.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/SceneNodeManager.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"

#include "treeface/base/PackageManager.h"

#include <treecore/AlignedMalloc.h>
#include "treeface/graphics/ImageManager.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/base/PackageManager.h"

#include <treecore/DynamicObject.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

Scene::Scene( GeometryManager* geo_mgr, MaterialManager* mat_mgr )
    : m_guts( new Guts() )
{
    m_guts->geo_mgr  = geo_mgr != nullptr ? geo_mgr : new GeometryManager();
    m_guts->mat_mgr  = mat_mgr != nullptr ? mat_mgr : new MaterialManager();
    m_guts->node_mgr = new SceneNodeManager( m_guts->geo_mgr, m_guts->mat_mgr );
}

Scene::Scene( const treecore::var& root,    GeometryManager* geo_mgr, MaterialManager* mat_mgr )
    : m_guts( new Guts() )
{
    m_guts->geo_mgr  = geo_mgr != nullptr ? geo_mgr : new GeometryManager();
    m_guts->mat_mgr  = mat_mgr != nullptr ? mat_mgr : new MaterialManager();
    m_guts->node_mgr = new SceneNodeManager( m_guts->geo_mgr, m_guts->mat_mgr );

    build( root );
}

Scene::Scene( const treecore::String& data_name, GeometryManager* geo_mgr, MaterialManager* mat_mgr ): m_guts( new Guts() )
{
    m_guts->geo_mgr  = geo_mgr != nullptr ? geo_mgr : new GeometryManager();
    m_guts->mat_mgr  = mat_mgr != nullptr ? mat_mgr : new MaterialManager();
    m_guts->node_mgr = new SceneNodeManager( m_guts->geo_mgr, m_guts->mat_mgr );

    var data_root = PackageManager::getInstance()->get_item_json( data_name );
    if (!data_root)
        throw ConfigParseError( "no package item named \"" + data_name + "\"" );

    build( data_root );
}

Scene::~Scene()
{
    if (m_guts)
        delete m_guts;
}

SceneNode* Scene::get_root_node() noexcept
{
    return m_guts->root_node.get();
}

SceneNode* Scene::get_node( const treecore::String& name ) noexcept
{
    return m_guts->node_mgr->get_node( name );
}

GeometryManager* Scene::get_geometry_manager() noexcept
{
    return m_guts->geo_mgr.get();
}

MaterialManager* Scene::get_material_manager() noexcept
{
    return m_guts->mat_mgr.get();
}

const Vec4f& Scene::get_global_light_color() const noexcept
{
    return m_guts->global_light_color;
}

void Scene::set_global_light_color( float r, float g, float b, float a ) noexcept
{
    m_guts->global_light_color.set( r, g, b, a );
}

void Scene::set_global_light_color( const Vec4f& value ) noexcept
{
    m_guts->global_light_color = value;
}

const Vec4f& Scene::get_global_light_direction() const noexcept
{
    return m_guts->global_light_direction;
}

void Scene::set_global_light_direction( float x, float y, float z ) noexcept
{
    m_guts->global_light_direction.set( x, y, z, 0 );
    m_guts->global_light_direction.normalize();
}

void Scene::set_global_light_direction( const Vec4f& value ) noexcept
{
    m_guts->global_light_direction = value;
    m_guts->global_light_direction.normalize();
}

const Vec4f& Scene::get_global_light_ambient() const noexcept
{
    return m_guts->global_light_ambient;
}

void Scene::set_global_light_ambient( float r, float g, float b, float a ) noexcept
{
    m_guts->global_light_ambient.set( r, g, b, a );
}

void Scene::set_global_light_ambient( const Vec4f& value ) noexcept
{
    m_guts->global_light_ambient = value;
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
        add_item( KEY_GLOBAL_LIGHT_DIRECTION, PropertyValidator::ITEM_ARRAY, false );
        add_item( KEY_GLOBAL_LIGHT_COLOR,     PropertyValidator::ITEM_ARRAY, false );
        add_item( KEY_GLOBAL_LIGHT_AMB,       PropertyValidator::ITEM_ARRAY, false );
        add_item( KEY_NODES,                  PropertyValidator::ITEM_ARRAY, true );
    }

    virtual ~ScenePropertyValidator() {}

    juce_DeclareSingleton( ScenePropertyValidator, false )
};

juce_ImplementSingleton( ScenePropertyValidator )

void Scene::build( const treecore::var& root )
{
    //
    // validate root node
    //
    if ( !root.isObject() )
        throw ConfigParseError( "Scene: root node is not KV" );

    const NamedValueSet& root_kv = root.getDynamicObject()->getProperties();
    {
        Result re = ScenePropertyValidator::getInstance()->validate( root_kv );
        if (!re)
            throw ConfigParseError( "Scene: property validation failed\n" + re.getErrorMessage() );
    }

    //
    // load properties
    //

    // direction of global light
    if ( root_kv.contains( KEY_GLOBAL_LIGHT_DIRECTION ) )
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_DIRECTION].getArray();
        if (values->size() != 4)
            throw ConfigParseError( "Scene: global light direction is not an array of 4 values" );
        m_guts->global_light_direction.set( float( (*values)[0] ),
                                            float( (*values)[1] ),
                                            float( (*values)[2] ),
                                            float( (*values)[3] )
        );
    }

    // color of global light
    if ( root_kv.contains( KEY_GLOBAL_LIGHT_COLOR ) )
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_COLOR].getArray();
        if (values->size() != 4)
            throw ConfigParseError( "Scene: global light color is not an array of 4 values" );
        m_guts->global_light_color.set( float( (*values)[0] ),
                                        float( (*values)[1] ),
                                        float( (*values)[2] ),
                                        float( (*values)[3] )
        );
    }

    // global light intensities
    if ( root_kv.contains( KEY_GLOBAL_LIGHT_AMB ) )
    {
        Array<var>* values = root_kv[KEY_GLOBAL_LIGHT_AMB].getArray();
        if (values->size() != 4)
            throw ConfigParseError( "Scene: global light ambient is not an array of 4 values" );
        m_guts->global_light_ambient.set( float( (*values)[0] ),
                                          float( (*values)[1] ),
                                          float( (*values)[2] ),
                                          float( (*values)[3] )
        );
    }

    // scene graph
    Array<var>* scenenode_nodes = root_kv[KEY_NODES].getArray();
    for (int i = 0; i < scenenode_nodes->size(); i++)
    {
        const var& scenenode_data = (*scenenode_nodes)[i];
        SceneNode* node = m_guts->node_mgr->add_nodes( scenenode_data );;
        m_guts->root_node->add_child( node );
    }
}

} // namespace treeface
