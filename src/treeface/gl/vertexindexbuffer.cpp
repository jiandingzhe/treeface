#include "treeface/gl/vertexindexbuffer.h"

TREEFACE_NAMESPACE_BEGIN

VertexIndexBuffer::VertexIndexBuffer()
{
    glGenBuffers(1, &m_buffer_vtx);
    glGenBuffers(1, &m_buffer_idx);
}

VertexIndexBuffer::~VertexIndexBuffer()
{
    glDeleteBuffers(1, &m_buffer_vtx);
    glDeleteBuffers(1, &m_buffer_idx);
}

TREEFACE_NAMESPACE_END
