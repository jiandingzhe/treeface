#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/base/Common.h"

#include "treeface/gl/GLBuffer.h"
#include "treeface/gl/VertexTemplate.h"

#include <treecore/ArrayRef.h>
#include <treecore/RefCountHolder.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>
#include <treecore/String.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

class Program;
class GLBuffer;
class VertexTemplate;

/**
 * @brief The VertexArray struct wraps OpenGL vertex array object (the VAO).
 *
 * It is a bridge between vertex buffer data and program vertex attributes.
 */
class VertexArray: public treecore::RefCountObject
{
public:
    class BindScope
    {
    public:
        BindScope( VertexArray& vertex_array ): m_array( vertex_array ) { m_array.bind();   }
        ~BindScope()                                                    { m_array.unbind(); }
    private:
        VertexArray& m_array;
    };

    /**
     * @brief
     *        allocated.
     *
     * If vertex info contain any attribute that the program don't have, it will
     * be simply ignored.
     *
     * @param buffer: a VertexIndexBuffer object, from which the vertex buffer
     *                will be bind.
     * @param vertex_info: it holds a series of vertex attribute metadata.
     * @param program: it should have been compiled and linked.
     */

    ///
    /// \brief create OpenGL vertex array object
    ///
    /// \param vertex_buffer
    /// \param index_buffer
    /// \param vertex_info    metadata for vertex composition
    /// \param program        vertex attribute will be fetched from this program
    ///
    VertexArray( GLBuffer* vertex_buffer,
                 GLBuffer* index_buffer,
                 const VertexTemplate& vertex_info,
                 Program* program );

    /**
     * @brief The underlying VAO object will also be released.
     */
    virtual ~VertexArray();

    // disable copy and move
    TREECORE_DECLARE_NON_COPYABLE( VertexArray );
    TREECORE_DECLARE_NON_MOVABLE( VertexArray );

    /**
     * @brief bind the underlying GL VAO
     */
    void bind() const noexcept
    {
        glBindVertexArray( m_array );
    }

    /**
     * @brief bind zero
     */
    static void unbind() noexcept
    {
        glBindVertexArray( 0 );
    }

    bool is_bound() const noexcept { return get_current_bound_vertex_array() == m_array; }

    void draw( GLPrimitive primitive, GLsizei num_idx ) noexcept
    {
        jassert( is_bound() );
        jassert( num_idx >= 0 );
        glDrawElements( primitive, num_idx, GLTypeEnumHelper<IndexType>::value, nullptr );
    }

    GLuint get_gl_handle() const noexcept { return m_array; }

    const VertexTemplate& get_vertex_template() noexcept { return m_vtx_info; }
    GLBuffer*             get_vertex_buffer()   noexcept { return m_buf_vtx; }
    GLBuffer*             get_index_buffer()    noexcept { return m_buf_idx; }
    Program*              get_program()         noexcept { return m_program; }

    static GLuint get_current_bound_vertex_array();
protected:
    /**
     * @brief the index of OpenGL vertex array object
     */
    GLuint m_array = 0;
    const VertexTemplate m_vtx_info;
    treecore::RefCountHolder<GLBuffer> m_buf_vtx;
    treecore::RefCountHolder<GLBuffer> m_buf_idx;
    treecore::RefCountHolder<Program>  m_program;
};

} // namespace treeface

#endif // TREEFACE_GL_VERTEX_ARRAY_H
