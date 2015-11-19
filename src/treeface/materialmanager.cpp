#include "treeface/materialmanager.h"

#include "treeface/config.h"
#include "treeface/enums.h"
#include "treeface/material.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/program.h"

#include "treeface/misc/propertyvalidator.h"
#include "treeface/misc/stringcast.h"

#include "treeface/guts/material_guts.h"

#include <treecore/DynamicObject.h>
#include <treecore/HashMap.h>
#include <treecore/RefCountHolder.h>
#include <treecore/MemoryBlock.h>
#include <treecore/JSON.h>
#include <treecore/Singleton.h>
#include <treecore/String.h>
#include <treecore/Variant.h>

#if defined TREEFACE_GL_3_0
#   error "TODO what should be it?"
#elif defined TREEFACE_GL_3_3
#   define TREEFACE_GLSL_VERSION_DEF "#version 330\n"
#elif defined TREEFACE_GL_ES_3_0
#   define TREEFACE_GLSL_VERSION_DEF "#version 300 es\n"
#else
#   error "unknown OpenGL version macro"
#endif

using namespace treecore;

namespace treeface {

struct MaterialManager::Impl
{
    HashMap<String, RefCountHolder<Material> > materials;
};

MaterialManager::MaterialManager()
    : m_impl(new Impl)
{
}

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

const char* _src_addition_raw_ =
        TREEFACE_GLSL_VERSION_DEF
        ;

const char* _src_addition_screen_space_ =
        TREEFACE_GLSL_VERSION_DEF
        ;

const char* _src_addition_scene_graph_ =
        TREEFACE_GLSL_VERSION_DEF
        "uniform highp mat4 matrix_model_view;\n"
        "uniform highp mat4 matrix_project;\n"
        "uniform highp mat4 matrix_model_view_project;\n"
        "uniform highp mat4 matrix_normal;\n"
        "uniform mediump vec4 global_light_direction;\n"
        "uniform mediump vec4 global_light_color;\n"
        "uniform mediump vec4 global_light_ambient;\n"
        "\n"
        ;

class MaterialPropertyValidator: public PropertyValidator
{
public:
    MaterialPropertyValidator()
    {
        add_item(KEY_PROGRAM, PropertyValidator::ITEM_ARRAY, true);
        add_item(KEY_TYPE, PropertyValidator::ITEM_SCALAR, true);
        add_item(KEY_PROJ_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        add_item(KEY_RECV_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        add_item(KEY_TRANSLUSCENT, PropertyValidator::ITEM_SCALAR, false);
        add_item(KEY_TEXTURE, PropertyValidator::ITEM_ARRAY, false);
    }

    virtual ~MaterialPropertyValidator() {}

    juce_DeclareSingleton(MaterialPropertyValidator, false)
};
juce_ImplementSingleton(MaterialPropertyValidator)

treecore::Result MaterialManager::build_material(const treecore::var& data, treecore::RefCountHolder<Material>& mat)
{
    if (!data.isObject())
    {
        return Result::fail("material root node is not KV");
    }

    NamedValueSet& data_kv = data.getDynamicObject()->getProperties();
    {
        Result re = MaterialPropertyValidator::getInstance()->validate(data_kv);
        if (!re)
            return re;
    }

    //
    // material type
    // non-raw material type would add several pre-defined uniforms providing
    // stuffs such as standard transform matrices and global light
    //
    MaterialType mat_type = from_string<MaterialType>(data_kv[KEY_TYPE].toString());

    //
    // build program
    //

    // get shader
    const var& node_program = data_kv[KEY_PROGRAM];
    Array<var>* program_names = node_program.getArray();
    if (program_names->size() != 2)
        return Result::fail("Invalid program specification: "+node_program.toString()+".\nExpect an array of two strings specifying vertex and fragment shader name.");

    String name_vertex = (*program_names)[0].toString();
    String name_frag = (*program_names)[1].toString();

    MemoryBlock src_vert_raw;
    MemoryBlock src_frag_raw;
    {
        Result re_src_vert = PackageManager::getInstance()->get_item_data(name_vertex, src_vert_raw);
        if (!re_src_vert)
            return Result::fail("MaterialManager: failed to get vertex shader source:\n" +
                                re_src_vert.getErrorMessage());

        Result re_src_frag = PackageManager::getInstance()->get_item_data(name_frag, src_frag_raw);
        if (!re_src_frag)
            return Result::fail("MaterialManager: failed to get fragment shader source:\n" +
                                re_src_frag.getErrorMessage());
    }

    // create material object by type
    // and preprocess shader source
    String src_vert;
    String src_frag;

    switch (mat_type)
    {
    case MATERIAL_RAW:
        mat = new Material();
        src_vert = String(_src_addition_raw_) + (const char*)src_vert_raw.getData();
        src_frag = String(_src_addition_raw_) + (const char*)src_frag_raw.getData();
        break;
    case MATERIAL_SCENE_GRAPH:
        mat = new SceneGraphMaterial();
        src_vert = String(_src_addition_scene_graph_) + (const char*)src_vert_raw.getData();
        src_frag = String(_src_addition_scene_graph_) + (const char*)src_frag_raw.getData();
        break;
    case MATERIAL_SCREEN_SPACE:
        die("material for screen space not implemented");
        src_vert = String(_src_addition_screen_space_) + (const char*)src_vert_raw.getData();
        src_frag = String(_src_addition_screen_space_) + (const char*)src_frag_raw.getData();
        break;
    default:
        die("unsupported material type %s", to_string(mat_type).toRawUTF8());
    }

    // create and build shader program
    mat->m_program = new Program();
    {
        Result re_prog = mat->m_program->build(src_vert.toRawUTF8(), src_frag.toRawUTF8());
        if (!re_prog)
            return re_prog;
    }

    //
    // load properties
    // get scene additive uniforms
    //
    if (mat_type == MATERIAL_SCENE_GRAPH)
    {
        // scene material defined uniforms
        const Program* prog = mat->m_program;

        SceneGraphMaterial* sgmat = dynamic_cast<SceneGraphMaterial*>(mat.get());
        sgmat->m_uni_model_view      = prog->get_uniform_location("matrix_model_view");
        sgmat->m_uni_proj            = prog->get_uniform_location("matrix_project");
        sgmat->m_uni_model_view_proj = prog->get_uniform_location("matrix_model_view_project");
        sgmat->m_uni_norm            = prog->get_uniform_location("matrix_normal");
        sgmat->m_uni_light_direct    = prog->get_uniform_location("global_light_direction");
        sgmat->m_uni_light_color     = prog->get_uniform_location("global_light_color");
        sgmat->m_uni_light_ambient   = prog->get_uniform_location("global_light_ambient");

        // scene properties
        if (data_kv.contains(KEY_PROJ_SHADOW))
            sgmat->m_project_shadow = bool(data_kv[KEY_PROJ_SHADOW]);
        if (data_kv.contains(KEY_RECV_SHADOW))
            sgmat->m_receive_shadow = bool(data_kv[KEY_RECV_SHADOW]);
        if (data_kv.contains(KEY_TRANSLUSCENT))
            sgmat->m_translucent = bool(data_kv[KEY_TRANSLUSCENT]);
    }
    else if (mat_type == MATERIAL_SCREEN_SPACE)
    {
        die("no implemented");
    }

    //
    // load textures
    //
    if (data_kv.contains(KEY_TEXTURE))
    {
        const var& tex_list_node = data_kv[KEY_TEXTURE];
        const Array<var>* tex_list = tex_list_node.getArray();

        for (int i_tex = 0; i_tex < tex_list->size(); i_tex++)
        {
            const var& tex_node = (*tex_list)[i_tex];

            Texture* tex_obj = new Texture();
            Result tex_re = tex_obj->build(tex_node);
            if (!tex_re)
                return Result::fail("failed to build texture "+String(i_tex)+": "+tex_re.getErrorMessage());

            Sampler* sampler_obj = new Sampler();

            // create texture layer
            // the "name" property should has been checked inside build method of Texture object
            String tex_name = tex_node.getProperty(Identifier("name"), var::null).toString();
            int uni_idx = mat->m_program->get_uniform_location(tex_name);

            if (uni_idx >= 0)
                mat->m_impl->layers.add({tex_name, tex_obj, sampler_obj, uni_idx});
            else
                warn("program don't have texture unit named %s", tex_name.toRawUTF8());
        }
    }

    return Result::ok();
}

Result MaterialManager::get_material(const String& name, RefCountHolder<Material>& mat)
{
    if (m_impl->materials.contains(name))
    {
        mat = m_impl->materials[name];
        return Result::ok();
    }

    // get material source by name
    MemoryBlock mat_data;
    Result item_re = PackageManager::getInstance()->get_item_data(name, mat_data);
    if (!item_re)
    {
        mat = nullptr;
        return Result::fail("failed to get material config:\n" +
                            item_re.getErrorMessage());
    }

    // parse JSON
    var mat_root_node;
    Result json_re = JSON::parse((char*)mat_data.getData(), mat_root_node);
    if (!json_re)
    {
        mat = nullptr;
        return Result::fail(String("failed to parse material JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            "==== material JSON content ====\n\n" +
                            String((const char*)mat_data.getData()) + "\n" +
                            "==== end of JSON content ====\n\n"
                            );
    }

    // build material from JSON structure
    Result mat_re = build_material(mat_root_node, mat);
    if (!mat_re)
    {
        mat = nullptr;
        return Result::fail(String("failed to build material using JSON content for \"") + name + String("\":\n") +
                            mat_re.getErrorMessage() + "\n" +
                            "==== material JSON content ====\n\n" +
                            String((const char*)mat_data.getData()) + "\n" +
                            "==== end of JSON content ====\n\n"
                            );
    }

    m_impl->materials.set(name, mat);
    return Result::ok();
}

treecore::Result MaterialManager::get_material(const treecore::String& name, Material** mat_pp)
{
    RefCountHolder<Material> mat;
    Result re = get_material(name, mat);
    *mat_pp = mat.get();
    return re;
}

bool MaterialManager::material_is_cached(const treecore::String& name)
{
    return m_impl->materials.contains(name);
}

bool MaterialManager::release_material_hold(const treecore::String& name)
{
    if (m_impl->materials.contains(name))
    {
        m_impl->materials.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace treeface
