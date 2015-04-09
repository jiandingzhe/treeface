#include "treeface/gl/imageref.h"
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

treejuce::Result Texture::set_image_data(ImageRef image, GLint internal_fmt) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 internal_fmt,
                 image.width,
                 image.height,
                 0,
                 image.format,
                 image.type,
                 image.data);

    GLenum err = glGetError();
    glBindTexture(GL_TEXTURE_2D, 0);

    if (err == GL_NO_ERROR)
    {
        return treejuce::Result::ok();
    }
    else
    {
        switch(err)
        {
        case GL_INVALID_ENUM:
            return treejuce::Result::fail("texture failed with \"invalid enum\"");
        case GL_INVALID_VALUE:
            return treejuce::Result::fail("texture failed with \"invalid value\"");
        case GL_INVALID_OPERATION:
            return treejuce::Result::fail("texture failed with \"invalid operation\"");
        default:
            return treejuce::Result::fail("texture failed with other error type: "+String(err));
        }
    }
}

treejuce::Result Texture::set_image_data(treejuce::ArrayRef<ImageRef> images, GLint internal_fmt) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, images.size() - 1);

    for (int level = 0; level < images.size(); level++)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     level,
                     internal_fmt,
                     images[level].width,
                     images[level].height,
                     0,
                     images[level].format,
                     images[level].type,
                     images[level].data);

        GLenum err = glGetError();

        if (err != GL_NO_ERROR)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            switch(err)
            {
            case GL_INVALID_ENUM:
                return treejuce::Result::fail("texture failed with \"invalid enum\" while setting mipmap level "+String(level));
            case GL_INVALID_VALUE:
                return treejuce::Result::fail("texture failed with \"invalid value\" while setting mipmap level "+String(level));
            case GL_INVALID_OPERATION:
                return treejuce::Result::fail("texture failed with \"invalid operation\" while setting mipmap level "+String(level));
            default:
                return treejuce::Result::fail("texture failed with other error type: "+String(err)+" while setting mipmap level "+String(level));
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return treejuce::Result::ok();
}

treejuce::Result Texture::set_mipmap_data(ImageRef image, GLint internal_fmt) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, internal_fmt, image.width, image.height, image.format, image.type, image.data);

    GLenum err = glGetError();
    glBindTexture(GL_TEXTURE_2D, 0);

    if (err == GL_NO_ERROR)
    {
        return treejuce::Result::ok();
    }
    else
    {
        switch(err)
        {
        case GL_INVALID_ENUM:
            return treejuce::Result::fail("texture failed with \"invalid enum\"");
        case GL_INVALID_VALUE:
            return treejuce::Result::fail("texture failed with \"invalid value\"");
        case GL_INVALID_OPERATION:
            return treejuce::Result::fail("texture failed with \"invalid operation\"");
        default:
            return treejuce::Result::fail("texture failed with other error type: "+String(err));
        }
    }
}

TREEFACE_NAMESPACE_END
