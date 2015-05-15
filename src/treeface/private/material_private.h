#ifndef TREEFACE_MATERIAL_PRIVATE_H
#define TREEFACE_MATERIAL_PRIVATE_H

#include "treeface/gl/sampler.h"
#include "treeface/gl/texture.h"

#include "treeface/material.h"

#include <treejuce/Array.h>
#include <treejuce/Holder.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN

struct TextureLayer
{
    treejuce::String name;
    treejuce::Holder<Texture> gl_texture;
    treejuce::Holder<Sampler> gl_sampler;
    GLint program_uniform_loc;
};

struct Material::Impl
{
    treejuce::Array<TextureLayer> layers;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MATERIAL_PRIVATE_H
