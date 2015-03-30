#ifndef TREEFACE_GL_VERTEX_INDEX_BUFFER_H
#define TREEFACE_GL_VERTEX_INDEX_BUFFER_H

#include "treeface/common.h"

#include "treejuce/Array.h"
#include "treejuce/Object.h"
#include "treejuce/Result.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class VertexArray;

class VertexIndexBuffer: public treejuce::Object
{
    friend class VertexArray;

public:
    VertexIndexBuffer();

    VertexIndexBuffer(const VertexIndexBuffer& other) = delete;
    VertexIndexBuffer(VertexIndexBuffer&& other) = delete;
    VertexIndexBuffer& operator = (const VertexIndexBuffer& other) = delete;
    VertexIndexBuffer& operator = (VertexIndexBuffer&& other) = delete;

    virtual ~VertexIndexBuffer();

    template<typename T_VTX, typename T_IDX>
    treejuce::Result load_data(const treejuce::Array<T_VTX>& data_vertex,
                               const treejuce::Array<T_IDX>& data_index);

    template<typename T_VTX, typename T_IDX>
    treejuce::Result load_data(const T_VTX* data_vertex, size_t n_vertex,
                               const T_IDX* data_index, size_t n_index);
protected:
    GLuint m_buffer_vtx = 0;
    GLuint m_buffer_idx = 0;
};

template<typename T_VTX, typename T_IDX>
treejuce::Result VertexIndexBuffer::load_data(const treejuce::Array<T_VTX>& data_vertex,
                                              const treejuce::Array<T_IDX>& data_index)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vtx);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(T_VTX) * data_vertex.size(),
                 data_vertex.getRawDataPointer(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_idx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(T_IDX) * data_index.size(),
                 data_index.getRawDataPointer(),
                 GL_STATIC_DRAW);
}

template<typename T_VTX, typename T_IDX>
treejuce::Result VertexIndexBuffer::load_data(const T_VTX* data_vertex, size_t n_vertex,
                                              const T_IDX* data_index, size_t n_index)
{
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
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_VERTEX_INDEX_BUFFER_H
