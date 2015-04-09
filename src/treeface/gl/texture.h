#ifndef TREEFACE_GL_TEXTURE_H
#define TREEFACE_GL_TEXTURE_H

#include "treeface/common.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/MathsFunctions.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class ImageRef;

extern const GLenum TEXTURE_UNITS[32];

/**
 * @brief wrapper for OpenGL 2D texture
 */
class Texture: public treejuce::Object
{
public:
    Texture();
    ~Texture();

    // disable copy and move
    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator = (const Texture&) = delete;
    Texture& operator = (Texture&&) = delete;

    void init() NOEXCEPT;

    /**
     * Set image data and set to only using single mipmap level.
     *
     * Mipmap base level and max level will be automatically set to zero.
     *
     * @param image: the image to be set
     * @param internal_fmt: device-side image format
     */
    treejuce::Result set_image_data(ImageRef image, GLint internal_fmt) NOEXCEPT;

    /**
     * Specify image data for all mipmap levels.
     *
     * Mipmap base level and max level will be automatically adjusted. But you
     * still need to manually modify filter_min to those mipmap-enabled modes.
     *
     * @param images: images to be set for all mipmap levels
     * @param internal_fmt: device-side image format
     */
    treejuce::Result set_image_data(treejuce::ArrayRef<ImageRef> images, GLint internal_fmt) NOEXCEPT;

    /**
     * automatically generate all mipmap levels using one image data.
     *
     * Mipmap base level and max level will be automatically adjusted. But you
     * still need to manually modify filter_min to those mipmap-enabled modes.
     *
     * @param image: the image to be set
     * @param internal_fmt: device-side image format
     */
    treejuce::Result set_mipmap_data(ImageRef image, GLint internal_fmt) NOEXCEPT;

    inline float get_min_lod() const NOEXCEPT
    { return m_min_lod; }

    inline void set_min_lod(float value) NOEXCEPT
    {
        if (m_min_lod != value)
        {
            m_param_changed = true;
            m_min_lod = value;
        }
    }

    inline float get_max_lod() const NOEXCEPT
    { return m_max_lod; }

    inline void set_max_lod(float value) NOEXCEPT
    {
        if (m_max_lod != value)
        {
            m_param_changed = true;
            m_max_lod = value;
        }
    }

    inline GLenum get_wrap_s() const NOEXCEPT
    { return m_wrap_s; }

    inline void set_wrap_s(GLenum value) NOEXCEPT
    {
        if (m_wrap_s != value)
        {
            m_param_changed = true;
            m_wrap_s = value;
        }
    }

    inline GLenum get_wrap_t() const NOEXCEPT
    { return m_wrap_t; }

    inline void set_wrap_t(GLenum value) NOEXCEPT
    {
        if (m_wrap_t != value)
        {
            m_param_changed = true;
            m_wrap_t = value;
        }
    }

    inline GLenum get_min_filter() const NOEXCEPT
    { return m_min_filter; }

    inline void set_min_filter(GLenum value) NOEXCEPT
    {
        if (m_min_filter != value)
        {
            m_param_changed = true;
            m_min_filter = value;
        }
    }

    inline GLenum get_mag_filter() const NOEXCEPT
    { return m_mag_filter; }

    inline void set_mag_filter(GLenum value) NOEXCEPT
    {
        if (m_mag_filter != value)
        {
            m_param_changed = true;
            m_mag_filter = value;
        }
    }

    inline void use() NOEXCEPT
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);

        if (m_param_changed)
        {
            m_param_changed = false;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, m_min_lod);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, m_max_lod);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
        }
    }

    inline void unuse() const NOEXCEPT
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

protected:
    GLuint m_texture;

    // texture parameters
    // If modified, will be uploaded to device on call to use().

    bool m_param_changed = true;
    float m_min_lod = -1000;
    float m_max_lod = 1000;
    GLenum m_wrap_s = GL_REPEAT;
    GLenum m_wrap_t = GL_REPEAT;
    GLenum m_min_filter = GL_NEAREST;
    GLenum m_mag_filter = GL_NEAREST;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_TEXTURE_H
