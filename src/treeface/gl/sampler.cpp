#include "treeface/gl/sampler.h"

#include <treejuce/StringRef.h>

TREEFACE_NAMESPACE_BEGIN

Sampler::~Sampler()
{
    if (m_sampler)
        glDeleteSamplers(1, &m_sampler);
}

void Sampler::init() NOEXCEPT
{
    glGenSamplers(1, &m_sampler);
    if (!m_sampler)
        die("failed to generate sampler");
}

TREEFACE_NAMESPACE_END
