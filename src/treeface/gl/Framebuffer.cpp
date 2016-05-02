#include "treeface/gl/Framebuffer.h"

#include "treeface/gl/Errors.h"
#include "treeface/gl/Texture.h"

using namespace treecore;

namespace treeface
{

Framebuffer::Framebuffer()
{
    glGenFramebuffers( 1, &m_handle );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_handle );
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers( 1, &m_handle );
}

void _check_error_( const String& msg )
{
    GLenum err = glGetError();
    switch (err)
    {
    case GL_INVALID_ENUM:
        throw GLInvalidEnum( msg );
    case GL_INVALID_VALUE:
        throw GLInvalidValue( msg );
    case GL_INVALID_OPERATION:
        throw GLInvalidOperation( msg );
    }
}

void Framebuffer::attach_2d_texture( GLFramebufferAttachment attach, Texture* texture ) const
{
    treecore_assert( is_bound_to_draw() );
    treecore_assert( texture->m_type == TFGL_TEXTURE_2D );
    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, attach, TFGL_TEXTURE_2D, texture->m_texture, 0 );
    TREECORE_DBGCODE( _check_error_( "failed to attach 2D texture" ) );
}

void Framebuffer::attach_cube_texture( GLFramebufferAttachment attach, Texture* texture, GLTextureCubeSide side ) const
{
    treecore_assert( is_bound_to_draw() );
    treecore_assert( texture->m_type == TFGL_TEXTURE_CUBE );
    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, attach, side, texture->m_texture, 0 );
    TREECORE_DBGCODE( _check_error_( "failed to attach cube texture" ) );
}

void Framebuffer::attach_texture_layer( GLFramebufferAttachment attach, Texture* texture, GLint layer ) const
{
    treecore_assert( is_bound_to_draw() );
    treecore_assert( texture->m_type == TFGL_TEXTURE_2D_ARRAY || texture->m_type == TFGL_TEXTURE_3D );
    glFramebufferTextureLayer( GL_DRAW_FRAMEBUFFER, attach, texture->m_texture, 0, layer );
    TREECORE_DBGCODE( _check_error_( "failed to attach one layer of 3D or array texture" ) );
}

} // namespace treeface
