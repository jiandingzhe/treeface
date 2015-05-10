#ifndef TREEFACE_GL_VERTEX_INDEX_BUFFER_H
#define TREEFACE_GL_VERTEX_INDEX_BUFFER_H

#include "treeface/common.h"

#include "treeface/gl/vertexattrib.h"

#include <treejuce/Array.h>
#include <treejuce/ArrayRef.h>
#include <treejuce/IntTypes.h>
#include <treejuce/MemoryBlock.h>
#include <treejuce/Object.h>

#include <GL/glew.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

class TestFramework;

TREEFACE_NAMESPACE_BEGIN
class VertexArray;
class Geometry;

/**
 * @brief The VertexIndexBuffer class holds a OpenGL vertex buffer object and an
 *        OpenGL index buffer object.
 *
 * The format of vertex buffer is not limited and is treated as a chunk of raw
 * bytes. You may set any data inside it(see VertexTemplate class). The format
 * of index buffer is unsigned short, as I think 65536 indices should be enough.
 *
 * This class also has host-side data buffers, and automatically synchronize
 * them between host and device when they are modified.
 */
class VertexIndexBuffer: public treejuce::Object
{
    friend class VertexArray;
    friend class Geometry;
    friend class ::TestFramework;

public:
    /**
     * @brief create an empty object, allocate OpenGL buffers.
     */
    VertexIndexBuffer();

    // disable copy and move
    JUCE_DECLARE_NON_COPYABLE(VertexIndexBuffer);
    JUCE_DECLARE_NON_MOVABLE(VertexIndexBuffer);

    /**
     * @brief the underlying GL buffer object will also be released.
     */
    virtual ~VertexIndexBuffer();

    /**
     * @brief bind buffer objects to array buffer and element array buffer
     */
    inline void use() const NOEXCEPT
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vtx);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_idx);
    }

    /**
     * @brief bind zero to array buffer and element array buffer
     */
    inline static void unuse() NOEXCEPT
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    /**
     * @brief set host-side data, mark data dirty.
     */
    template<typename VTX_T>
    inline void set_host_data(treejuce::ArrayRef<VTX_T> data_vtx, treejuce::ArrayRef<const treejuce::uint16> data_idx) NOEXCEPT
    {
        m_data_vtx.replaceWith(data_vtx.get_const_data(), sizeof(VTX_T) * data_vtx.size());
        m_data_idx.resize(data_idx.size());
        memcpy(m_data_idx.getRawDataPointer(), data_idx.get_const_data(), data_idx.size() * sizeof(treejuce::uint16));
        m_data_changed = true;
    }

    /**
     * @brief call draw command on current vertex and index data. The use()
     * method must be called before calling this method.
     *
     * @param primitive: OpenGL primitive type.
     */
    inline void draw(GLenum primitive) NOEXCEPT
    {
        if (m_data_changed)
        {
            upload_data();
            m_data_changed = false;
        }
        glDrawElements(primitive, m_data_idx.size(), GL_UNSIGNED_SHORT, nullptr);
    }

    /**
     * @brief get OpenGL vertex buffer object ID.
     * @return the buffer object ID.
     */
    inline GLuint get_vertex_buffer_id() const NOEXCEPT
    {
        return m_buffer_vtx;
    }

    /**
     * @brief get OpenGL index buffer object ID.
     * @return the buffer object ID.
     */
    inline GLuint get_index_buffer_id() const NOEXCEPT
    {
        return m_buffer_idx;
    }

protected:
    void upload_data() const NOEXCEPT;

    bool m_data_changed = false;
    treejuce::MemoryBlock m_data_vtx;
    treejuce::Array<treejuce::uint16> m_data_idx;
    GLuint m_buffer_vtx = 0;
    GLuint m_buffer_idx = 0;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_VERTEX_INDEX_BUFFER_H
