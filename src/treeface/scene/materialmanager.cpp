#include "treeface/scene/materialmanager.h"
#include "treeface/scene/material.h"

#include "treeface/packagemanager.h"

#include "treeface/gl/program.h"
#include "treeface/gl/programmanager.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/private/material_private.h"

#include <treejuce/DynamicObject.h>
#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/String.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
template <>
struct ContainerDeletePolicy<uint8>
{
    static void destroy (uint8* ptr)
    {
        free(ptr);
    }
};
TREEFACE_JUCE_NAMESPACE_END


using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct MaterialManager::Impl
{
    HashMap<String, Holder<Material> > materials;
    Holder<ProgramManager> prog_mgr;
};

MaterialManager::MaterialManager(ProgramManager* prog_mgr)
    : m_impl(new Impl)
{
    m_impl->prog_mgr = prog_mgr;
}

MaterialManager::~MaterialManager()
{
    if (m_impl)
        delete m_impl;
}

#define KEY_PROGRAM      "program"
#define KEY_SCENE_ADDT   "scene_additive"
#define KEY_PROJ_SHADOW  "project_shadow"
#define KEY_RECV_SHADOW  "receive_shadow"
#define KEY_TRANSLUSCENT "transluscent"
#define KEY_TEXTURE      "textures"

Result _validate_(const NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_PROGRAM, PropertyValidator::ITEM_ARRAY, true);
        validator->add_item(KEY_PROJ_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        validator->add_item(KEY_SCENE_ADDT, PropertyValidator::ITEM_SCALAR, false);
        validator->add_item(KEY_RECV_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        validator->add_item(KEY_TRANSLUSCENT, PropertyValidator::ITEM_SCALAR, false);
        validator->add_item(KEY_TEXTURE, PropertyValidator::ITEM_ARRAY, false);
    }

    return validator->validate(kv);
}

treejuce::Result MaterialManager::build_material(const treejuce::var& data, Material* mat)
{
    if (!data.isObject())
    {
        return Result::fail("material root node is not KV");
    }

    NamedValueSet& data_kv = data.getDynamicObject()->getProperties();

    //
    // build program
    //

    // whether program use scene additive
    // which are several pre-defined uniforms providing standard transform
    // matrices and one global light
    bool scene_additive = false;
    if (data_kv.contains(KEY_SCENE_ADDT))
    {
        scene_additive = bool(data_kv[KEY_SCENE_ADDT]);
    }
    printf("scene additive: %d\n", scene_additive);

    // shader file name
    const var& node_program = data_kv[KEY_PROGRAM];
    Array<var>* program_names = node_program.getArray();
    if (program_names->size() != 2)
        return Result::fail("Invalid program specification: "+node_program.toString()+".\nExpect an array of two strings specifying vertex and fragment shader name.");

    String name_vertex = program_names->getReference(0).toString();
    String name_frag = program_names->getReference(1).toString();

    // create program object
    Program* program = nullptr;
    Result program_re = m_impl->prog_mgr->get_program(name_vertex, name_frag, scene_additive, &program);

    if (!program_re)
        return Result::fail("material: failed to build GL program:\n"+program_re.getErrorMessage());

    mat->m_impl->program = program;

    // get scene additive uniforms
    if (scene_additive)
    {
        mat->m_impl->i_uni_trans        = program->get_uniform_index("matrix_model_view");
        mat->m_impl->i_uni_norm_trans   = program->get_uniform_index("matrix_normal");
        mat->m_impl->i_uni_proj_trans   = program->get_uniform_index("matrix_project");
        mat->m_impl->i_uni_light_direct = program->get_uniform_index("main_light_direction");
        mat->m_impl->i_uni_light_color  = program->get_uniform_index("main_light_color");
    }

    //
    // load properties
    //
    if (data_kv.contains(KEY_PROJ_SHADOW))
        mat->m_impl->project_shadow = bool(data_kv[KEY_PROJ_SHADOW]);
    if (data_kv.contains(KEY_RECV_SHADOW))
        mat->m_impl->project_shadow = bool(data_kv[KEY_RECV_SHADOW]);
    if (data_kv.contains(KEY_TRANSLUSCENT))
        mat->m_impl->project_shadow = bool(data_kv[KEY_TRANSLUSCENT]);

    //
    // load textures
    //
    if (data_kv.contains(KEY_TEXTURE))
    {
        const var& tex_list_node = data_kv[KEY_TEXTURE];
        const Array<var>* tex_list = tex_list_node.getArray();

        for (int i_tex = 0; i_tex < tex_list->size(); i_tex++)
        {
            const var& tex_node = tex_list->getReference(i_tex);

            Texture* tex_obj = new Texture();
            Result tex_re = tex_obj->build(tex_node);
            if (!tex_re)
                return Result::fail("failed to build texture "+String(i_tex)+": "+tex_re.getErrorMessage());

            Sampler* sampler_obj = new Sampler();

            // create texture layer
            // the "name" property should has been checked inside build method of Texture object
            String tex_name = tex_node.getProperty(Identifier("name"), var::null).toString();
            int uni_idx = program->get_uniform_index(tex_name);
            if (uni_idx < 0)
                return Result::fail("program don't have texture uniform named "+tex_name);

            mat->m_impl->layers.add({tex_name, tex_obj, sampler_obj, i_tex, uni_idx});
        }
    }

    return Result::ok();
}

Result MaterialManager::get_material(const String& name, Material** mat_pp)
{
    if (m_impl->materials.contains(name))
    {
        *mat_pp = m_impl->materials[name].get();
        return Result::ok();
    }

    ArrayRef<uint8> mat_data = PackageManager::getInstance()->get_item_data(name);
    if (!mat_data.get_data())
    {
        *mat_pp = nullptr;
        return Result::fail("failed to get material config from package manager using name \""+name+"\"");
    }

    ScopedPointer<uint8> mat_data_scope(mat_data.get_data());

    var mat_root_node;
    Result json_re = JSON::parse((char*)mat_data.get_data(), mat_root_node);
    if (!json_re)
    {
        *mat_pp = nullptr;
        return Result::fail(String("failed to parse material JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            "==== material JSON content ====\n\n" +
                            String((const char*)mat_data.get_data()) + "\n" +
                            "==== end of JSON content ====\n\n"
                            );
    }

    Holder<Material> mat = new Material();
    Result mat_re = build_material(mat_root_node, mat);
    if (!mat_re)
    {
        *mat_pp = nullptr;
        return Result::fail(String("failed to build material using JSON content for \"") + name + String("\":\n") +
                            mat_re.getErrorMessage() + "\n" +
                            "==== material JSON content ====\n\n" +
                            String((const char*)mat_data.get_data()) + "\n" +
                            "==== end of JSON content ====\n\n"
                            );
    }

    *mat_pp = mat.get();
    m_impl->materials.set(name, mat);
    return Result::ok();
}

treejuce::Result MaterialManager::get_material(const treejuce::String& name, treejuce::Holder<Material>& result)
{
    Material* mat_ptr = nullptr;
    Result re = get_material(name, &mat_ptr);
    result = mat_ptr;
    return re;
}

bool MaterialManager::material_is_cached(const treejuce::String& name)
{
    return m_impl->materials.contains(name);
}

bool MaterialManager::release_material_hold(const treejuce::String& name)
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

TREEFACE_NAMESPACE_END
