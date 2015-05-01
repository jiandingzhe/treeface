#include "treeface/scene/material.h"

#include "treeface/image.h"
#include "treeface/imagemanager.h"

#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"
#include "treeface/gl/sampler.h"
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
    Holder<Sampler> gl_sampler;
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
#define KEY_TEXTURE      "textures"

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
        _validator_->add_item(KEY_TEXTURE, PropertyValidator::ITEM_ARRAY, false);
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
    if (root_kv.contains(KEY_TEXTURE))
    {
        const var& tex_list_node = root_kv[KEY_TEXTURE];
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

            m_impl->layers.add({tex_name, tex_obj, sampler_obj, i_tex, uni_idx});
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

void Material::use() NOEXCEPT
{

    for (int i_layer = 0; i_layer < m_impl->layers.size(); i_layer++)
    {
        TextureLayer& curr_layer = m_impl->layers.getReference(i_layer);
        jassert(i_layer == curr_layer.layer);

        glActiveTexture(GL_TEXTURE0 + i_layer);
        curr_layer.gl_sampler->use(i_layer);
        curr_layer.gl_texture->use();
        m_impl->program->instant_set_uniform(curr_layer.program_uniform_idx, *curr_layer.gl_sampler);
    }

    glActiveTexture(GL_TEXTURE0);
    m_impl->program->use();
}

void Material::unuse() NOEXCEPT
{
    for (int i_layer = 0; i_layer < m_impl->layers.size(); i_layer++)
    {
        TextureLayer& curr_layer = m_impl->layers.getReference(i_layer);

        glActiveTexture(GL_TEXTURE0 + i_layer);
        curr_layer.gl_sampler->unuse(i_layer);
        curr_layer.gl_texture->unuse();
    }

    glActiveTexture(GL_TEXTURE0);
    m_impl->program->unuse();
}


TREEFACE_NAMESPACE_END
