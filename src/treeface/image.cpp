#include "treeface/image.h"
#include "treeface/misc/stringcast.h"

#include "treeface/gl/imageref.h"

#include <treejuce/IntTypes.h>
#include <treejuce/Result.h>
#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Result convert_fi_image_info_to_gl(FIBITMAP* fi_img, GLenum& gl_format, GLenum& gl_type)
{
    FREE_IMAGE_TYPE fi_type = FreeImage_GetImageType(fi_img);
    uint32 bits_per_pixel = FreeImage_GetBPP(fi_img);
    FREE_IMAGE_COLOR_TYPE fi_color_type = FreeImage_GetColorType(fi_img);

    uint32 fi_mask_r = FreeImage_GetRedMask(fi_img);
    uint32 fi_mask_g = FreeImage_GetGreenMask(fi_img);
    uint32 fi_mask_b = FreeImage_GetBlueMask(fi_img);

    switch (fi_type)
    {
    case FIT_BITMAP:
        switch (fi_color_type)
        {
        case FIC_RGB:
            gl_format = GL_RGB;
            if (bits_per_pixel == 16)
            {
                if (fi_mask_r == FI16_565_RED_MASK &&
                    fi_mask_g == FI16_565_GREEN_MASK &&
                    fi_mask_b == FI16_565_BLUE_MASK)
                {
                    gl_type = GL_UNSIGNED_SHORT_5_6_5;
                }
                else
                {
                    return Result::fail("unsupported color encoding for 16 bit RGB: r "+String::toHexString(int(fi_mask_r))+", g "+String::toHexString(int(fi_mask_g))+", b "+String::toHexString(int(fi_mask_b)));
                }
            }
            else if (bits_per_pixel == 24)
            {
                gl_type = GL_UNSIGNED_BYTE;
            }
            else
            {
                return Result::fail("unsupported pixel size "+String(bits_per_pixel)+" for RGB bitmap");
            }
            break;

        case FIC_RGBALPHA:
            gl_format = GL_RGBA;
            if (bits_per_pixel == 16)
            {
                if (fi_mask_r == FI16_555_RED_MASK &&
                    fi_mask_g == FI16_555_GREEN_MASK &&
                    fi_mask_b == FI16_555_BLUE_MASK)
                {
                    gl_type = GL_UNSIGNED_SHORT_5_5_5_1;
                }
                else if (fi_mask_r == 0x0F00 &&
                         fi_mask_g == 0x00F0 &&
                         fi_mask_b == 0x000F)
                {
                    // TODO: this is probably incorrect for big-endian machine
                    gl_type = GL_UNSIGNED_SHORT_4_4_4_4;
                }
                else
                {
                    return Result::fail("unsupported color encoding for 16 bit RGBA: r "+String::toHexString(int(fi_mask_r))+", g "+String::toHexString(int(fi_mask_g))+", b "+String::toHexString(int(fi_mask_b)));
                }
            }
            else if (bits_per_pixel == 32)
            {
                gl_type = GL_UNSIGNED_BYTE;
            }
            else
            {
                return Result::fail("unsupported pixel size "+String(bits_per_pixel)+" for RGBA bitmap");
            }
            break;

        case FIC_MINISBLACK:
            if (bits_per_pixel == 8)
            {
                gl_format = GL_ALPHA;
                gl_type = GL_UNSIGNED_BYTE;
            }
            else
            {
                return Result::fail("unsupported pixel size "+String()+" for greyscale bitmap");
            }
            break;

        default:
            return Result::fail("unsupported FreeImage color mode "+to_string(fi_color_type)+" for bitmaps");
        }
        break;

    case FIT_RGB16:
        gl_format = GL_RGB;
        gl_type = GL_UNSIGNED_SHORT;
        break;

    case FIT_RGBA16:
        gl_format = GL_RGBA;
        gl_type = GL_UNSIGNED_SHORT;
        break;

    case FIT_RGBF:
        gl_format = GL_RGB;
        gl_type = GL_FLOAT;
        break;

    case FIT_RGBAF:
        gl_format = GL_RGBA;
        gl_type = GL_FLOAT;
        break;

    case FIT_UINT16:
        if (fi_color_type == FIC_MINISBLACK)
        {
            gl_format = GL_ALPHA;
            gl_type = GL_UNSIGNED_SHORT;
        }
        else
        {
            return Result::fail("unsupported color type: "+to_string(fi_type)+" for uint16 array image");
        }
        break;
    case FIT_UINT32:
        if (fi_color_type == FIC_MINISBLACK)
        {
            gl_format = GL_ALPHA;
            gl_type = GL_UNSIGNED_INT;
        }
        else
        {
            return Result::fail("unsupported color type: "+to_string(fi_type)+" for uint32 array image");
        }
        break;

    case FIT_FLOAT:
        if (fi_color_type == FIC_MINISBLACK)
        {
            gl_format = GL_ALPHA;
            gl_type = GL_FLOAT;
        }
        else
        {
            return Result::fail("unsupported color type: "+to_string(fi_type)+" for float array image");
        }
        break;

    case FIT_DOUBLE:
        if (fi_color_type == FIC_MINISBLACK)
        {
            gl_format = GL_ALPHA;
            gl_type = GL_DOUBLE;
        }
        else
        {
            return Result::fail("unsupported color type: "+to_string(fi_type)+" for double array image");
        }
        break;

    default:
        return Result::fail("unsupported FreeImage type: "+to_string(fi_type));
    }

    return Result::ok();
}

