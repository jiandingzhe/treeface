#ifndef TREEFACE_GL_TEXTURE_H
#define TREEFACE_GL_TEXTURE_H

#include "treeface/base/Common.h"
#include "treeface/gl/Enums.h"
#include "treeface/gl/ImageRef.h"

#include <treecore/ArrayRef.h>
#include <treecore/MathsFunctions.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treecore {
class var;
} // namespace treecore

namespace treeface {

class Framebuffer;

extern const GLenum TEXTURE_UNITS[32];

/**
 * @brief wrapper for OpenGL 2D texture
 */
class Texture: public treecore::RefCountObject
{
    friend class Framebuffer;

public:
    class BindScope
    {
    public:
        BindScope( Texture& texture ): m_texture( texture ) { m_texture.bind();   }
        ~BindScope()                                        { m_texture.unbind(); }
    private:
        Texture& m_texture;
    };

    ///
    /// \brief Create 2D texture using one image.
    ///
    /// \param image          the image to be set
    /// \param internal_fmt   device-side image format
    /// \param num_gen_mipmap number of extra mipmap levels to generate. Set to
    ///                       N means N+1 levels of mipmaps in total.
    ///
    Texture( TextureCompatibleImageRef image, treecore::uint32 num_gen_mipmap );

    ///
    /// \brief Create 2D texture with all mipmap levels explicitly specified
    ///
    /// \param images        images for all mipmap levels
    /// \param internal_fmt  device-side image format
    ///
    Texture( treecore::ArrayRef<TextureCompatibleImageRef> images );

    ///
    /// \brief Create empty immutable 2D texture object
    ///
    /// \param width         texture width
    /// \param height        texture width
    /// \param levels        number of mipmap levels
    /// \param internal_fmt  device-side image format
    ///
    Texture( GLsizei width, GLsizei height, GLsizei levels, GLInternalImageFormat internal_fmt );

    ///
    /// \brief Create 2D texture array
    ///
    /// \param images         images to be set for each frame
    /// \param internal_fmt   device-side image format
    /// \param num_gen_mipmap number of extra mipmap levels to generate. Set to
    ///                       N means N+1 levels of mipmaps in total.
    ///
    Texture( TextureCompatibleImageArrayRef images, treecore::uint32 num_gen_mipmap );

    ///
    /// \brief Create cube map texture
    ///
    /// \param img_x_plus     image at x+ direction
    /// \param img_x_minus    image at x- direction
    /// \param img_y_plus     image at y+ direction
    /// \param img_y_minus    image at y- direction
    /// \param img_z_plus     image at z+ direction
    /// \param img_z_minus    image at z- direction
    /// \param internal_fmt   device-side image format
    /// \param num_gen_mipmap number of extra mipmap levels to generate. Set to
    ///                       N means N+1 levels of mipmaps in total.
    ///
    Texture( TextureCompatibleImageRef img_x_plus, TextureCompatibleImageRef img_x_minus,
             TextureCompatibleImageRef img_y_plus, TextureCompatibleImageRef img_y_minus,
             TextureCompatibleImageRef img_z_plus, TextureCompatibleImageRef img_z_minus,
             treecore::uint32 num_gen_mipmap );

    ///
    /// \brief Create 3D texture
    ///
    /// \param voxel          data to be set
    /// \param internal_fmt   device-side image format
    /// \param num_gen_mipmap number of extra mipmap levels to generate. Set to
    ///                       N means N+1 levels of mipmaps in total.
    ///
    Texture( TextureCompatibleVoxelBlockRef voxel, treecore::uint32 num_gen_mipmap );

    ///
    /// \brief create Texture object using JSON nodes
    ///
    /// \param texture_root_node
    ///
    Texture( const treecore::var& texture_root_node );

    // disable copy and move
    TREECORE_DECLARE_NON_COPYABLE( Texture );
    TREECORE_DECLARE_NON_MOVABLE( Texture );

    virtual ~Texture();

    float get_min_lod() const noexcept
    {
        jassert( is_bound() );
        float re = std::numeric_limits<float>::signaling_NaN();
        glGetTexParameterfv( m_type, GL_TEXTURE_MIN_LOD, &re );
        return re;
    }

    void set_min_lod( float value ) noexcept
    {
        jassert( is_bound() );
        glTexParameterf( m_type, GL_TEXTURE_MIN_LOD, value );
    }

    float get_max_lod() const noexcept
    {
        jassert( is_bound() );
        float re = std::numeric_limits<float>::signaling_NaN();
        glGetTexParameterfv( m_type, GL_TEXTURE_MAX_LOD, &re );
        return re;
    }

    void set_max_lod( float value ) noexcept
    {
        jassert( is_bound() );
        glTexParameterf( m_type, GL_TEXTURE_MAX_LOD, value );
    }

    GLTextureWrap get_wrap_s() const noexcept
    {
        jassert( is_bound() );
        GLint re = -1;
        glGetTexParameteriv( m_type, GL_TEXTURE_WRAP_S, &re );
        return GLTextureWrap( re );
    }

    void set_wrap_s( GLTextureWrap value ) noexcept
    {
        jassert( is_bound() );
        glTexParameteri( m_type, GL_TEXTURE_WRAP_S, value );
    }

    GLTextureWrap get_wrap_t() const noexcept
    {
        jassert( is_bound() );
        GLint re = -1;
        glGetTexParameteriv( m_type, GL_TEXTURE_WRAP_T, &re );
        return GLTextureWrap( re );
    }

    void set_wrap_t( GLTextureWrap value ) noexcept
    {
        jassert( is_bound() );
        glTexParameteri( m_type, GL_TEXTURE_WRAP_T, value );
    }

    GLTextureFilter get_min_filter() const noexcept
    {
        jassert( is_bound() );
        GLint re = -1;
        glGetTexParameteriv( m_type, GL_TEXTURE_MIN_FILTER, &re );
        return GLTextureFilter( re );
    }

    void set_min_filter( GLTextureFilter value ) noexcept
    {
        jassert( is_bound() );
        glTexParameteri( m_type, GL_TEXTURE_MIN_FILTER, value );
    }

    GLTextureFilter get_mag_filter() const noexcept
    {
        jassert( is_bound() );
        GLint re = -1;
        glGetTexParameteriv( m_type, GL_TEXTURE_MAG_FILTER, &re );
        return GLTextureFilter( re );
    }

    void set_mag_filter( GLTextureFilter value ) noexcept
    {
        jassert( is_bound() );
        glTexParameteri( m_type, GL_TEXTURE_MAG_FILTER, value );
    }

    void bind() noexcept { glBindTexture( m_type, m_texture ); }

    /**
     * @brief bind zero
     */
    void unbind() noexcept { glBindTexture( m_type, 0 ); }

    bool is_bound() const noexcept { return get_current_bound_texture( m_type ) == m_texture; }

    GLuint get_gl_handle() const noexcept { return m_texture; }

    static GLuint get_current_bound_texture( GLTextureType type ) noexcept;

protected:
    GLuint m_texture = 0;
    GLTextureType m_type;
    bool m_immutable = false;
};

} // namespace treeface

#endif // TREEFACE_GL_TEXTURE_H
