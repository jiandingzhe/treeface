#include "treeface/scene/Material.h"

#include "treeface/graphics/Image.h"
#include "treeface/graphics/ImageManager.h"

#include "treeface/gl/ImageRef.h"
#include "treeface/gl/Program.h"
#include "treeface/Config.h"

#include "treeface/scene/guts/Material_guts.h"

#include "treeface/base/PackageManager.h"
#include "treeface/misc/StringCast.h"

#include <treecore/Logger.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>
#include <treecore/String.h>
#include <treecore/StringRef.h>

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

Material::Material(): m_impl( new Impl() )
{}

Material::~Material()
{
    if (m_impl)
        delete m_impl;
}

Program* Material::get_program() const noexcept
{
    return m_program.get();
}

int32 Material::get_num_textures() const noexcept
{
    return m_impl->layers.size();
}

Texture* Material::get_texture( treecore::int32 layer_idx ) const noexcept
{
    if ( layer_idx >= m_impl->layers.size() )
        return nullptr;
    else
        return m_impl->layers[layer_idx].gl_texture.get();
}

Texture* Material::get_texture( const Identifier& uniform_name ) const noexcept
{
    for (int i = 0; i < m_impl->layers.size(); i++)
    {
        if (m_impl->layers[i].uniform_name == uniform_name)
            return m_impl->layers[i].gl_texture.get();
    }

    return nullptr;
}

void Material::bind() noexcept
{
    // use textures
    for (int i_layer = 0; i_layer < m_impl->layers.size(); i_layer++)
    {
        TextureLayer& curr_layer = m_impl->layers[i_layer];

        glActiveTexture( GL_TEXTURE0 + i_layer );
        curr_layer.gl_texture->bind();
    }

    // use program
    glActiveTexture(GL_TEXTURE0);
    m_program->bind();

    // set samplers to program
    for (int i_layer = 0; i_layer < m_impl->layers.size(); i_layer++)
    {
        TextureLayer& curr_layer = m_impl->layers[i_layer];
        m_program->set_uniform( curr_layer.program_uniform_loc, i_layer );
    }
}

void Material::unbind() noexcept
{
    for (int i_layer = 0; i_layer < m_impl->layers.size(); i_layer++)
    {
        TextureLayer& curr_layer = m_impl->layers[i_layer];

        glActiveTexture( GL_TEXTURE0 + i_layer );
        glBindSampler( i_layer, 0 );
        curr_layer.gl_texture->unbind();
    }

    glActiveTexture( GL_TEXTURE0 );
    m_program->unbind();
}

treecore::String Material::get_shader_source_addition() const noexcept
{
    return TREEFACE_GLSL_VERSION_DEF "\n";
}

} // namespace treeface
