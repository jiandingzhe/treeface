#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/common.h"

#include "treeface/gl/vertexattrib.h"

#include <treecore/ArrayRef.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>
#include <treecore/String.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

class VertexIndexBuffer;
class Program;

class VertexTemplate;

/**
 * @brief The VertexArray struct wraps OpenGL vertex array object (the VAO).
 *
 * It is a bridge between vertex buffer data and program vertex attributes.
 */
struct VertexArray: public treecore::RefCountObject
{
    /**
     * @brief create an empty VertexArray object. OpenGL VAO ID will be
     *        allocated.
     */
    VertexArray();

    /**
     * @brief The underlying VAO ID will also be released.
     */
    virtual ~VertexArray();

    // disable copy and move
    TREECORE_DECLARE_NON_COPYABLE(VertexArray);
    TREECORE_DECLARE_NON_MOVABLE(VertexArray);

    /**
     * @brief build vertex array
     *
     * If vertex info contain any attribute that the program don't have, it will
     * be simply ignored.
     *
     * @param buffer: a VertexIndexBuffer object, from which the vertex buffer
     *                will be bind.
     * @param vertex_info: it holds a series of vertex attribute metadata.
     * @param program: it should have been compiled and linked.
     *
     * @return ok if success, fail if any error occurred.
     */
    treecore::Result build(const VertexIndexBuffer* buffer,
                           const VertexTemplate& vertex_info,
                           const Program* program);

    /**
     * @brief build one vertex attribute
     *
     * If this vertex attribute is not exist in the program, it will be ignored.
     *
     * @param buffer: a VertexIndexBuffer object, from which the vertex buffer
     *                will be bind.
     * @param attrib: the vertex attribute to be bind.
     * @param stride: the size in bytes of one vertex.
     * @param program: it should have been compiled and linked.
     *
     * @return ok if success, fail if any error occurred.
     */
    treecore::Result build_one(const VertexIndexBuffer* buffer,
                               HostVertexAttrib attrib,
                               GLsizei stride,
                               const Program* program);

    /**
     * @brief bind the underlying GL VAO
     */
    inline void use() const noexcept
    {
        glBindVertexArray(m_array);
    }

    /**
     * @brief bind zero
     */
    inline static void unuse() noexcept
    {
        glBindVertexArray(0);
    }

    /**
     * @brief the index of OpenGL vertex array object
     */
    GLuint m_array = 0;
};

} // namespace treeface

#endif // TREEFACE_GL_VERTEX_ARRAY_H
