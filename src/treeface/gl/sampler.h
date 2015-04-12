#ifndef TREEFACE_GL_SAMPLER_H
#define TREEFACE_GL_SAMPLER_H

#include "treeface/common.h"

#include <GL/glew.h>

#include <treejuce/Object.h>
#include <treejuce/PlatformDefs.h>

TREEFACE_NAMESPACE_BEGIN

struct Sampler: public treejuce::Object
{
    Sampler(GLenum unit): m_unit(unit - GL_TEXTURE0) {}
    virtual ~Sampler();

    // invalidate copy and move
    Sampler(const Sampler& other) = delete;
    Sampler(Sampler&& other) = delete;
    Sampler& operator = (const Sampler& other) = delete;
    Sampler& operator = (Sampler&& other) = delete;

    void init() NOEXCEPT;

    void use() const NOEXCEPT
    {
        glBindSampler(m_unit, m_sampler);
    }

    void unuse() const NOEXCEPT
    {
        glBindSampler(m_unit, 0);
    }

    GLuint m_unit = -1;
    GLuint m_sampler = 0;
};


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_SAMPLER_H
