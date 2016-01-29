#include "treeface/scene/MaterialManager.h"

#include "treeface/Config.h"
#include "treeface/base/Enums.h"
#include "treeface/scene/Material.h"
#include "treeface/base/PackageManager.h"

#include "treeface/scene/SceneGraphMaterial.h"

#include "treeface/gl/Program.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"
#include "treeface/misc/StringCast.h"

#include "treeface/scene/guts/Material_guts.h"

#include <treecore/DynamicObject.h>
#include <treecore/HashMap.h>
#include <treecore/MemoryBlock.h>
#include <treecore/JSON.h>
#include <treecore/RefCountHolder.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

#if defined TREEFACE_GL_3_0
#    error "TODO what should be it?"
#elif defined TREEFACE_GL_3_3
#    define TREEFACE_GLSL_VERSION_DEF "#version 330\n"
#elif defined TREEFACE_GL_ES_3_0
#    define TREEFACE_GLSL_VERSION_DEF "#version 300 es\n"
#else
#    error "unknown OpenGL version macro"
#endif

using namespace treecore;

namespace treeface {

struct MaterialManager::Impl
{
    HashMap<Identifier, RefCountHolder<Material> > materials;
};

MaterialManager::MaterialManager()
    : m_impl( new Impl )
{}

MaterialManager::~MaterialManager()
{
    if (m_impl)
        delete m_impl;
}

#define KEY_PROGRAM      "program"
#define KEY_TYPE         "type"
#define KEY_PROJ_SHADOW  "project_shadow"
#define KEY_RECV_SHADOW  "receive_shadow"
#define KEY_TRANSLUSCENT "transluscent"
#define KEY_TEXTURE      "textures"

#define NAME_MAT_MV       "matrix_model_view"
#define NAME_MAT_PROJ     "matrix_project"
#define NAME_MAT_MVP      "matrix_model_view_project"
#define NAME_MAT_NORM     "matrix_normal"
#define NAME_GLB_L_DIRECT "global_light_direction"
#define NAME_GLB_L_COLOR  "global_light_color"
#define NAME_GLB_L_AMB    "global_light_ambient"

const char* _src_addition_raw_ =
    TREEFACE_GLSL_VERSION_DEF
;

const char* _src_addition_screen_space_ =
    TREEFACE_GLSL_VERSION_DEF
;

const char* _src_addition_scene_graph_ =
    TREEFACE_GLSL_VERSION_DEF
    "uniform highp mat4 " NAME_MAT_MV ";\n"
    "uniform highp mat4 " NAME_MAT_PROJ ";\n"
    "uniform highp mat4 " NAME_MAT_MVP ";\n"
    "uniform highp mat4 " NAME_MAT_NORM ";\n"
    "uniform mediump vec4 " NAME_GLB_L_DIRECT ";\n"
    "uniform mediump vec4 " NAME_GLB_L_COLOR ";\n"
    "uniform mediump vec4 " NAME_GLB_L_AMB ";\n"
    "\n"
;

class MaterialPropertyValidator: public PropertyValidator
{
public:
    MaterialPropertyValidator()
    {
        add_item( KEY_PROGRAM,      PropertyValidator::ITEM_ARRAY,  true );
        add_item( KEY_TYPE,         PropertyValidator::ITEM_SCALAR, true );
        add_item( KEY_PROJ_SHADOW,  PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_RECV_SHADOW,  PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_TRANSLUSCENT, PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_TEXTURE,      PropertyValidator::ITEM_ARRAY,  false );
    }

    virtual ~MaterialPropertyValidator() {}

    juce_DeclareSingleton( MaterialPropertyValidator, false )
};
juce_ImplementSingleton( MaterialPropertyValidator )

Material * MaterialManager::build_material( const treecore::Identifier & name, const treecore::var & data )
{
    // validate data
    if ( !data.isObject() )
        throw ConfigParseError( "Material root node is not KV" );

    NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = MaterialPropertyValidator::getInstance()->validate( data_kv );
        if (!re)
            throw ConfigParseError( "Invalid material JSON node: " + re.getErrorMessage() );
    }

    //
    // material type
    // non-raw material type would add some predefined uniforms providing
    // stuffs such as standard transform matrices and global light
    //
    MaterialType mat_type;
    if ( !fromString<MaterialType>( data_kv[KEY_TYPE], mat_type ) )
        throw ConfigParseError( "Invalid material type: " + data_kv[KEY_TYPE].toString() );

    //
    // build program
    //

    // get shader
    const var&  node_program  = data_kv[KEY_PROGRAM];
    Array<var>* program_names = node_program.getArray();
    if (program_names->size() != 2)
        throw ConfigParseError( "Invalid program specification: " + node_program.toString() + ".\nExpect an array of two strings specifying vertex and fragment shader name." );

    String name_vertex = (*program_names)[0].toString();
    String name_frag   = (*program_names)[1].toString();

