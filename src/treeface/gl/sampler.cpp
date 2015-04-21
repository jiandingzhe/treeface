#include "treeface/gl/sampler.h"

#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Sampler::~Sampler()
{
    glGenSamplers(1, &m_sampler);
    if (!m_sampler)
        die("failed to allocate sampler ID");
}


TREEFACE_NAMESPACE_END
