#ifndef TREEFACE_FRAMEBUFFER_H
#define TREEFACE_FRAMEBUFFER_H

#include "treeface/gl/Enums.h"

#include <treecore/RefCountObject.h>

namespace treeface
{

class Texture;

class Framebuffer: public treecore::RefCountObject
{
public:
    Framebuffer();

    TREECORE_DECLARE_NON_COPYABLE( Framebuffer );
    TREECORE_DECLARE_NON_MOVABLE( Framebuffer );

    virtual ~Framebuffer();

    void bind_draw() const noexcept { glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_handle ); }
    void bind_read() const noexcept { glBindFramebuffer( GL_READ_FRAMEBUFFER, m_handle ); }

    bool is_bound_to_draw() const noexcept { return m_handle == get_current_bound_draw(); }
    bool is_bound_to_read() const noexcept { return m_handle == get_current_bound_read(); }

    void attach_2d_texture( GLFramebufferAttachment attach, Texture* texture ) const;
    void attach_cube_texture( GLFramebufferAttachment attach, Texture* texture, GLTextureCubeSide side ) const;
    void attach_texture_layer( GLFramebufferAttachment attach, Texture* texture, GLint layer ) const;

    void clear() noexcept
    {
        jassert( is_bound_to_draw() );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    }

    GLuint get_gl_handle() const noexcept { return m_handle; }

    static void bind_draw_to_default() noexcept { glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ); }
    static void bind_read_to_default() noexcept { glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ); }

    static GLuint get_current_bound_draw() noexcept
    {
        GLint result = -1;
        glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &result );
        return GLuint( result );
    }

    static GLuint get_current_bound_read() noexcept
    {
        GLint result = -1;
        glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &result );
        return GLuint( result );
    }

protected:
    GLuint m_handle = 0;
};

} // namespace treeface

#endif // TREEFACE_FRAMEBUFFER_H
