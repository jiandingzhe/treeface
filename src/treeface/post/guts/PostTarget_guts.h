#ifndef TREEFACE_POST_TARGET_GUTS_H
#define TREEFACE_POST_TARGET_GUTS_H

#include "treeface/post/PostTarget.h"

#include "treeface/gl/Framebuffer.h"

#include "treeface/post/PostProcess.h"
#include "treeface/post/PostSurface.h"

#include <treecore/HashMap.h>
#include <treecore/Identifier.h>

namespace treeface
{

struct PostTarget::Guts
{
    PostProcess* producer_process = nullptr;
    treecore::RefCountHolder<Framebuffer> fbo = new Framebuffer();
    treecore::HashMap<treecore::Identifier, PostSurface*> surfaces;
};

} // namespace treeface

#endif // TREEFACE_POST_TARGET_GUTS_H