    MemoryBlock src_vert_raw;
    MemoryBlock src_frag_raw;
    {
        if ( !PackageManager::getInstance()->get_item_data( name_vertex, src_vert_raw, true ) )
            throw ConfigParseError( "MaterialManager: no vertex shader resource named \"" + name_vertex + "\"" );

        if ( !PackageManager::getInstance()->get_item_data( name_frag, src_frag_raw, true ) )
            throw ConfigParseError( "MaterialManager: no fragment shader resource named \"" + name_frag + "\"" );
    }

    // create material object by type
    // and preprocess shader source
    Material* mat = nullptr;
    String    src_vert;
    String    src_frag;

    switch (mat_type)
    {
    case MATERIAL_RAW:
        mat      = new Material();
        src_vert = String( _src_addition_raw_ ) + (const char*) src_vert_raw.getData();
        src_frag = String( _src_addition_raw_ ) + (const char*) src_frag_raw.getData();
        break;
    case MATERIAL_SCENE_GRAPH:
        mat      = new SceneGraphMaterial();
        src_vert = String( _src_addition_scene_graph_ ) + (const char*) src_vert_raw.getData();
        src_frag = String( _src_addition_scene_graph_ ) + (const char*) src_frag_raw.getData();
        break;
    case MATERIAL_SCREEN_SPACE:
        die( "material for screen space not implemented" );
        src_vert = String( _src_addition_screen_space_ ) + (const char*) src_vert_raw.getData();
        src_frag = String( _src_addition_screen_space_ ) + (const char*) src_frag_raw.getData();
        break;
    default:
        die( "unsupported material type enum: %d", mat_type );
    }

    // create and build shader program
    mat->m_program = new Program( src_vert.toRawUTF8(), src_frag.toRawUTF8() );

    //
    // load properties
    // get scene additive uniforms
    //
    if (mat_type == MATERIAL_SCENE_GRAPH)
    {
        // scene material defined uniforms
        const Program* prog = mat->m_program;

        SceneGraphMaterial* sgmat = dynamic_cast<SceneGraphMaterial*>( mat );
        sgmat->m_uni_model_view      = prog->get_uniform_location( NAME_MAT_MV );
        sgmat->m_uni_proj            = prog->get_uniform_location( NAME_MAT_PROJ );
        sgmat->m_uni_model_view_proj = prog->get_uniform_location( NAME_MAT_MVP );
        sgmat->m_uni_norm            = prog->get_uniform_location( NAME_MAT_NORM );
        sgmat->m_uni_light_direct    = prog->get_uniform_location( NAME_GLB_L_DIRECT );
        sgmat->m_uni_light_color     = prog->get_uniform_location( NAME_GLB_L_COLOR );
        sgmat->m_uni_light_ambient   = prog->get_uniform_location( NAME_GLB_L_AMB );

        // scene properties
        if ( data_kv.contains( KEY_PROJ_SHADOW ) )
            sgmat->m_project_shadow = bool(data_kv[KEY_PROJ_SHADOW]);
        if ( data_kv.contains( KEY_RECV_SHADOW ) )
            sgmat->m_receive_shadow = bool(data_kv[KEY_RECV_SHADOW]);
        if ( data_kv.contains( KEY_TRANSLUSCENT ) )
            sgmat->m_translucent = bool(data_kv[KEY_TRANSLUSCENT]);
    }
    else if (mat_type == MATERIAL_SCREEN_SPACE)
    {
        die( "no implemented" );
    }

    //
    // load textures
    //
    if ( data_kv.contains( KEY_TEXTURE ) )
    {
        const var& tex_list_node   = data_kv[KEY_TEXTURE];
        const Array<var>* tex_list = tex_list_node.getArray();

        for (int i_tex = 0; i_tex < tex_list->size(); i_tex++)
        {
            const var& tex_node = (*tex_list)[i_tex];

            Texture* tex_obj = new Texture( tex_node );

            // create texture layer
            // the "name" property should has been checked inside build method of Texture object
            String tex_name = tex_node.getProperty( Identifier( "name" ), var::null ).toString();
            int    uni_idx  = mat->m_program->get_uniform_location( tex_name );

            if (uni_idx >= 0)
                mat->m_impl->layers.add( { tex_name, tex_obj, uni_idx } );
            else
                warn( "program don't have texture unit named %s", tex_name.toRawUTF8() );
        }
    }

    // store material
    m_impl->materials.set( name, mat );

    return mat;
}

Material* MaterialManager::get_material( const Identifier& name )
{
    if ( m_impl->materials.contains( name ) )
        return m_impl->materials[name];

    // get item JSON
    var mat_root_node = PackageManager::getInstance()->get_item_json( name );
    if (!mat_root_node)
        throw ConfigParseError( "no material named \"" + name.toString() + "\"" );

    // build material from JSON structure
    return build_material( name, mat_root_node );
}

bool MaterialManager::material_is_cached( const treecore::Identifier& name )
{
    return m_impl->materials.contains( name );
}

bool MaterialManager::release_material_hold( const treecore::Identifier& name )
{
    if ( m_impl->materials.contains( name ) )
    {
        m_impl->materials.remove( name );
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace treeface
