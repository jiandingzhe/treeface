#include "treeface/gl/texture.h"

#include <treejuce/String.h>
#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

const GLenum TEXTURE_UNITS[32] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
    GL_TEXTURE10,
    GL_TEXTURE11,
    GL_TEXTURE12,
    GL_TEXTURE13,
    GL_TEXTURE14,
    GL_TEXTURE15,
    GL_TEXTURE16,
    GL_TEXTURE17,
    GL_TEXTURE18,
    GL_TEXTURE19,
    GL_TEXTURE20,
    GL_TEXTURE21,
    GL_TEXTURE22,
    GL_TEXTURE23,
    GL_TEXTURE24,
    GL_TEXTURE25,
    GL_TEXTURE26,
    GL_TEXTURE27,
    GL_TEXTURE28,
    GL_TEXTURE29,
    GL_TEXTURE30,
    GL_TEXTURE31,
};

Texture::Texture()
{
}

Texture::~Texture()
{
    if (m_texture)
        glDeleteTextures(1, &m_texture);
}

void Texture::init() NOEXCEPT
{
    glGenTextures(1, &m_texture);
    if (!m_texture)
        die("failed to generate texture");
}

void Texture::set_image_data(GLint internal_fmt,
                             GLenum format, GLenum type,
                             GLsizei w, GLsizei h, treejuce::uint8* data) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_fmt, w, h, 0, format, type, data);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        die("set_image_data failed with %d\n", err);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_mipmap_data(GLint internal_fmt,
                              GLenum format, GLenum type,
                              GLsizei w, GLsizei h, treejuce::uint8* data) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, internal_fmt, w, h, format, type, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TREEFACE_NAMESPACE_END
