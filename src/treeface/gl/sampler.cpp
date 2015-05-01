#include "treeface/gl/sampler.h"

#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Sampler::Sampler()
{
    glGenSamplers(1, &m_sampler);
    if (!m_sampler)
        die("failed to allocate sampler ID");
}

Sampler::~Sampler()
{
    if (m_sampler)
        glDeleteSamplers(1, &m_sampler);
}

TREEFACE_NAMESPACE_END
