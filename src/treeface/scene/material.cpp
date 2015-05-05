#include "treeface/scene/material.h"

#include "treeface/image.h"
#include "treeface/imagemanager.h"

#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"

#include "treeface/private/material_private.h"

#include "treeface/packagemanager.h"
#include "treeface/gl/programmanager.h"
#include "treeface/misc/stringcast.h"

#include <treejuce/Logger.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>
#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Material::Material(): m_impl(new Impl())
{
}

Material::~Material()
{
    if (m_impl)
        delete m_impl;
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
