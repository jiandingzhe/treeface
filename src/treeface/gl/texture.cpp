#include "treeface/gl/imageref.h"
#include "treeface/gl/texture.h"
#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/stringcast.h"

#include <treejuce/DynamicObject.h>
#include <treejuce/Logger.h>
#include <treejuce/HashSet.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/String.h>
#include <treejuce/StringRef.h>
#include <treejuce/Variant.h>

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
    glGenTextures(1, &m_texture);
    if (!m_texture)
        die("failed to generate texture");
}

Texture::~Texture()
{
    if (m_texture)
        glDeleteTextures(1, &m_texture);
}

treejuce::Result Texture::set_image_data(ImageRef image, GLint internal_fmt, bool gen_mipmap) NOEXCEPT
{
    glBindTexture(GL_TEXTURE_2D, m_texture);

    if (gen_mipmap)
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D,
                          internal_fmt,
                          image.width,
                          image.height,
                          image.format,
                          image.type,
                          image.data);
    }
    else
    {
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
    }

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

Result _validate_keys_(NamedValueSet& kv)
{
    HashSet<String> expected_keys;
    expected_keys.insert("image");
    expected_keys.insert("internal_format");
    expected_keys.insert("mag_filter_linear");
    expected_keys.insert("min_filter_linear");
    expected_keys.insert("mipmap");
    expected_keys.insert("mipmap_filter_linear");
    expected_keys.insert("wrap_s");
    expected_keys.insert("wrap_t");

    for (int i = 0; i < kv.size(); i++)
    {
        Identifier key = kv.getName(i);
        if (!expected_keys.contains(key.toString()))
        {
            return Result::fail("texture specification contain unidentified key: "+key.toString());
        }
    }
    return Result::ok();
}

Result Texture::build(const treejuce::var& tex_node)
{
    if (!tex_node.isObject())
        return Result::fail("texture node is not KV");

    NamedValueSet& tex_kv = tex_node.getDynamicObject()->getProperties();
    {
        Result re = _validate_keys_(tex_kv);
        if (!re)
            return re;
    }

    //
    // load properties
    //
    m_param_changed = true;

    bool mag_linear = false;
    if (tex_kv.contains("mag_filter_linear"))
        mag_linear = bool(tex_kv["mag_filter_linear"]);

    if (mag_linear)
        m_mag_filter = GL_LINEAR;
    else
        m_mag_filter = GL_NEAREST;

    if (tex_kv.contains("wrap_s"))
    {
        String wrap_s_str = tex_kv["wrap_s"].toString();
        if (wrap_s_str.compareIgnoreCase("repeat") == 0)
            m_wrap_s = GL_REPEAT;
        else if (wrap_s_str.compareIgnoreCase("mirrored_repeat") == 0)
            m_wrap_s = GL_MIRRORED_REPEAT;
        else if (wrap_s_str.compareIgnoreCase("clamp_to_edge") == 0)
            m_wrap_s = GL_CLAMP_TO_EDGE;
    }

    if (tex_kv.contains("wrap_t"))
    {
        String wrap_t_str = tex_kv["wrap_t"].toString();
        if (wrap_t_str.compareIgnoreCase("repeat") == 0)
            m_wrap_t = GL_REPEAT;
        else if (wrap_t_str.compareIgnoreCase("mirrored_repeat") == 0)
            m_wrap_t = GL_MIRRORED_REPEAT;
        else if (wrap_t_str.compareIgnoreCase("clamp_to_edge") == 0)
            m_wrap_t = GL_CLAMP_TO_EDGE;
    }

    //
    // load image, create texture object
    //
    if (!tex_kv.contains("internal_format"))
        return Result::fail("texture don't have internal_format specified");
    GLenum internal_format = gl_internal_format_from_string(tex_kv["internal_format"].toString());

    bool use_mipmap = true;
    if (tex_kv.contains("mipmap"))
        use_mipmap = bool(tex_kv["mipmap"]);

    bool min_linear = true;
    if (tex_kv.contains("min_filter_linear"))
        min_linear = bool(tex_kv["min_filter_linear"]);

    if (!tex_kv.contains("image"))
        return Result::fail("texture don't have image");

    if (tex_kv["image"].isArray())
    {
        if (!use_mipmap)
        {
            use_mipmap = true;
            Logger::writeToLog("Texture has multiple images specified, but mipmap set to false, implicitly change it to true.");
        }

        // load images specified for each mipmap
        Array<var>* image_name_nodes = tex_kv["image"].getArray();

        Array<ImageRef> img_refs;
        for (int i = 0; i < image_name_nodes->size(); i++)
        {
            String img_name = image_name_nodes->getReference(i).toString();
            Image* img = nullptr;
            Result img_re = ImageManager::getInstance()->get_image(img_name, &img);
            if (!img_re)
                return img_re;
            img_refs.add(img->get_image_ref());
        }

        set_image_data(img_refs, internal_format);
    }
    else
    {
        String img_name = tex_kv["image"].toString();
        Image* img = nullptr;
        Result img_re = ImageManager::getInstance()->get_image(img_name, &img);
        if (!img_re)
            return img_re;

        set_image_data(img->get_image_ref(), internal_format, use_mipmap);
    }

    if (use_mipmap)
    {
        bool mipmap_linear = true;
        if (tex_kv.contains("mipmap_filter_linear"))
            mipmap_linear = bool(tex_kv["mipmap_filter_linear"]);

        if (min_linear)
        {
            if (mipmap_linear)
                m_min_filter = GL_LINEAR_MIPMAP_LINEAR;
            else
                m_min_filter = GL_LINEAR_MIPMAP_NEAREST;
        }
        else
        {
            if (mipmap_linear)
                m_min_filter = GL_NEAREST_MIPMAP_LINEAR;
            else
                m_min_filter = GL_NEAREST_MIPMAP_NEAREST;
        }
    }
    else
    {
        if (tex_kv.contains("mipmap_filter_linear"))
            Logger::writeToLog("");

        if (min_linear)
            m_min_filter = GL_LINEAR;
        else
            m_min_filter = GL_NEAREST;
    }

    return Result::ok();
}

TREEFACE_NAMESPACE_END
