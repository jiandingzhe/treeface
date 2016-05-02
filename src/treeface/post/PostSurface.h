#ifndef TREEFACE_POST_SURFACE_H
#define TREEFACE_POST_SURFACE_H

#include <treecore/RefCountObject.h>

#include "treecore/ClassUtils.h"
#include "treeface/gl/Enums.h"

namespace treeface
{

class PostNetwork;
class PostProcess;
class PostTarget;
class Framebuffer;

class PostSurface: public treecore::RefCountObject
{
    friend class PostNetwork;
    friend class PostTarget;
    friend class PostProcess;

public:
    PostSurface();

    TREECORE_DECLARE_NON_COPYABLE( PostSurface )
    TREECORE_DECLARE_NON_MOVABLE( PostSurface )

    virtual ~PostSurface();

protected:
    virtual void attach_to( Framebuffer* fbo, GLFramebufferAttachment attach ) = 0;

    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_POST_SURFACE_H
