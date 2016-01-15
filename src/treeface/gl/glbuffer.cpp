#include "treeface/gl/glbuffer.h"
#include "treeface/common.h"

using namespace treecore;

namespace treeface
{

GLBuffer::GLBuffer( GLBufferType type, GLBufferUsage usage )
    : m_type( type )
    , m_usage( usage )
{
    glGenBuffers( 1, &m_buffer );
    if (!m_buffer)
        die( "failed to create buffer" );
    glBindBuffer( m_type, m_buffer );
}

GLBuffer::~GLBuffer()
{
    if (m_buffer)
        glDeleteBuffers( 1, &m_buffer );
}

GLuint GLBuffer::get_current_bound_buffer( GLBufferType type )
{
    GLenum pname  = -1;
    GLint  result = -1;

    switch (type)
    {
    case TFGL_BUFFER_INDEX:    pname = GL_ELEMENT_ARRAY_BUFFER_BINDING;      break;
    case TFGL_BUFFER_VERTEX:   pname = GL_ARRAY_BUFFER_BINDING;              break;
    case TFGL_BUFFER_UNIFORM:  pname = GL_UNIFORM_BUFFER_BINDING;            break;
    case TFGL_BUFFER_READ:     pname = GL_COPY_READ_BUFFER_BINDING;          break;
    case TFGL_BUFFER_WRITE:    pname = GL_COPY_WRITE_BUFFER_BINDING;         break;
    case TFGL_BUFFER_FEEDBACK: pname = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING; break;
    case TFGL_BUFFER_PACK:     pname = GL_PIXEL_PACK_BUFFER_BINDING;         break;
    case TFGL_BUFFER_UNPACK:   pname = GL_PIXEL_UNPACK_BUFFER_BINDING;       break;
    }

    glGetIntegerv( pname, &result );
    return GLuint( result );
}

} // namespace treeface
