#include "treeface/gl/buffer.h"
#include "treeface/common.h"

using namespace treecore;

namespace treeface
{

Buffer::Buffer(GLenum type, GLenum policy)
    : m_type(type)
    , m_policy(policy)
{
    glGenBuffers(1, &m_buffer);
    if (!m_buffer)
        die("failed to create buffer");
}

} // namespace treeface
