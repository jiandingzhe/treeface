#include "treeface/gl/vertexindexbuffer.h"

#include <treejuce/StringRef.h>

TREEFACE_NAMESPACE_BEGIN

VertexIndexBuffer::VertexIndexBuffer()
{
}

VertexIndexBuffer::~VertexIndexBuffer()
{
    if (m_buffer_vtx)
        glDeleteBuffers(1, &m_buffer_vtx);
    if (m_buffer_idx)
        glDeleteBuffers(1, &m_buffer_idx);
}

void VertexIndexBuffer::init() NOEXCEPT
{
    glGenBuffers(1, &m_buffer_vtx);
    if (!m_buffer_vtx)
        die("failed to create vertex buffer");
    glGenBuffers(1, &m_buffer_idx);
    if (!m_buffer_idx)
        die("failed to create index buffer");
}

TREEFACE_NAMESPACE_END
