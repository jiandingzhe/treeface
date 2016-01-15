#ifndef TREEFACE_GL_SAMPLER_H
#define TREEFACE_GL_SAMPLER_H

#include "treeface/common.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <treecore/RefCountObject.h>
#include <treecore/PlatformDefs.h>
#include <treecore/Result.h>

namespace treeface {

struct Sampler: public treecore::RefCountObject
{
    Sampler();
    virtual ~Sampler();

    // invalidate copy and move
    TREECORE_DECLARE_NON_COPYABLE( Sampler );
    TREECORE_DECLARE_NON_MOVABLE( Sampler );

    void bind( GLuint unit ) const noexcept
    {
        glBindSampler( unit, m_sampler );
    }

    static void unbind( GLuint unit ) noexcept
    {
        glBindSampler( unit, 0 );
    }

    GLuint m_sampler = 0;
};

} // namespace treeface

#endif // TREEFACE_GL_SAMPLER_H
