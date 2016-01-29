#include "treeface/graphics/Image.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/StringCast.h"

#include "treeface/gl/ImageRef.h"

#include <treecore/IntTypes.h>
#include <treecore/MemoryBlock.h>
#include <treecore/Result.h>
#include <treecore/StringRef.h>

using namespace treecore;

namespace treeface {

void intepret_fi_image_format( FIBITMAP* fi_img, uint8& result_num_channel, GLImageDataType& result_gl_type )
{
    FREE_IMAGE_TYPE fi_type = FreeImage_GetImageType( fi_img );
    uint32 bits_per_pixel   = FreeImage_GetBPP( fi_img );
    FREE_IMAGE_COLOR_TYPE fi_color_type = FreeImage_GetColorType( fi_img );

    uint32 fi_mask_r = FreeImage_GetRedMask( fi_img );
    uint32 fi_mask_g = FreeImage_GetGreenMask( fi_img );
    uint32 fi_mask_b = FreeImage_GetBlueMask( fi_img );

    switch (fi_type)
    {
    case FIT_BITMAP:
        switch (fi_color_type)
        {
        case FIC_RGB:
            result_num_channel = 3;

            if (bits_per_pixel == 16)
            {
                if (fi_mask_r == FI16_565_RED_MASK &&
                    fi_mask_g == FI16_565_GREEN_MASK &&
                    fi_mask_b == FI16_565_BLUE_MASK)
                {
                    result_gl_type = TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_6_5;
                }
                else
                {
                    throw ImageLoadError( "unsupported color encoding for 16 bit RGB: r " + String::toHexString( int(fi_mask_r) ) + ", g " + String::toHexString( int(fi_mask_g) ) + ", b " + String::toHexString( int(fi_mask_b) ) );
                }
            }
            else if (bits_per_pixel == 24)
            {
                result_gl_type = TFGL_IMAGE_DATA_UNSIGNED_BYTE;
            }
            else
            {
                throw ImageLoadError( "unsupported pixel size " + String( bits_per_pixel ) + " for RGB bitmap" );
            }
            break;

        case FIC_RGBALPHA:
            result_num_channel = 4;

            if (bits_per_pixel == 16)
            {
                if (fi_mask_r == FI16_555_RED_MASK &&
                    fi_mask_g == FI16_555_GREEN_MASK &&
                    fi_mask_b == FI16_555_BLUE_MASK)
                {
                    result_gl_type = TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_5_5_1;
                }
                else if (fi_mask_r == 0x0F00 &&
                         fi_mask_g == 0x00F0 &&
                         fi_mask_b == 0x000F)
                {
                    // TODO: this is probably incorrect for big-endian machine
                    result_gl_type = TFGL_IMAGE_DATA_UNSIGNED_SHORT_4_4_4_4;
                }
                else
                {
                    throw ImageLoadError( "unsupported color encoding for 16 bit RGBA: r " + String::toHexString( int(fi_mask_r) ) + ", g " + String::toHexString( int(fi_mask_g) ) + ", b " + String::toHexString( int(fi_mask_b) ) );
                }
            }
            else if (bits_per_pixel == 32)
            {
                result_gl_type = TFGL_IMAGE_DATA_UNSIGNED_BYTE;
            }
            else
            {
                throw ImageLoadError( "unsupported pixel size " + String( bits_per_pixel ) + " for RGBA bitmap" );
            }
            break;

        case FIC_MINISBLACK:
            if (bits_per_pixel == 8)
            {
                result_num_channel = 1;
                result_gl_type     = TFGL_IMAGE_DATA_UNSIGNED_BYTE;
            }
            else
            {
                throw ImageLoadError( "unsupported pixel size " + String() + " for greyscale bitmap" );
            }
            break;

        default:
            throw ImageLoadError( "unsupported FreeImage color mode " + toString( fi_color_type ) + " for bitmaps" );
        }
        break;

    case FIT_RGB16:
        result_num_channel = 3;
        result_gl_type     = TFGL_IMAGE_DATA_UNSIGNED_SHORT;
        break;

    case FIT_RGBA16:
        result_num_channel = 4;
        result_gl_type     = TFGL_IMAGE_DATA_UNSIGNED_SHORT;
        break;

    case FIT_RGBF:
        result_num_channel = 3;
        result_gl_type     = TFGL_IMAGE_DATA_FLOAT;
        break;

    case FIT_RGBAF:
        result_num_channel = 4;
        result_gl_type     = TFGL_IMAGE_DATA_FLOAT;
        break;

    case FIT_UINT16:
        if (fi_color_type == FIC_MINISBLACK)
        {
            result_num_channel = 1;
            result_gl_type     = TFGL_IMAGE_DATA_UNSIGNED_SHORT;
        }
        else
        {
            throw ImageLoadError( "unsupported color type: " + toString( fi_type ) + " for uint16 array image" );
        }
        break;
    case FIT_UINT32:
        if (fi_color_type == FIC_MINISBLACK)
        {
            result_num_channel = 1;
            result_gl_type     = TFGL_IMAGE_DATA_UNSIGNED_INT;
        }
        else
        {
            throw ImageLoadError( "unsupported color type: " + toString( fi_type ) + " for uint32 array image" );
        }
        break;

    case FIT_FLOAT:
        if (fi_color_type == FIC_MINISBLACK)
        {
            result_num_channel = 1;
            result_gl_type     = TFGL_IMAGE_DATA_FLOAT;
        }
        else
        {
            throw ImageLoadError( "unsupported color type: " + toString( fi_type ) + " for float array image" );
        }
        break;

    case FIT_DOUBLE:
        throw ImageLoadError( "image of FIT_DOUBLE is not supported" );
        break;

    default:
        throw ImageLoadError( "unsupported FreeImage type: " + toString( fi_type ) );
    }
}

Image::Image()
{}

Image::Image( MemoryBlock& data )
{
    FIMEMORY* mem_stream = FreeImage_OpenMemory( (BYTE*) data.getData(), (int32) data.getSize() );
    if (!mem_stream)
        throw ImageLoadError( "failed to create FreeImage memory handle" );

    FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory( mem_stream );
    m_fi_img = FreeImage_LoadFromMemory( format, mem_stream );

    FreeImage_CloseMemory( mem_stream );

    if (!m_fi_img)
        throw ImageLoadError( "failed to load FreeImage from memory (format is " + toString( format ) + ")" );

    intepret_fi_image_format( m_fi_img, m_num_channel, m_gl_type );
}

Image::Image( FIBITMAP* fi_img )
    : m_fi_img( fi_img )
{
    intepret_fi_image_format( m_fi_img, m_num_channel, m_gl_type );
}

Image::Image( const Image& other )
    : m_num_channel( other.m_num_channel )
    , m_gl_type( other.m_gl_type )
    , m_fi_img( FreeImage_Clone( other.m_fi_img ) )
{}

Image& Image::operator =( const Image& other )
{
    m_num_channel = other.m_num_channel;
    m_gl_type     = other.m_gl_type;
    m_fi_img      = FreeImage_Clone( other.m_fi_img );
    return *this;
}

Image::~Image()
{
    if (m_fi_img)
        FreeImage_Unload( m_fi_img );
}

void Image::set_image( FIBITMAP* new_img )
{
    if (m_fi_img)
        FreeImage_Unload( m_fi_img );

    m_fi_img = new_img;
    intepret_fi_image_format( m_fi_img, m_num_channel, m_gl_type );
}

FIBITMAP* Image::exchange_image( FIBITMAP* new_img )
{
    FIBITMAP* orig_img = m_fi_img;

    m_fi_img = new_img;
    intepret_fi_image_format( m_fi_img, m_num_channel, m_gl_type );

    return orig_img;
}

struct TextureFormatCast
{
    GLInternalImageFormat in_fmt;
    GLImageDataType       type;
    GLImageFormat         fmt;
};

TextureFormatCast _to_gl_format_solo_( TextureImageSoloChannelPolicy solo_pol, TextureImageIntDataPolicy int_pol, GLImageDataType type )
{
    switch (type)
    {
    case TFGL_IMAGE_DATA_BYTE:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_R8_SNORM, type,                          TFGL_IMAGE_FORMAT_RED };
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_R8I,      type,                          TFGL_IMAGE_FORMAT_RED_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_R8UI,     TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_RED_INT };
            }

        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE, TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_LUMINANCE };
            else throw TextureImageFormatError( "cannot use single-channel 8-bit image data as luminance texture without converting int to float" );

        case TEXTURE_IMAGE_SOLO_AS_ALPHA:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_ALPHA,     TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_ALPHA };
            else throw TextureImageFormatError( "cannot use single-channel 8-bit image data as alpha texture without converting int to float" );

        case TEXTURE_IMAGE_SOLO_AS_DEPTH:
            throw TextureImageFormatError( "cannot use single-channel 8-bit image data as depth texture" );
        }

    case TFGL_IMAGE_DATA_UNSIGNED_BYTE:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_R8,   type,                 TFGL_IMAGE_FORMAT_RED };
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_R8I,  TFGL_IMAGE_DATA_BYTE, TFGL_IMAGE_FORMAT_RED_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_R8UI, type,                 TFGL_IMAGE_FORMAT_RED_INT };
            }

        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE, type, TFGL_IMAGE_FORMAT_LUMINANCE };
            else throw TextureImageFormatError( "cannot use single-channel 8-bit image data as luminance texture without converting int to float" );

        case TEXTURE_IMAGE_SOLO_AS_ALPHA:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_ALPHA,     type, TFGL_IMAGE_FORMAT_ALPHA };
            else throw TextureImageFormatError( "cannot use single-channel 8-bit image data as alpha texture without converting int to float" );

        case TEXTURE_IMAGE_SOLO_AS_DEPTH:
            throw TextureImageFormatError( "cannot use single-channel 8-bit image data as depth texture" );
        }

    case TFGL_IMAGE_DATA_SHORT:
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT: throw TextureImageFormatError( "cannot use single-channel 16-bit integral image data as red float texture" );
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_R16I,  TFGL_IMAGE_DATA_SHORT,          TFGL_IMAGE_FORMAT_RED_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_R16UI, TFGL_IMAGE_DATA_UNSIGNED_SHORT, TFGL_IMAGE_FORMAT_RED_INT };
            }

        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE:
            throw TextureImageFormatError( "cannot use 16-bit integral image data as luminance texture" );

        case TEXTURE_IMAGE_SOLO_AS_ALPHA:
            throw TextureImageFormatError( "cannot use 16-bit integral image data as alpha texture" );

        case TEXTURE_IMAGE_SOLO_AS_DEPTH:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_DEPTH16, TFGL_IMAGE_DATA_UNSIGNED_SHORT, TFGL_IMAGE_FORMAT_DEPTH };
            else throw TextureImageFormatError( "cannot use single-channel 16-bit integra image data as depth texture without converting to float" );
        }

    case TFGL_IMAGE_DATA_INT:
    case TFGL_IMAGE_DATA_UNSIGNED_INT:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:    throw TextureImageFormatError( "cannot use single-channel 32-bit integral image data as red float texture" );
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_R32I,  TFGL_IMAGE_DATA_INT,          TFGL_IMAGE_FORMAT_RED_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_R32UI, TFGL_IMAGE_DATA_UNSIGNED_INT, TFGL_IMAGE_FORMAT_RED_INT };
            }

        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE:
            throw TextureImageFormatError( "cannot use single-channel 32-bit integral image data as luminance texture" );

        case TEXTURE_IMAGE_SOLO_AS_ALPHA:
            throw TextureImageFormatError( "cannot use single-channel 32-bit integral image data as alpha texture" );

        case TEXTURE_IMAGE_SOLO_AS_DEPTH:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24, TFGL_IMAGE_DATA_UNSIGNED_INT, TFGL_IMAGE_FORMAT_DEPTH };
            else throw TextureImageFormatError( "cannot use single-channel 32-bit integral image data as depth texture without converting to float" );
        }

    case TFGL_IMAGE_DATA_HALF_FLOAT:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED: return { TFGL_INTERNAL_IMAGE_FORMAT_R16F, type, TFGL_IMAGE_FORMAT_RED };
        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE: throw TextureImageFormatError( "cannot use single-channel half-float image data as luminance texture" );
        case TEXTURE_IMAGE_SOLO_AS_ALPHA:     throw TextureImageFormatError( "cannot use single-channel half-float image data as alpha texture" );
        case TEXTURE_IMAGE_SOLO_AS_DEPTH:     throw TextureImageFormatError( "cannot use single-channel half-float image data as depth texture" );
        }

    case TFGL_IMAGE_DATA_FLOAT:
        switch (solo_pol)
        {
        case TEXTURE_IMAGE_SOLO_AS_RED:   return { TFGL_INTERNAL_IMAGE_FORMAT_R32F,     type, TFGL_IMAGE_FORMAT_RED };
        case TEXTURE_IMAGE_SOLO_AS_DEPTH: return { TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F, type, TFGL_IMAGE_FORMAT_DEPTH };
        case TEXTURE_IMAGE_SOLO_AS_LUMINANCE: throw TextureImageFormatError( "cannot use single-channel float image data as luminance texture" );
        case TEXTURE_IMAGE_SOLO_AS_ALPHA:     throw TextureImageFormatError( "cannot use single-channel float image data as alpha texture" );
        }

    default:
        throw TextureImageFormatError( "unsupported data type of single-channel image for texturing: " + toString( type ) );
    }
}

