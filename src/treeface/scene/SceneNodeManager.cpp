#include "treeface/scene/SceneNodeManager.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/GeometryManager.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/VisualObject.h"

#include "treeface/gl/VertexArray.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"

#include "treeface/scene/guts/SceneNode_guts.h"

#include "treeface/scene/MaterialManager.h"
#include "treeface/base/PackageManager.h"

#include <treecore/CriticalSection.h>
#include <treecore/DynamicObject.h>
#include <treecore/HashMap.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct SceneNodeManager::Impl
{
    RefCountHolder<GeometryManager> geo_mgr;
    RefCountHolder<MaterialManager> mat_mgr;

    HashMap<Identifier, RefCountHolder<SceneNode> > nodes;
};

SceneNodeManager::SceneNodeManager( GeometryManager* geo_mgr, MaterialManager* mat_mgr )
    : m_impl( new Impl() )
{
    m_impl->geo_mgr = geo_mgr;
    m_impl->mat_mgr = mat_mgr;
}

SceneNodeManager::~SceneNodeManager()
{
    if (m_impl)
        delete m_impl;
}

SceneNode* SceneNodeManager::add_nodes( const treecore::var& data )
{
    SceneNode* root_node = new SceneNode();
    build_node( data, root_node );
    return root_node;
}

SceneNode* SceneNodeManager::get_node( const treecore::Identifier& name )
{
    if ( m_impl->nodes.contains( name ) )
        return m_impl->nodes[name].get();

    // build node object
    var data_root = PackageManager::getInstance()->get_item_json( name );
    if (!data_root)
        throw ConfigParseError( "no package item named \"" + name.toString() + "\"" );
    return add_nodes( data_root );
}

#define KEY_VISUAL_MAT "material"
#define KEY_VISUAL_GEO "geometry"

class VisualItemPropertyValidator: public PropertyValidator
{
public:
    VisualItemPropertyValidator()
    {
        add_item( KEY_VISUAL_MAT, PropertyValidator::ITEM_SCALAR, true );
        add_item( KEY_VISUAL_GEO, PropertyValidator::ITEM_SCALAR, true );
    }

    virtual ~VisualItemPropertyValidator() {}

    juce_DeclareSingleton( VisualItemPropertyValidator, false )
};
juce_ImplementSingleton( VisualItemPropertyValidator )

VisualObject * SceneNodeManager::create_visual_object( const var &data )
{
    // validate node
    if ( !data.isObject() )
        throw ConfigParseError( "visual item node is not KV" );

    const NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = VisualItemPropertyValidator::getInstance()->validate( data_kv );
        if (!re) throw ConfigParseError( re.getErrorMessage() );
    }

    // get material and geometry
    Material* mat = m_impl->mat_mgr->get_material( data_kv[KEY_VISUAL_MAT].toString() );
    if (mat == nullptr)
        throw ConfigParseError( "no material named \"" + data_kv[KEY_VISUAL_MAT].toString() + "\"" );

    SceneGraphMaterial* scene_mat = dynamic_cast<SceneGraphMaterial*>(mat);
    if (!scene_mat)
        throw ConfigParseError( "material \"" + data_kv[KEY_VISUAL_MAT].toString() + "\" is not a scene graph material" );

    Geometry* geom = m_impl->geo_mgr->get_geometry( data_kv[KEY_VISUAL_GEO].toString() );
    if (geom == nullptr)
        throw ConfigParseError( "no geometry named \"" + data_kv[KEY_VISUAL_GEO].toString() + "\"" );

    // do build
    return new VisualObject( geom, scene_mat );
}

#define KEY_ID     "id"
#define KEY_TRANS  "transform"
#define KEY_CHILD  "children"
#define KEY_VISUAL "visual"

class SceneNodePropertyValidator: public PropertyValidator
{
public:
    SceneNodePropertyValidator()
    {
        add_item( KEY_ID,     PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_TRANS,  PropertyValidator::ITEM_ARRAY,  false );
        add_item( KEY_CHILD,  PropertyValidator::ITEM_ARRAY,  false );
        add_item( KEY_VISUAL, PropertyValidator::ITEM_ARRAY,  false );
    }

    virtual ~SceneNodePropertyValidator() {}

    juce_DeclareSingleton( SceneNodePropertyValidator, false )
};
juce_ImplementSingleton( SceneNodePropertyValidator )

void SceneNodeManager::build_node( const treecore::var& data, SceneNode* node )
{
    if ( !data.isObject() )
        throw ConfigParseError( "SceneNode data root is not KV" );

    const NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = SceneNodePropertyValidator::getInstance()->validate( data_kv );
        if (!re)
            throw ConfigParseError( "node data validation failed:\n" + re.getErrorMessage() );
    }

    //
    // node transform
    //
    if ( data_kv.contains( KEY_TRANS ) )
    {
        const Array<var>* trans_array = data_kv[KEY_TRANS].getArray();
        if (trans_array->size() != 16)
            throw ConfigParseError( "transform is not an array of 16 numbers: " + String( trans_array->size() ) );

        Mat4f mat( static_cast<float>( (*trans_array)[0] ),  float( (*trans_array)[1] ),  float( (*trans_array)[2] ),  float( (*trans_array)[3] ),
                   static_cast<float>( (*trans_array)[4] ),  float( (*trans_array)[5] ),  float( (*trans_array)[6] ),  float( (*trans_array)[7] ),
                   static_cast<float>( (*trans_array)[8] ),  float( (*trans_array)[9] ),  float( (*trans_array)[10] ), float( (*trans_array)[11] ),
                   static_cast<float>( (*trans_array)[12] ), float( (*trans_array)[13] ), float( (*trans_array)[14] ), float( (*trans_array)[15] ) );
        node->m_impl->trans       = mat;
        node->m_impl->trans_dirty = true;
    }

    //
    // visual items
    //
    if ( data_kv.contains( KEY_VISUAL ) )
    {
        const Array<var>* visual_array = data_kv[KEY_VISUAL].getArray();
        for (int i = 0; i < visual_array->size(); i++)
        {
            // TODO: may be not a VisualObject
            VisualObject* item = create_visual_object( (*visual_array)[i] );
            node->add_item( item );
        }
    }

    //
    // child nodes
    //
    if ( data_kv.contains( KEY_CHILD ) )
    {
        const Array<var>* child_array = data_kv[KEY_CHILD].getArray();
        for (int i = 0; i < child_array->size(); i++)
        {
            SceneNode* child = new SceneNode();
            build_node( (*child_array)[i], child );

            child->m_impl->parent       = node;
            child->m_impl->global_dirty = true;
            node->m_impl->child_nodes.add( child );
        }
    }

    //
    // if has ID, store it
    //
    if ( data_kv.contains( KEY_ID ) )
        m_impl->nodes.set( data_kv[KEY_ID].toString(), node );
}

} // namespace treeface
