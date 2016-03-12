#include "treeface/scene/MaterialManager.h"

#include "treeface/Config.h"

#include "treeface/base/Enums.h"
#include "treeface/base/PackageManager.h"

#include "treeface/gl/Program.h"
#include "treeface/gl/TextureManager.h"

#include "treeface/graphics/VectorGraphicsMaterial.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"
#include "treeface/misc/StringCast.h"

#include "treeface/scene/Material.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/guts/Material_guts.h"

#include <treecore/DynamicObject.h>
#include <treecore/HashMap.h>
#include <treecore/MemoryBlock.h>
#include <treecore/JSON.h>
#include <treecore/RefCountHolder.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct ProgramKey
{
    Identifier   name_vert;
    Identifier   name_frag;
    MaterialType type;
};

bool operator ==( const ProgramKey& a, const ProgramKey& b )
{
    return a.name_vert == b.name_vert && a.name_frag == b.name_frag && a.type == b.type;
}

struct ProgramKeyHasher
{
    int generateHash( const ProgramKey& key, int limit ) const noexcept
    {
        pointer_sized_uint result = pointer_sized_uint( key.name_vert.getPtr() ) +
                                    pointer_sized_uint( key.name_frag.getPtr() ) +
                                    pointer_sized_uint( key.type );
        return int(result) % limit;
    }
};

typedef HashMap<ProgramKey, RefCountHolder<Program>, ProgramKeyHasher> ProgramMap;

struct MaterialManager::Impl
{
    HashMap<Identifier, RefCountHolder<Material> > materials;
    ProgramMap programs;

    Program* get_or_build_program( Identifier name_vert, Identifier name_frag, MaterialType type );
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
#define KEY_OUTPUT       "output"
#define KEY_PROJ_SHADOW  "project_shadow"
#define KEY_RECV_SHADOW  "receive_shadow"
#define KEY_TRANSLUSCENT "transluscent"
#define KEY_TEXTURE      "textures"

#define KEY_OUTPUT_COLORS  "colors"
#define KEY_OUTPUT_DEPTH   "depth"
#define KEY_OUTPUT_STENCIL "stencil"

class MaterialPropertyValidator: public PropertyValidator, public treecore::RefCountSingleton<MaterialPropertyValidator>
{
public:
    MaterialPropertyValidator()
    {
        add_item( KEY_PROGRAM,      PropertyValidator::ITEM_ARRAY,  true );
        add_item( KEY_TYPE,         PropertyValidator::ITEM_SCALAR, true );
        add_item( KEY_OUTPUT,       PropertyValidator::ITEM_HASH,   false );
        add_item( KEY_PROJ_SHADOW,  PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_RECV_SHADOW,  PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_TRANSLUSCENT, PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_TEXTURE,      PropertyValidator::ITEM_HASH,   false );
    }

    virtual ~MaterialPropertyValidator() {}
};

class MaterialOutputPropertyValidator: public PropertyValidator, public treecore::RefCountSingleton<MaterialOutputPropertyValidator>
{
public:
    MaterialOutputPropertyValidator()
    {
        add_item( KEY_OUTPUT_COLORS,  PropertyValidator::ITEM_ARRAY,  false );
        add_item( KEY_OUTPUT_DEPTH,   PropertyValidator::ITEM_SCALAR, false );
        add_item( KEY_OUTPUT_STENCIL, PropertyValidator::ITEM_SCALAR, false );
    }

    virtual ~MaterialOutputPropertyValidator() {}
};

Material* MaterialManager::build_material( const treecore::Identifier& name, const treecore::var& data )
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
    // create material by type
    //

    // get material type
    MaterialType mat_type;
    if ( !fromString<MaterialType>( data_kv[KEY_TYPE], mat_type ) )
        throw ConfigParseError( "Invalid material type: " + data_kv[KEY_TYPE].toString() );

    // create material object by type
    Material* mat = nullptr;

    switch (mat_type)
    {
    case MATERIAL_RAW:             mat = new Material(); break;
    case MATERIAL_SCENE_GRAPH:     mat = new SceneGraphMaterial(); break;
    case MATERIAL_VECTOR_GRAPHICS: mat = new VectorGraphicsMaterial(); break;
    case MATERIAL_SCREEN_SPACE:    die( "material for screen space not implemented" ); break;
    default:
        die( "unsupported material type enum: %d", mat_type );
    }

    //
    // build program
    //

