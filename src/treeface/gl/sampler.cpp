#include "treeface/gl/sampler.h"

#include <treecore/StringRef.h>

using namespace treecore;

namespace treeface {

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

} // namespace treeface