TextureFormatCast _to_gl_format_dual_( TextureImageDualChannelPolicy dual_pol, TextureImageIntDataPolicy int_pol, GLImageDataType type )
{
    switch (type)
    {
    case TFGL_IMAGE_DATA_BYTE:
        switch (dual_pol)
        {
        case TEXTURE_IMAGE_DUAL_AS_RED_GREEN:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RG8_SNORM, type,                          TFGL_IMAGE_FORMAT_RG };
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RG8I,      type,                          TFGL_IMAGE_FORMAT_RG_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RG8UI,     TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_RG_INT };
            }

        case TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE_ALPHA, TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_LUMINANCE_ALPHA };
            else throw TextureImageFormatError( "cannot use two-channel 8-bit image as luminance-alpha texture without converting to float" );

        case TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL:
            throw TextureImageFormatError( "cannot use two-channel 8-bit image as depth-stencil texture" );
        }

    case TFGL_IMAGE_DATA_UNSIGNED_BYTE:
        switch (dual_pol)
        {
        case TEXTURE_IMAGE_DUAL_AS_RED_GREEN:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RG8,   type,                 TFGL_IMAGE_FORMAT_RG };
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RG8I,  TFGL_IMAGE_DATA_BYTE, TFGL_IMAGE_FORMAT_RG_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RG8UI, type,                 TFGL_IMAGE_FORMAT_RG_INT };
            }

        case TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA:
            if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE_ALPHA, TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_LUMINANCE_ALPHA };
            else throw TextureImageFormatError( "cannot use two-channel 8-bit image as luminance-alpha texture without converting to float" );

        case TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL:
            throw TextureImageFormatError( "cannot use two-channel 8-bit image as depth-stencil texture" );
        }

    case TFGL_IMAGE_DATA_SHORT:
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT:
        switch (dual_pol)
        {
        case TEXTURE_IMAGE_DUAL_AS_RED_GREEN:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:        throw TextureImageFormatError( "cannot use two-channel 16 bit integral image data as float red-green texture" );
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RG16I,  TFGL_IMAGE_DATA_SHORT,          TFGL_IMAGE_FORMAT_RG_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RG16UI, TFGL_IMAGE_DATA_UNSIGNED_SHORT, TFGL_IMAGE_FORMAT_RG_INT };
            }
        case TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA: throw TextureImageFormatError( "cannot use two-channel 16-bit integral image data as luminance-alpha texture" );
        case TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL:   throw TextureImageFormatError( "cannot use two-channel 16-bit integral image data as depth-stencil texture" );
        }

    case TFGL_IMAGE_DATA_INT:
    case TFGL_IMAGE_DATA_UNSIGNED_INT:
        switch (dual_pol)
        {
        case TEXTURE_IMAGE_DUAL_AS_RED_GREEN:
            switch (int_pol)
            {
            case  TEXTURE_IMAGE_INT_TO_FLOAT:        throw TextureImageFormatError( "cannot use two-channel 32 bit integral image data as float red-green texture" );
            case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RG32I,  TFGL_IMAGE_DATA_INT,          TFGL_IMAGE_FORMAT_RG_INT };
            case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RG32UI, TFGL_IMAGE_DATA_UNSIGNED_INT, TFGL_IMAGE_FORMAT_RG_INT };
            }
        case TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA: throw TextureImageFormatError( "cannot use two-channel 32-bit integral image data as luminance-alpha texture" );
        case TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL:   throw TextureImageFormatError( "cannot use two-channel 32-bit integral image data as depth-stencil texture" );
        }

    case TFGL_IMAGE_DATA_UNSIGNED_INT_24_8:
        if (dual_pol == TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL && int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT)
            return { TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24_STENCIL8, type, TFGL_IMAGE_FORMAT_DEPTH_STENCIL };
        else
            throw TextureImageFormatError( "cannot use 24-8 image data as texture types other than float depth-stencil texture" );

    case TFGL_IMAGE_DATA_HALF_FLOAT:
        if (dual_pol == TEXTURE_IMAGE_DUAL_AS_RED_GREEN)
            return { TFGL_INTERNAL_IMAGE_FORMAT_RG16F, type, TFGL_IMAGE_FORMAT_RG };
        else
            throw TextureImageFormatError( "cannot use two-channel half-float image data texture types other than red-green texture" );

    case TFGL_IMAGE_DATA_FLOAT:
        if (dual_pol == TEXTURE_IMAGE_DUAL_AS_RED_GREEN)
            return { TFGL_INTERNAL_IMAGE_FORMAT_RG32F, type, TFGL_IMAGE_FORMAT_RG };
        else
            throw TextureImageFormatError( "cannot use two-channel float image data as texture types other than red-green texture" );

    case TFGL_IMAGE_DATA_FLOAT_32_UNSIGNED_INT_24_8_REV:
        if (dual_pol == TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL && int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT)
            return { TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F_STENCIL8, type, TFGL_IMAGE_FORMAT_DEPTH_STENCIL };
        else
            throw TextureImageFormatError( "cannot use f32-uint24-8 image data as texture types other than float depth-stencil texture" );

    default:
        throw TextureImageFormatError( "cannot use data type for 2-channel image data as texture: " + toString( type ) );
    }
}

