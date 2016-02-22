#ifndef TREEFACE_TEXTURE_POST_SURFACE_GUTS_H
#define TREEFACE_TEXTURE_POST_SURFACE_GUTS_H

#include "treeface/post/TexturePostSurface.h"

#include <treecore/RefCountHolder.h>

namespace treeface
{

struct TexturePostSurface::Guts
{
    treecore::RefCountHolder<Texture> texture;
    union
    {
        int slice;
        GLTextureCubeSide cube_side;
    };
};

} // namespace treeface

#endif // TREEFACE_TEXTURE_POST_SURFACE_GUTS_H
