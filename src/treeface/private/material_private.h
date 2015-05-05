#ifndef TREEFACE_MATERIAL_PRIVATE_H
#define TREEFACE_MATERIAL_PRIVATE_H

#include "treeface/gl/sampler.h"
#include "treeface/gl/texture.h"

#include "treeface/scene/material.h"

#include <treejuce/Array.h>
#include <treejuce/Holder.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN

struct TextureLayer
{
    treejuce::String name;
    treejuce::Holder<Texture> gl_texture;
    treejuce::Holder<Sampler> gl_sampler;
    GLint layer;
    GLint program_uniform_idx;
};

struct Material::Impl
{
    treejuce::Array<TextureLayer> layers;
    treejuce::Holder<Program> program;
    bool translucent = false;
    bool project_shadow = true;
    bool receive_shadow = true;
    GLint i_uni_trans = -1;
    GLint i_uni_norm_trans = -1;
    GLint i_uni_proj_trans = -1;
    GLint i_uni_light_direct = -1;
    GLint i_uni_light_color = -1;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MATERIAL_PRIVATE_H
