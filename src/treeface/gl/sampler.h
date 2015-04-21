#ifndef TREEFACE_GL_SAMPLER_H
#define TREEFACE_GL_SAMPLER_H

#include "treeface/common.h"

#include <GL/glew.h>

#include <treejuce/Object.h>
#include <treejuce/PlatformDefs.h>
#include <treejuce/Result.h>

TREEFACE_NAMESPACE_BEGIN

struct Sampler: public treejuce::Object
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

    void unuse(GLuint unit) const NOEXCEPT
    {
        glBindSampler(unit, 0);
    }

    GLuint m_sampler = 0;
};


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_SAMPLER_H
