#ifndef TREEFACE_MATERIAL_PRIVATE_H
#define TREEFACE_MATERIAL_PRIVATE_H

#include "treeface/gl/Texture.h"

#include "treeface/scene/Material.h"

#include <treecore/Array.h>
#include <treecore/Identifier.h>
#include <treecore/RefCountHolder.h>

namespace treeface {

struct TextureLayer
{
    treecore::Identifier uniform_name;
    treecore::RefCountHolder<Texture> gl_texture;
    GLint program_uniform_loc;
};

struct FragDataLoc
{
    treecore::Identifier name;
    GLint location;
};

struct Material::Impl
{
    treecore::Array<TextureLayer> layers;

    treecore::Array<FragDataLoc> output_colors;
    bool output_managed = false;
    bool output_depth = false;
    bool output_stencil = false;
};

} // namespace treeface

#endif // TREEFACE_MATERIAL_PRIVATE_H
