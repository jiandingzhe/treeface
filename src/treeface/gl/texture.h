#ifndef TREEFACE_GL_TEXTURE_H
#define TREEFACE_GL_TEXTURE_H

#include "treeface/common.h"
#include "treeface/gl/imageref.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/MathsFunctions.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>

#define GLEW_STATIC
#include <GL/glew.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

extern const GLenum TEXTURE_UNITS[32];

/**
 * @brief wrapper for OpenGL 2D texture
 */
class Texture: public treejuce::Object
{
public:
    Texture();
    virtual ~Texture();

    // disable copy and move
    JUCE_DECLARE_NON_COPYABLE(Texture);
    JUCE_DECLARE_NON_MOVABLE(Texture);

    /**
     * @brief Set image data.
     *
     * @param image: the image to be set
     * @param internal_fmt: device-side image format
     * @param gen_mipmap: if set to true, a series of mipmaps will be generated
     *        using GLU; otherwise only use single level of texture, and set
     *        mipmap base and max levels to zero.
     *
     * @return ok if success, otherwise false.
     */
    treejuce::Result set_image_data(ImageRef image, GLint internal_fmt, bool gen_mipmap) NOEXCEPT;

    /**
     * @brief Specify image data for all mipmap levels explicitly.
     *
     * Mipmap base level and max level will be automatically adjusted. But you
     * still need to manually modify filter_min to those mipmap-enabled modes.
     *
     * @param images: images to be set for all mipmap levels
     * @param internal_fmt: device-side image format
     *
     * @return ok if success, otherwise false.
     */
    treejuce::Result set_image_data(treejuce::ArrayRef<ImageRef> images, GLint internal_fmt) NOEXCEPT;

    /**
     * @brief build texture from JSON data
     *
     * @param texture_root_node
     *
     * @return ok if success, otherwise false.
     */
    treejuce::Result build(const treejuce::var& texture_root_node);

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

    /**
     * @brief bind texture
     *
     * If any cached texture parameter is modified, they will be uploaded to the
     * device.
     */
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

    /**
     * @brief bind zero
     */
    static inline void unuse() NOEXCEPT
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