Image::Image()
{}

Image::Image(FIBITMAP* fi_img)
    : m_fi_img(fi_img)
{
    Result re = convert_fi_image_info_to_gl(fi_img, m_gl_format, m_gl_type);
    if (!re)
        die("%s", re.getErrorMessage().toRawUTF8());
}

Image::Image(const Image& other)
    : m_gl_format(other.m_gl_format)
    , m_gl_type(other.m_gl_type)
    , m_fi_img(FreeImage_Clone(other.m_fi_img))
{
}

Image& Image::operator = (const Image& other)
{
    m_gl_format = other.m_gl_format;
    m_gl_type   = other .m_gl_type;
    m_fi_img    = FreeImage_Clone(other.m_fi_img);
    return *this;
}

Image::Image(Image&& other)
    : m_gl_format(other.m_gl_format)
    , m_gl_type(other.m_gl_type)
    , m_fi_img(other.m_fi_img)
{
    other.m_fi_img = nullptr;
}

Image& Image::operator = (Image&& other)
{
    m_gl_format = other.m_gl_format;
    m_gl_type   = other .m_gl_type;
    m_fi_img    = other.m_fi_img;
    other.m_fi_img = nullptr;
    return *this;
}

Image::~Image()
{
    if (m_fi_img)
        FreeImage_Unload(m_fi_img);
}

void Image::set_image(FIBITMAP* new_img)
{
    if (m_fi_img)
        FreeImage_Unload(m_fi_img);

    m_fi_img = new_img;

    Result re = convert_fi_image_info_to_gl(m_fi_img, m_gl_format, m_gl_type);
    if (!re)
        die("%s", re.getErrorMessage().toRawUTF8());
}

FIBITMAP* Image::exchange_image(FIBITMAP* new_img)
{
    FIBITMAP* orig = m_fi_img;
    m_fi_img = new_img;

    Result re = convert_fi_image_info_to_gl(m_fi_img, m_gl_format, m_gl_type);
    if (!re)
        die("%s", re.getErrorMessage().toRawUTF8());

    return orig;
}


ImageRef Image::get_image_ref() const NOEXCEPT
{
    if (m_fi_img)
        return {m_gl_format, m_gl_type, int(FreeImage_GetWidth(m_fi_img)), int(FreeImage_GetHeight(m_fi_img)), FreeImage_GetBits(m_fi_img)};
    else
        return {0, 0, 0, 0, nullptr};
}

TREEFACE_NAMESPACE_END
