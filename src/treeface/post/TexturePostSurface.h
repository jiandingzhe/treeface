#ifndef TREEFACE_TEXTURE_POST_SURFACE_H
#define TREEFACE_TEXTURE_POST_SURFACE_H

#include "treeface/post/PostSurface.h"

#include "treeface/gl/Texture.h"

#include <treecore/RefCountHolder.h>

namespace treeface
{

///
/// \brief post process rendering surface on a texture
///
/// It holds a 2D texture, or one side of cube map texture, or one slice of a 2D
/// texture array or 3D texture.
///
class TexturePostSurface: public PostSurface
{
public:
    ///
    /// \brief create surface and also create an empty 2D texture
    ///
    /// \param width         width in pixels
    /// \param height        height in pixels
    /// \param levels        number of mipmap levels
    /// \param internal_fmt  texture data storage format
    ///
    /// \see Texture(GLsizei width, GLsizei height, GLsizei levels, GLInternalImageFormat internal_fmt)
    ///
    TexturePostSurface(GLsizei width, GLsizei height, GLsizei levels, GLInternalImageFormat internal_fmt);

    ///
    /// \brief create surface and assign a 2D texture with it
    /// \param texture  texture to be assigned, must be a 2D texture.
    ///
    TexturePostSurface(Texture* texture);

    ///
    /// \brief create surface and assign one side of a cube map texture with it
    /// \param texture  texture to be assigned, must be a cube map texture.
    /// \param side     which side to be assigned
    ///
    TexturePostSurface(Texture* texture, GLTextureCubeSide side);

    ///
    /// \brief create surface and assign one slice of a 3D texture or 2D texture array
    /// \param texture  texture to be assigned, must be a 3D texture or 2D texture array
    /// \param slice    which slice to be assigned
    ///
    TexturePostSurface(Texture* texture, int32 slice);

    TREECORE_DECLARE_NON_COPYABLE(TexturePostSurface)
    TREECORE_DECLARE_NON_MOVABLE(TexturePostSurface)

    virtual ~TexturePostSurface();

    Texture* get_texture() const noexcept;

protected:
    struct Guts;

    void attach_to( Framebuffer* fbo, GLFramebufferAttachment attach ) override;

    Guts* m_guts;

};

} // namespace treeface

#endif // TREEFACE_TEXTURE_POST_SURFACE_H
