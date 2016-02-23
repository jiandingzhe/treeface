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

struct Material::Impl
{
    treecore::Array<TextureLayer> layers;
};

} // namespace treeface

#endif // TREEFACE_MATERIAL_PRIVATE_H
