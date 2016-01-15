#ifndef TREEFACE_GL_BUFFER_H
#define TREEFACE_GL_BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <treecore/ArrayRef.h>
#include <treecore/MemoryBlock.h>
#include <treecore/RefCountObject.h>

#include "treeface/enums.h"

#include "treeface/gl/enums.h"

namespace treeface
{

class GLBuffer: public treecore::RefCountObject
{
public:
    class BindScope
    {
    public:
        BindScope( GLBuffer& buffer ): m_buffer( buffer ) { m_buffer.bind();   }
        ~BindScope()                                      { m_buffer.unbind(); }
    private:
        GLBuffer& m_buffer;
    };

    GLBuffer() = delete;
    GLBuffer( GLBufferType type, GLBufferUsage policy );

    TREECORE_DECLARE_NON_COPYABLE( GLBuffer );
    TREECORE_DECLARE_NON_MOVABLE( GLBuffer );

    virtual ~GLBuffer();

    void bind()   const { glBindBuffer( m_type, m_buffer ); }
    void unbind() const { glBindBuffer( m_type, 0 ); }

    bool is_bound() const
    {
        return get_current_bound_buffer( m_type ) == m_buffer;
    }

    void upload_data( const void* data, GLsizei num_byte )
    {
        jassert( get_current_bound_buffer( m_type ) == m_buffer );
        glBufferData( m_type, num_byte, data, m_usage );
    }

    template<typename T>
    void upload_data( const treecore::ArrayRef<T>& data )
    {
        upload_data( data.get_data(), data.size() * sizeof(T) );
    }

    void upload_data( const treecore::MemoryBlock& data )
    {
        upload_data( data.getData(), GLsizei( data.getSize() ) );
    }

    GLuint get_gl_handle() const noexcept { return m_buffer; }

    static GLuint get_current_bound_buffer( GLBufferType type );

protected:
    GLBufferType  m_type;
    GLBufferUsage m_usage;
    GLuint        m_buffer = 0;
};

} // namespace treeface

#endif // TREEFACE_GL_BUFFER_H
