#ifndef TREEFACE_MATERIAL_PRIVATE_H
#define TREEFACE_MATERIAL_PRIVATE_H

#include "treeface/gl/sampler.h"
#include "treeface/gl/texture.h"

#include "treeface/material.h"

#include <treecore/Array.h>
#include <treecore/Identifier.h>
#include <treecore/RefCountHolder.h>

namespace treeface {

struct TextureLayer
{
    treecore::Identifier name;
    treecore::RefCountHolder<Texture> gl_texture;
    treecore::RefCountHolder<Sampler> gl_sampler;
    GLint program_uniform_loc;
};

struct Material::Impl
{
    treecore::Array<TextureLayer> layers;
};

} // namespace treeface

#endif // TREEFACE_MATERIAL_PRIVATE_H
