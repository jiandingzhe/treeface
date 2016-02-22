#include "treeface/post/TexturePostSurface.h"
#include "treeface/post/guts/TexturePostSurface_guts.h"

#include "treeface/gl/Framebuffer.h"

namespace treeface
{

TexturePostSurface::TexturePostSurface( GLsizei width, GLsizei height, GLsizei levels, GLInternalImageFormat internal_fmt )
    : m_guts( new Guts() )
{
    m_guts->texture = new Texture( width, height, levels, internal_fmt );
}

TexturePostSurface::TexturePostSurface( Texture* texture )
    : m_guts( new Guts() )
{
    m_guts->texture = texture;
}

TexturePostSurface::TexturePostSurface( Texture* texture, GLTextureCubeSide side )
    : m_guts( new Guts() )
{
    m_guts->texture   = texture;
    m_guts->cube_side = side;
}

TexturePostSurface::TexturePostSurface( Texture* texture, int32 slice )
    : m_guts( new Guts() )
{
    m_guts->texture = texture;
    m_guts->slice   = slice;
}

TexturePostSurface::~TexturePostSurface()
{
    delete TexturePostSurface::m_guts;
}

void TexturePostSurface::attach_to( Framebuffer* fbo, GLFramebufferAttachment attach )
{
    switch ( m_guts->texture->get_type() )
    {
    case TFGL_TEXTURE_2D:
        fbo->attach_2d_texture( attach, m_guts->texture );
        break;
    case TFGL_TEXTURE_CUBE:
        fbo->attach_cube_texture( attach, m_guts->texture, m_guts->cube_side );
        break;
    case TFGL_TEXTURE_2D_ARRAY:
    case TFGL_TEXTURE_3D:
        fbo->attach_texture_layer( attach, m_guts->texture, m_guts->slice );
        break;
    }
}

} // namespace treeface