TextureFormatCast _to_gl_format_triple_( TextureImageIntDataPolicy int_pol, GLImageDataType type )
{
    switch (type)
    {
    case TFGL_IMAGE_DATA_BYTE:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8_SNORM, type,                          TFGL_IMAGE_FORMAT_RGB };
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8I,      type,                          TFGL_IMAGE_FORMAT_RGB_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI,     TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_RGB_INT };
        }

    case TFGL_IMAGE_DATA_UNSIGNED_BYTE:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8,   type,                 TFGL_IMAGE_FORMAT_RGB };
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8I,  TFGL_IMAGE_DATA_BYTE, TFGL_IMAGE_FORMAT_RGB_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI, type,                 TFGL_IMAGE_FORMAT_RGB_INT };
        }

    case TFGL_IMAGE_DATA_SHORT:
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    throw TextureImageFormatError( "cannot use 3-channel 16-bit integral image data as float texture" );
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGB16I,  TFGL_IMAGE_DATA_SHORT,          TFGL_IMAGE_FORMAT_RGB_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB16UI, TFGL_IMAGE_DATA_UNSIGNED_SHORT, TFGL_IMAGE_FORMAT_RGB_INT };
        }

    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_6_5:
        if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_RGB565, type, TFGL_IMAGE_FORMAT_RGB };
        else throw TextureImageFormatError( "cannot use 3-channel 5-6-5 image data without converting to float" );

    case TFGL_IMAGE_DATA_UNSIGNED_INT_10F_11F_11F_REV:
        if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_R11F_G11F_B10F, type, TFGL_IMAGE_FORMAT_RGB };
        else throw TextureImageFormatError( "cannot use 3-channel 10-11-11 image data without converting to float" );

    case TFGL_IMAGE_DATA_INT:
    case TFGL_IMAGE_DATA_UNSIGNED_INT:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    throw TextureImageFormatError( "cannot use 3-channel 32-bit integral image data as float texture" );
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGB32I,  TFGL_IMAGE_DATA_INT,          TFGL_IMAGE_FORMAT_RGB_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB32UI, TFGL_IMAGE_DATA_UNSIGNED_INT, TFGL_IMAGE_FORMAT_RGB_INT };
        }

    case TFGL_IMAGE_DATA_HALF_FLOAT: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB16F, type, TFGL_IMAGE_FORMAT_RGB };

    case TFGL_IMAGE_DATA_FLOAT:      return { TFGL_INTERNAL_IMAGE_FORMAT_RGB32F, type, TFGL_IMAGE_FORMAT_RGB };

    default: throw TextureImageFormatError( "cannot use 3-channel image as texture: data type is " + toString( type ) );
    }
}

