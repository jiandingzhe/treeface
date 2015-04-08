#ifndef TREEFACE_GL_TEXTURE_H
#define TREEFACE_GL_TEXTURE_H

#include "treeface/common.h"

#include <treejuce/Array.h>
#include <treejuce/MathsFunctions.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

extern const GLenum TEXTURE_UNITS[32];

/**
 * @brief wrapper for OpenGL 2D texture
 */
struct Texture
{
    Texture();

    ~Texture();

    void init() NOEXCEPT;

    void set_image_data(GLint internal_fmt,
                        GLenum format, GLenum type,
                        GLsizei w, GLsizei h, treejuce::uint8* data) NOEXCEPT;

    void set_mipmap_data(GLint internal_fmt,
                         GLenum format, GLenum type,
                         GLsizei w, GLsizei h, treejuce::uint8* data) NOEXCEPT;

    inline void use() const NOEXCEPT
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    inline void unuse() const NOEXCEPT
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint m_texture;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_TEXTURE_H
