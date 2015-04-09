#ifndef TREEFACE_GL_VERTEX_INDEX_BUFFER_H
#define TREEFACE_GL_VERTEX_INDEX_BUFFER_H

#include "treeface/common.h"

#include <treejuce/Array.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

struct VertexIndexBuffer: public treejuce::Object
{
    VertexIndexBuffer();

    // disable copy and move
    VertexIndexBuffer(const VertexIndexBuffer& other) = delete;
    VertexIndexBuffer(VertexIndexBuffer&& other) = delete;
    VertexIndexBuffer& operator = (const VertexIndexBuffer& other) = delete;
    VertexIndexBuffer& operator = (VertexIndexBuffer&& other) = delete;

    virtual ~VertexIndexBuffer();

    void init() NOEXCEPT;
    template<typename T_VTX, typename T_IDX>
    treejuce::Result load_data(const treejuce::Array<T_VTX>& data_vertex,
                               const treejuce::Array<T_IDX>& data_index);

    template<typename T_VTX, typename T_IDX>
    treejuce::Result load_data(const T_VTX* data_vertex, size_t n_vertex,
                               const T_IDX* data_index, size_t n_index);

    inline void use() const NOEXCEPT
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vtx);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_idx);
    }

    inline void unuse() const NOEXCEPT
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline void draw(GLenum primitive, GLsizei n_index, GLenum index_type) const NOEXCEPT
    {
        glDrawElements(primitive, n_index, index_type, nullptr);
    }

    GLuint m_buffer_vtx = 0;
    GLuint m_buffer_idx = 0;
};

template<typename T_VTX, typename T_IDX>
treejuce::Result VertexIndexBuffer::load_data(const treejuce::Array<T_VTX>& data_vertex,
                                              const treejuce::Array<T_IDX>& data_index)
{
    return load_data(data_vertex.getRawDataPointer(), data_vertex.size(),
                     data_index.getRawDataPointer(),  data_index.size());
}

template<typename T_VTX, typename T_IDX>
treejuce::Result VertexIndexBuffer::load_data(const T_VTX* data_vertex, size_t n_vertex,
                                              const T_IDX* data_index, size_t n_index)
{
    if (!m_buffer_vtx || !m_buffer_idx)
        return treejuce::Result::fail("vertex index buffer not initialized");

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vtx);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(T_VTX) * n_vertex,
                 data_vertex,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_idx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(T_IDX) * n_index,
                 data_index,
                 GL_STATIC_DRAW);

    return treejuce::Result::ok();
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_VERTEX_INDEX_BUFFER_H
