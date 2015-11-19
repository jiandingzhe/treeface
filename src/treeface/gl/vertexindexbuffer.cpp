#include "treeface/gl/vertexindexbuffer.h"

#include <treecore/StringRef.h>

using namespace treecore;

namespace treeface {

VertexIndexBuffer::VertexIndexBuffer()
{
    glGenBuffers(1, &m_buffer_vtx);
    if (!m_buffer_vtx)
        die("failed to create vertex buffer");

    glGenBuffers(1, &m_buffer_idx);
    if (!m_buffer_idx)
        die("failed to create index buffer");
}

VertexIndexBuffer::~VertexIndexBuffer()
{
    if (m_buffer_vtx)
        glDeleteBuffers(1, &m_buffer_vtx);
    if (m_buffer_idx)
        glDeleteBuffers(1, &m_buffer_idx);
}

void VertexIndexBuffer::upload_data() const noexcept
{
    glBufferData(GL_ARRAY_BUFFER,
                 m_data_vtx.getSize(),
                 m_data_vtx.getData(),
                 GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(treecore::uint16) * m_data_idx.size(),
                 m_data_idx.getRawDataConstPointer(),
                 GL_STATIC_DRAW);
}


} // namespace treeface