TextureFormatCast _to_gl_format_quad_( TextureImageIntDataPolicy int_pol, GLImageDataType type )
{
    switch (type)
    {
    case TFGL_IMAGE_DATA_BYTE:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8_SNORM, type,                          TFGL_IMAGE_FORMAT_RGBA };
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I,      type,                          TFGL_IMAGE_FORMAT_RGBA_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI,     TFGL_IMAGE_DATA_UNSIGNED_BYTE, TFGL_IMAGE_FORMAT_RGBA_INT };
        }

    case TFGL_IMAGE_DATA_UNSIGNED_BYTE:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8,   type,                 TFGL_IMAGE_FORMAT_RGBA };
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I,  TFGL_IMAGE_DATA_BYTE, TFGL_IMAGE_FORMAT_RGBA_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI, type,                 TFGL_IMAGE_FORMAT_RGBA_INT };
        }

    case TFGL_IMAGE_DATA_SHORT:
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    throw TextureImageFormatError( "cannot use 4-channel 16-bit integral image data as float texture" );
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA16I,  TFGL_IMAGE_DATA_SHORT,          TFGL_IMAGE_FORMAT_RGBA_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA16UI, TFGL_IMAGE_DATA_UNSIGNED_SHORT, TFGL_IMAGE_FORMAT_RGBA_INT };
        }

    case TFGL_IMAGE_DATA_INT:
    case TFGL_IMAGE_DATA_UNSIGNED_INT:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    throw TextureImageFormatError( "cannot use 4-channel 32-bit integral image data as float texture" );
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA32I,  TFGL_IMAGE_DATA_INT,          TFGL_IMAGE_FORMAT_RGBA_INT };
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA32UI, TFGL_IMAGE_DATA_UNSIGNED_INT, TFGL_IMAGE_FORMAT_RGBA_INT };
        }

    case TFGL_IMAGE_DATA_HALF_FLOAT: return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA16F, type, TFGL_IMAGE_FORMAT_RGBA };

    case TFGL_IMAGE_DATA_FLOAT:      return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA32F, type, TFGL_IMAGE_FORMAT_RGBA };

    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_4_4_4_4:
        if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_RGBA4, type, TFGL_IMAGE_FORMAT_RGBA };
        else throw TextureImageFormatError( "cannot use 4-channel 4-4-4-4 image data as non-float texture" );

    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_5_5_1:
        if (int_pol ==  TEXTURE_IMAGE_INT_TO_FLOAT) return { TFGL_INTERNAL_IMAGE_FORMAT_RGB5_A1, type, TFGL_IMAGE_FORMAT_RGBA };
        else throw TextureImageFormatError( "cannot use 4-channel 5-5-5-1 image data as non-float texture" );

    case TFGL_IMAGE_DATA_UNSIGNED_INT_2_10_10_10_REV:
        switch (int_pol)
        {
        case  TEXTURE_IMAGE_INT_TO_FLOAT:    return { TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2,   type, TFGL_IMAGE_FORMAT_RGBA };
        case  TEXTURE_IMAGE_INT_AS_SIGNED:   throw TextureImageFormatError( "cannot use 4-channel 2-10-10-10 image data as signed integral texture" );
        case  TEXTURE_IMAGE_INT_AS_UNSIGNED: return { TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2UI, type, TFGL_IMAGE_FORMAT_RGBA_INT };
        }

    default:
        throw TextureImageFormatError( "unsupported data type for 4-channel image texture: " + toString( type ) );
    }
}

