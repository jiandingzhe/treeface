#include "treeface/scene/material.h"

#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/packagemanager.h"
#include "treeface/programmanager.h"
#include "treeface/stringcast.h"

#include <treejuce/Array.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/Holder.h>
#include <treejuce/Logger.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct TextureLayer
{
    String name;
    Holder<Texture> gl_texture;
    GLint layer;
    GLint program_uniform_idx;
};

struct Material::Impl
{
    Array<TextureLayer> layers;
    Holder<Program> program;
    bool translucent = false;
    bool project_shadow = true;
    bool receive_shadow = true;
};


Material::Material()
{
}

Material::~Material()
{
    if (m_impl)
        delete m_impl;
}

treejuce::Result Material::init(const treejuce::var& root_node)
{
    m_impl = new Impl();

    if (!root_node.isObject())
    {
        return Result::fail("material root node is not KV");
    }

    NamedValueSet& root_kv = root_node.getDynamicObject()->getProperties();

    //
    // firstly, build program
    //
    if (!root_kv.contains("program"))
        return Result::fail("root node has no program");

    const var& node_program = root_kv["program"];
    if (!node_program.isArray())
        return Result::fail("Invalid program specification: "+node_program.toString()+".\nExpect an array of two strings specifying vertex and fragment shader name.");

    Array<var>* program_names = node_program.getArray();
    if (program_names->size() != 2)
        return Result::fail("Invalid program specification: "+node_program.toString()+".\nExpect an array of two strings specifying vertex and fragment shader name.");

    String name_vertex = program_names->getReference(0).toString();
    String name_frag = program_names->getReference(1).toString();

    Program* program = nullptr;
    Result program_re = ProgramManager::getInstance()->get_program(name_vertex, name_frag, &program);

    if (!program_re)
        return Result::fail("material: failed to build GL program:\n"+program_re.getErrorMessage());

    m_impl->program = program;

    //
    // load properties
    //
    if (root_kv.contains("project_shadow"))
        m_impl->project_shadow = bool(root_kv["project_shadow"]);
    if (root_kv.contains("receive_shadow"))
        m_impl->project_shadow = bool(root_kv["receive_shadow"]);
    if (root_kv.contains("transluscent"))
        m_impl->project_shadow = bool(root_kv["transluscent"]);

    //
    // load textures
    //
    HashMap<String, TextureLayer> textures_by_name;
    if (root_kv.contains("texture"))
    {
        const var& node_tex_list = root_kv["texture"];
        if (!node_tex_list.isObject())
            return Result::fail("the value of \"texture\" is not an object");

        const NamedValueSet& tex_list = node_tex_list.getDynamicObject()->getProperties();

        for (int i_tex = 0; i_tex < tex_list.size(); i_tex++)
        {
            Identifier tex_key = tex_list.getName(i_tex);
            const var& tex_node = tex_list[tex_key];
            String tex_name = tex_key.toString();

            // create texture layer
            int uni_idx = program->get_uniform_index_by_name(tex_name);
            if (uni_idx < 0)
                return Result::fail("program don't have texture uniform named "+tex_name);

            Texture* tex_obj = new Texture();
            tex_obj->init();
            m_impl->layers.add({tex_name, tex_obj, i_tex, uni_idx});

            Result tex_re = tex_obj->build(tex_node);
            if (!tex_re)
            {
                return Result::fail("failed to build texture "+tex_name+": "+tex_re.getErrorMessage());
            }
        }
    }

    return Result::ok();
}

Program* Material::get_program() NOEXCEPT
{
    return m_impl->program.get();
}

int32 Material::get_num_textures() const NOEXCEPT
{
    return m_impl->layers.size();
}

Texture* Material::get_texture(treejuce::int32 layer_idx) NOEXCEPT
{
    if (layer_idx >= m_impl->layers.size())
        return nullptr;
    else
        return m_impl->layers[layer_idx].gl_texture.get();
}

Texture* Material::get_texture(treejuce::StringRef name) NOEXCEPT
{
    for (int i = 0; i < m_impl->layers.size(); i++)
    {
        if (m_impl->layers[i].name == name)
            return m_impl->layers[i].gl_texture.get();
    }

    return nullptr;
}


TREEFACE_NAMESPACE_END
