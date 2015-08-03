#ifndef TREEFACE_GL_SAMPLER_H
#define TREEFACE_GL_SAMPLER_H

#include "treeface/common.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <treecore/Object.h>
#include <treecore/PlatformDefs.h>
#include <treecore/Result.h>

namespace treeface {

struct Sampler: public treecore::Object
{
    Sampler();
    virtual ~Sampler();

    // invalidate copy and move
    JUCE_DECLARE_NON_COPYABLE(Sampler);
    JUCE_DECLARE_NON_MOVABLE(Sampler);

    void use(GLuint unit) const NOEXCEPT
    {
        glBindSampler(unit, m_sampler);
    }

    static void unuse(GLuint unit) NOEXCEPT
    {
        glBindSampler(unit, 0);
    }

    GLuint m_sampler = 0;
};


} // namespace treeface

#endif // TREEFACE_GL_SAMPLER_H