TextureFormatCast _to_gl_format_( TextureImageSoloChannelPolicy solo_pol, TextureImageDualChannelPolicy dual_pol, TextureImageIntDataPolicy int_pol,
                                  int num_channel, GLImageDataType type )
{
    switch (num_channel)
    {
    case 1: return _to_gl_format_solo_( solo_pol, int_pol, type );
    case 2: return _to_gl_format_dual_( dual_pol, int_pol, type );
    case 3: return _to_gl_format_triple_( int_pol, type );
    case 4: return _to_gl_format_quad_( int_pol, type );
    default:
        throw TextureImageFormatError( "invalid number of color channel: " + String( num_channel ) );
    }
}

TextureCompatibleImageRef Image::get_texture_compatible_2d( TextureImageSoloChannelPolicy solo_pol, TextureImageDualChannelPolicy dual_pol, TextureImageIntDataPolicy int_pol ) const
{
    TextureFormatCast cast = _to_gl_format_( solo_pol, dual_pol, int_pol, m_num_channel, m_gl_type );

    return TextureCompatibleImageRef{ cast.fmt, cast.in_fmt, cast.type,
                                      GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                      GLsizei( FreeImage_GetHeight( m_fi_img ) ),
                                      FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageRef Image::get_texture_compatible_2d_solo( TextureImageSoloChannelPolicy solo_pol,
                                                                 TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 1)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 1-channel" );

    TextureFormatCast cast = _to_gl_format_solo_( solo_pol, int_pol, m_gl_type );

    return TextureCompatibleImageRef{ cast.fmt, cast.in_fmt, cast.type,
                                      GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                      GLsizei( FreeImage_GetHeight( m_fi_img ) ),
                                      FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageRef Image::get_texture_compatible_2d_dual( TextureImageDualChannelPolicy dual_pol,
                                                                 TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 2)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 2-channel" );

    TextureFormatCast cast = _to_gl_format_dual_( dual_pol, int_pol, m_gl_type );

    return TextureCompatibleImageRef{ cast.fmt, cast.in_fmt, cast.type,
                                      GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                      GLsizei( FreeImage_GetHeight( m_fi_img ) ),
                                      FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageRef Image::get_texture_compatible_2d_triple( TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 3)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 3-channel" );

    TextureFormatCast cast = _to_gl_format_triple_( int_pol, m_gl_type );

    return TextureCompatibleImageRef{ cast.fmt, cast.in_fmt, cast.type,
                                      GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                      GLsizei( FreeImage_GetHeight( m_fi_img ) ),
                                      FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageRef Image::get_texture_compatible_2d_quad( TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 4)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 4-channel" );

    TextureFormatCast cast = _to_gl_format_quad_( int_pol, m_gl_type );

    return TextureCompatibleImageRef{ cast.fmt, cast.in_fmt, cast.type,
                                      GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                      GLsizei( FreeImage_GetHeight( m_fi_img ) ),
                                      FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageArrayRef Image::get_texture_compatible_2d_array( treecore::int32 num_frame, TextureImageSoloChannelPolicy solo_pol, TextureImageDualChannelPolicy dual_pol, TextureImageIntDataPolicy int_pol ) const
{
    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_frame = h_raw / num_frame;

    TextureFormatCast cast = _to_gl_format_( solo_pol, dual_pol, int_pol, m_num_channel, m_gl_type );

    return TextureCompatibleImageArrayRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_frame ),
                                           GLsizei( num_frame ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageArrayRef Image::get_texture_compatible_2d_array_solo( int32 num_frame,
                                                                            TextureImageSoloChannelPolicy solo_pol,
                                                                            TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 1)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 1-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_frame = h_raw / num_frame;

    TextureFormatCast cast = _to_gl_format_solo_( solo_pol, int_pol, m_gl_type );

    return TextureCompatibleImageArrayRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_frame ),
                                           GLsizei( num_frame ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageArrayRef Image::get_texture_compatible_2d_array_dual( int32 num_frame,
                                                                            TextureImageDualChannelPolicy dual_pol,
                                                                            TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 2)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 2-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_frame = h_raw / num_frame;

    TextureFormatCast cast = _to_gl_format_dual_( dual_pol, int_pol, m_gl_type );

    return TextureCompatibleImageArrayRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_frame ),
                                           GLsizei( num_frame ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageArrayRef Image::get_texture_compatible_2d_array_triple( int32 num_frame,
                                                                              TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 3)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 3-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_frame = h_raw / num_frame;

    TextureFormatCast cast = _to_gl_format_triple_( int_pol, m_gl_type );

    return TextureCompatibleImageArrayRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_frame ),
                                           GLsizei( num_frame ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleImageArrayRef Image::get_texture_compatible_2d_array_quad( int32 num_frame,
                                                                            TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 4)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 4-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_frame = h_raw / num_frame;

    TextureFormatCast cast = _to_gl_format_quad_( int_pol, m_gl_type );

    return TextureCompatibleImageArrayRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_frame ),
                                           GLsizei( num_frame ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleVoxelBlockRef Image::get_texture_compatible_3d( treecore::int32 num_slice, TextureImageSoloChannelPolicy solo_pol, TextureImageDualChannelPolicy dual_pol, TextureImageIntDataPolicy int_pol ) const
{
    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_slice = h_raw / num_slice;

    TextureFormatCast cast = _to_gl_format_( solo_pol, dual_pol, int_pol, m_num_channel, m_gl_type );

    return TextureCompatibleVoxelBlockRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_slice ),
                                           GLsizei( num_slice ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleVoxelBlockRef Image::get_texture_compatible_3d_solo( int32 num_slice,
                                                                      TextureImageSoloChannelPolicy solo_pol,
                                                                      TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 1)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 1-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_slice = h_raw / num_slice;

    TextureFormatCast cast = _to_gl_format_solo_( solo_pol, int_pol, m_gl_type );

    return TextureCompatibleVoxelBlockRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_slice ),
                                           GLsizei( num_slice ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleVoxelBlockRef Image::get_texture_compatible_3d_dual( int32 num_slice,
                                                                      TextureImageDualChannelPolicy dual_pol,
                                                                      TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 2)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 2-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_slice = h_raw / num_slice;

    TextureFormatCast cast = _to_gl_format_dual_( dual_pol, int_pol, m_gl_type );

    return TextureCompatibleVoxelBlockRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_slice ),
                                           GLsizei( num_slice ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleVoxelBlockRef Image::get_texture_compatible_3d_triple( int32 num_slice,
                                                                        TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 3)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 3-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_slice = h_raw / num_slice;

    TextureFormatCast cast = _to_gl_format_triple_( int_pol, m_gl_type );

    return TextureCompatibleVoxelBlockRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_slice ),
                                           GLsizei( num_slice ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

TextureCompatibleVoxelBlockRef Image::get_texture_compatible_3d_quad( int32 num_slice,
                                                                      TextureImageIntDataPolicy int_pol ) const
{
    if (m_num_channel != 4)
        throw TextureImageFormatError( "attempt to use " + String( m_num_channel ) + "-channel image data as 4-channel" );

    uint32 h_raw   = GLsizei( FreeImage_GetHeight( m_fi_img ) );
    uint32 h_slice = h_raw / num_slice;

    TextureFormatCast cast = _to_gl_format_quad_( int_pol, m_gl_type );

    return TextureCompatibleVoxelBlockRef{ cast.fmt, cast.in_fmt, cast.type,
                                           GLsizei( FreeImage_GetWidth( m_fi_img ) ),
                                           GLsizei( h_slice ),
                                           GLsizei( num_slice ),
                                           (void*) FreeImage_GetBits( m_fi_img ) };
}

} // namespace treeface