    // get shader source
    const var&  node_program  = data_kv[KEY_PROGRAM];
    Array<var>* program_names = node_program.getArray();
    if (program_names->size() != 2)
        throw ConfigParseError( "Invalid program specification: " + node_program.toString() + ".\nExpect an array of two strings specifying vertex and fragment shader name." );

    ProgramKey prog_key{ Identifier( (*program_names)[0].toString() ),
                         Identifier( (*program_names)[1].toString() ),
                         mat_type };

    Program* prog = m_impl->programs.getOrDefault( prog_key, nullptr );

    if (prog == nullptr)
    {
        MemoryBlock src_vert_raw;
        MemoryBlock src_frag_raw;
        {
            if ( !PackageManager::getInstance()->get_item_data( prog_key.name_vert, src_vert_raw, true ) )
                throw ConfigParseError( "MaterialManager: no vertex shader resource named \"" + prog_key.name_vert.toString() + "\"" );

            if ( !PackageManager::getInstance()->get_item_data( prog_key.name_frag, src_frag_raw, true ) )
                throw ConfigParseError( "MaterialManager: no fragment shader resource named \"" + prog_key.name_frag.toString() + "\"" );
        }

        // preprocess shader source
        String src_vert = mat->get_shader_source_addition() + (const char*) src_vert_raw.getData();
        String src_frag = mat->get_shader_source_addition() + (const char*) src_frag_raw.getData();

        // create and store program
        prog = new Program( src_vert.toRawUTF8(), src_frag.toRawUTF8() );
        m_impl->programs.set( prog_key, prog );
    }

    mat->init( prog );

    //
    // load scene properties
    // get scene additive uniforms
    //
    SceneGraphMaterial* sgmat = dynamic_cast<SceneGraphMaterial*>(mat);
    if (sgmat != nullptr)
    {
        // scene properties
        if ( data_kv.contains( KEY_PROJ_SHADOW ) )
            sgmat->m_project_shadow = bool(data_kv[KEY_PROJ_SHADOW]);
        if ( data_kv.contains( KEY_RECV_SHADOW ) )
            sgmat->m_receive_shadow = bool(data_kv[KEY_RECV_SHADOW]);
        if ( data_kv.contains( KEY_TRANSLUSCENT ) )
            sgmat->m_translucent = bool(data_kv[KEY_TRANSLUSCENT]);
    }

    //
    // load output configuration
    //
    if ( data_kv.contains( KEY_OUTPUT ) )
    {
        mat->m_impl->output_managed = true;
        const var& node_output = data_kv[KEY_OUTPUT];
        const NamedValueSet& kv_output = node_output.getDynamicObject()->getProperties();

        if ( kv_output.contains( KEY_OUTPUT_DEPTH ) )
            mat->m_impl->output_depth = bool(kv_output[KEY_OUTPUT_DEPTH]);
        if ( kv_output.contains( KEY_OUTPUT_STENCIL ) )
            mat->m_impl->output_stencil = bool(kv_output[KEY_OUTPUT_STENCIL]);

        if ( kv_output.contains( KEY_OUTPUT_COLORS ) )
        {
            Array<var>* color_out_names = kv_output[KEY_OUTPUT_COLORS].getArray();
            for (const var& color_out_name : * color_out_names)
            {
                GLint color_out_loc = glGetFragDataLocation( prog->get_gl_handle(), color_out_name.toString().toRawUTF8() );
                mat->m_impl->output_colors.add( { Identifier( color_out_name ), color_out_loc } );
            }
        }
    }
    else
    {
        mat->m_impl->output_managed = false;
    }

    //
    // load textures
    //
    if ( data_kv.contains( KEY_TEXTURE ) )
    {
        const NamedValueSet::MapType& textures = data_kv[KEY_TEXTURE].getDynamicObject()->getProperties().getValues();
        Program* prog = mat->m_program;

        for (NamedValueSet::MapType::ConstIterator it( textures ); it.next(); )
        {
            const Identifier& uni_name = it.key();
            int uni_loc = prog->get_uniform_location( uni_name );

            if (uni_loc >= 0)
            {
                Texture* tex_obj = TextureManager::getInstance()->get_texture( it.value().toString() );
                mat->m_impl->layers.add( { uni_name, tex_obj, uni_loc } );
            }
            else
            {
                warn( "program don't have texture uniform named %s", it.key().toString().toRawUTF8() );
            }
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
