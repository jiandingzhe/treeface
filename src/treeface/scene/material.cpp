#include "treeface/scene/material.h"

#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/packagemanager.h"
#include "treeface/programmanager.h"
#include "treeface/stringcast.h"
#include "treeface/misc/propertyvalidator.h"

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

#define KEY_PROGRAM      "program"
#define KEY_PROJ_SHADOW  "project_shadow"
#define KEY_RECV_SHADOW  "receive_shadow"
#define KEY_TRANSLUSCENT "transluscent"
#define KEY_TEX          "textures"

PropertyValidator* _validator_ = nullptr;
Result _validate_(const NamedValueSet& kv)
{
    if (!_validator_)
    {
        _validator_ = new PropertyValidator();
        _validator_->add_item(KEY_PROGRAM, PropertyValidator::ITEM_ARRAY, true);
        _validator_->add_item(KEY_PROJ_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        _validator_->add_item(KEY_RECV_SHADOW, PropertyValidator::ITEM_SCALAR, false);
        _validator_->add_item(KEY_TRANSLUSCENT, PropertyValidator::ITEM_SCALAR, false);
        _validator_->add_item(KEY_TEX, PropertyValidator::ITEM_HASH, false);
    }

    return _validator_->validate(kv);
}

treejuce::Result Material::build(const treejuce::var& root_node)
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
    const var& node_program = root_node[KEY_PROGRAM];
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
    if (root_kv.contains(KEY_PROJ_SHADOW))
        m_impl->project_shadow = bool(root_kv[KEY_PROJ_SHADOW]);
    if (root_kv.contains(KEY_RECV_SHADOW))
        m_impl->project_shadow = bool(root_kv[KEY_RECV_SHADOW]);
    if (root_kv.contains(KEY_TRANSLUSCENT))
        m_impl->project_shadow = bool(root_kv[KEY_TRANSLUSCENT]);

    //
    // load textures
    //
    HashMap<String, TextureLayer> textures_by_name;
    if (root_kv.contains(KEY_TEX))
    {
        const var& node_tex_list = root_kv[KEY_TEX];
        const NamedValueSet& tex_list = node_tex_list.getDynamicObject()->getProperties();

        for (int i_tex = 0; i_tex < tex_list.size(); i_tex++)
        {
            Identifier tex_key = tex_list.getName(i_tex);
            const var& tex_node = tex_list[tex_key];
            String tex_name = tex_key.toString();

            // create texture layer
            int uni_idx = program->get_uniform_index(tex_name);
            if (uni_idx < 0)
                return Result::fail("program don't have texture uniform named "+tex_name);

            Texture* tex_obj = new Texture();
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
