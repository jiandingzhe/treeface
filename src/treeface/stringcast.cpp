#include "treeface/stringcast.h"

#include <treejuce/StringRef.h>

TREEFACE_NAMESPACE_BEGIN

template<>
FREE_IMAGE_FORMAT from_string<FREE_IMAGE_FORMAT>(const treejuce::String& string)
{
    if (string.compareIgnoreCase("unknown") == 0)     return FIF_UNKNOWN;
    else if (string.compareIgnoreCase("bmp") == 0)    return FIF_BMP;
    else if (string.compareIgnoreCase("ico") == 0)    return FIF_ICO;
    else if (string.compareIgnoreCase("jpg") == 0)    return FIF_JPEG;
    else if (string.compareIgnoreCase("jpe") == 0)    return FIF_JPEG;
    else if (string.compareIgnoreCase("jif") == 0)    return FIF_JPEG;
    else if (string.compareIgnoreCase("jpeg") == 0)   return FIF_JPEG;
    else if (string.compareIgnoreCase("jng") == 0)    return FIF_JNG;
    else if (string.compareIgnoreCase("koa") == 0)    return FIF_KOALA;
    else if (string.compareIgnoreCase("koala") == 0)  return FIF_KOALA;
    else if (string.compareIgnoreCase("iff") == 0)    return FIF_IFF;
    else if (string.compareIgnoreCase("lbm") == 0)    return FIF_LBM;
    else if (string.compareIgnoreCase("mng") == 0)    return FIF_MNG;
    else if (string.compareIgnoreCase("pbm") == 0)    return FIF_PBM;
    else if (string.compareIgnoreCase("pbmraw") == 0) return FIF_PBMRAW;
    else if (string.compareIgnoreCase("pcd") == 0)    return FIF_PCD;
    else if (string.compareIgnoreCase("pcx") == 0)    return FIF_PCX;
    else if (string.compareIgnoreCase("pgm") == 0)    return FIF_PGM;
    else if (string.compareIgnoreCase("pgmraw") == 0) return FIF_PGMRAW;
    else if (string.compareIgnoreCase("png") == 0)    return FIF_PNG;
    else if (string.compareIgnoreCase("ppm") == 0)    return FIF_PPM;
    else if (string.compareIgnoreCase("ppmraw") == 0) return FIF_PPMRAW;
    else if (string.compareIgnoreCase("ras") == 0)    return FIF_RAS;
    else if (string.compareIgnoreCase("tga") == 0)    return FIF_TARGA;
    else if (string.compareIgnoreCase("targa") == 0)  return FIF_TARGA;
    else if (string.compareIgnoreCase("tif") == 0)    return FIF_TIFF;
    else if (string.compareIgnoreCase("tiff") == 0)   return FIF_TIFF;
    else if (string.compareIgnoreCase("wbmp") == 0)   return FIF_WBMP;
    else if (string.compareIgnoreCase("psd") == 0)    return FIF_PSD;
    else if (string.compareIgnoreCase("cut") == 0)    return FIF_CUT;
    else if (string.compareIgnoreCase("xbm") == 0)    return FIF_XBM;
    else if (string.compareIgnoreCase("xpm") == 0)    return FIF_XPM;
    else if (string.compareIgnoreCase("dds") == 0)    return FIF_DDS;
    else if (string.compareIgnoreCase("gif") == 0)    return FIF_GIF;
    else if (string.compareIgnoreCase("hdr") == 0)    return FIF_HDR;
    else if (string.compareIgnoreCase("g3") == 0)     return FIF_FAXG3;
    else if (string.compareIgnoreCase("faxg3") == 0)  return FIF_FAXG3;
    else if (string.compareIgnoreCase("sgi") == 0)    return FIF_SGI;
    else if (string.compareIgnoreCase("exr") == 0)    return FIF_EXR;
    else if (string.compareIgnoreCase("j2k") == 0)    return FIF_J2K;
    else if (string.compareIgnoreCase("j2c") == 0)    return FIF_J2K;
    else if (string.compareIgnoreCase("jp2") == 0)    return FIF_JP2;
    else if (string.compareIgnoreCase("pfm") == 0)    return FIF_PFM;
    else if (string.compareIgnoreCase("pct") == 0)    return FIF_PICT;
    else if (string.compareIgnoreCase("pict") == 0)   return FIF_PICT;
    else if (string.compareIgnoreCase("pic") == 0)    return FIF_PICT;
    else if (string.compareIgnoreCase("raw") == 0)    return FIF_RAW;
    else
        die("invalid FreeImage format string representation: %s", string.toRawUTF8());
}

template<>
treejuce::String to_string<FREE_IMAGE_FORMAT>(FREE_IMAGE_FORMAT arg)
{
    switch (arg)
    {
    case FIF_UNKNOWN: return "unknown";
    case FIF_BMP:     return "bmp";
    case FIF_ICO:     return "ico";
    case FIF_JPEG:    return "jpg";
    case FIF_JNG:     return "jng";
    case FIF_KOALA:   return "koa";
    case FIF_LBM:     return "iff";
    //case FIF_IFF:
    case FIF_MNG:     return "mng";
    case FIF_PBM:     return "pbm";
    case FIF_PBMRAW:  return "pbmraw";
    case FIF_PCD:     return "pcd";
    case FIF_PCX:     return "pcx";
    case FIF_PGM:     return "pgm";
    case FIF_PGMRAW:  return "pgmraw";
    case FIF_PNG:     return "png";
    case FIF_PPM:     return "ppm";
    case FIF_PPMRAW:  return "ppmraw";
    case FIF_RAS:     return "ras";
    case FIF_TARGA:   return "tga";
    case FIF_TIFF:    return "tif";
    case FIF_WBMP:    return "wbmp";
    case FIF_PSD:     return "psd";
    case FIF_CUT:     return "cut";
    case FIF_XBM:     return "xbm";
    case FIF_XPM:     return "xpm";
    case FIF_DDS:     return "dds";
    case FIF_GIF:     return "gif";
    case FIF_HDR:     return "hdr";
    case FIF_FAXG3:   return "g3";
    case FIF_SGI:     return "sgi";
    case FIF_EXR:     return "exr";
    case FIF_J2K:     return "j2k";
    case FIF_JP2:     return "jp2";
    case FIF_PFM:     return "pfm";
    case FIF_PICT:    return "pct";
    case FIF_RAW:     return "raw";
    default:
        die("invlid FreeImage format enum: %x", arg);
    }
}

template<>
FREE_IMAGE_TYPE from_string<FREE_IMAGE_TYPE>(const treejuce::String& string)
{
    if (string.compareIgnoreCase("unknown") == 0)
        return FIT_UNKNOWN;
    else if (string.compareIgnoreCase("bitmap") == 0)
        return FIT_BITMAP;
    else if (string.compareIgnoreCase("uint16") == 0)
        return FIT_UINT16;
    else if (string.compareIgnoreCase("int16") == 0)
        return FIT_INT16;
    else if (string.compareIgnoreCase("uint32") == 0)
        return FIT_UINT32;
    else if (string.compareIgnoreCase("int32") == 0)
        return FIT_INT32;
    else if (string.compareIgnoreCase("float") == 0)
        return FIT_FLOAT;
    else if (string.compareIgnoreCase("double") == 0)
        return FIT_DOUBLE;
    else if (string.compareIgnoreCase("complex") == 0)
        return FIT_COMPLEX;
    else if (string.compareIgnoreCase("rgb16") == 0)
        return FIT_RGB16;
    else if (string.compareIgnoreCase("rgba16") == 0)
        return FIT_RGBA16;
    else if (string.compareIgnoreCase("rgbf") == 0)
        return FIT_RGBF;
    else if (string.compareIgnoreCase("rgbaf") == 0)
        return FIT_RGBAF;
    else
        return FIT_UNKNOWN;
}

template<>
treejuce::String to_string<FREE_IMAGE_TYPE>(FREE_IMAGE_TYPE arg)
{
    switch(arg)
    {
    case FIT_UNKNOWN: return "unknown";
    case FIT_BITMAP:  return "bitmap";
    case FIT_UINT16:  return "uint16";
    case FIT_INT16:   return "int16";
    case FIT_UINT32:  return "uint32";
    case FIT_INT32:   return "int32";
    case FIT_FLOAT:   return "float";
    case FIT_DOUBLE:  return "double";
    case FIT_COMPLEX: return "complex";
    case FIT_RGB16:   return "rgb16";
    case FIT_RGBA16:  return "rgba16";
    case FIT_RGBF:    return "rgbf";
    case FIT_RGBAF:   return "rgbaf";
    default:          return "unknown";
    }
}

template<>
FREE_IMAGE_COLOR_TYPE from_string<FREE_IMAGE_COLOR_TYPE>(const treejuce::String& string)
{
    if      (string.compareIgnoreCase("miniswhite") == 0) return FIC_MINISWHITE;
    else if (string.compareIgnoreCase("minisblack") == 0) return FIC_MINISBLACK;
    else if (string.compareIgnoreCase("rgb") == 0)        return FIC_RGB;
    else if (string.compareIgnoreCase("palette") == 0)    return FIC_PALETTE;
    else if (string.compareIgnoreCase("rgbalpha") == 0)   return FIC_RGBALPHA;
    else if (string.compareIgnoreCase("rgba") == 0)       return FIC_RGBALPHA;
    else if (string.compareIgnoreCase("cmyk") == 0)       return FIC_CMYK;
    else
        die("invalid FreeImage color type string representation: %s", string.toRawUTF8());
}

template<>
treejuce::String to_string<FREE_IMAGE_COLOR_TYPE>(FREE_IMAGE_COLOR_TYPE arg)
{
    switch (arg)
    {
    case FIC_MINISWHITE: return "miniswhite";
    case FIC_MINISBLACK: return "minisblack";
    case FIC_RGB:        return "rgb";
    case FIC_PALETTE:    return "palette";
    case FIC_RGBALPHA:   return "rgba";
    case FIC_CMYK:       return "cmyk";
    default:
        die("invalid FreeImage color type enum: %x", arg);
    }
}

GLenum gl_internal_format_from_string(const treejuce::String& string)
{
    if      (string.compareIgnoreCase("alpha") == 0)             return GL_ALPHA;
    else if (string.compareIgnoreCase("blue") == 0)              return GL_BLUE;
    else if (string.compareIgnoreCase("green") == 0)             return GL_GREEN;
    else if (string.compareIgnoreCase("red") == 0)               return GL_RED;
    else if (string.compareIgnoreCase("rgb") == 0)               return GL_RGB;
    else if (string.compareIgnoreCase("rgba") == 0)              return GL_RGBA;

    else if (string.compareIgnoreCase("luminance_alpha") == 0)   return GL_LUMINANCE_ALPHA;
    else if (string.compareIgnoreCase("luminance") == 0)         return GL_LUMINANCE;
    else if (string.compareIgnoreCase("depth16") == 0)           return GL_DEPTH_COMPONENT16;
    else if (string.compareIgnoreCase("depth24") == 0)           return GL_DEPTH_COMPONENT24;
    else if (string.compareIgnoreCase("depth32f") == 0)          return GL_DEPTH_COMPONENT32F;
    else if (string.compareIgnoreCase("depth24_stencil8") == 0)  return GL_DEPTH24_STENCIL8;
    else if (string.compareIgnoreCase("depth32f_stencil8") == 0) return GL_DEPTH32F_STENCIL8;

    else if (string.compareIgnoreCase("r8") == 0)                return GL_R8;
    else if (string.compareIgnoreCase("r8_snorm") == 0)          return GL_R8_SNORM;
    else if (string.compareIgnoreCase("r16f") == 0)              return GL_R16F;
    else if (string.compareIgnoreCase("r32f") == 0)              return GL_R32F;
    else if (string.compareIgnoreCase("r16ui") == 0)             return GL_R16UI;
    else if (string.compareIgnoreCase("r16i") == 0)              return GL_R16I;
    else if (string.compareIgnoreCase("r32ui") == 0)             return GL_R32UI;
    else if (string.compareIgnoreCase("r32i") == 0)              return GL_R32I;
    else if (string.compareIgnoreCase("rg8") == 0)               return GL_RG8;
    else if (string.compareIgnoreCase("rg8_snorm") == 0)         return GL_RG8_SNORM;
    else if (string.compareIgnoreCase("rg16f") == 0)             return GL_RG16F;
    else if (string.compareIgnoreCase("rg32f") == 0)             return GL_RG32F;
    else if (string.compareIgnoreCase("rg8ui") == 0)             return GL_RG8UI;
    else if (string.compareIgnoreCase("rg8i") == 0)              return GL_RG8I;
    else if (string.compareIgnoreCase("rg16ui") == 0)            return GL_RG16UI;
    else if (string.compareIgnoreCase("rg16i") == 0)             return GL_RG16I;
    else if (string.compareIgnoreCase("rg32ui") == 0)            return GL_RG32UI;
    else if (string.compareIgnoreCase("rg32i") == 0)             return GL_RG32I;
    else if (string.compareIgnoreCase("rgb8") == 0)              return GL_RGB8;
    else if (string.compareIgnoreCase("srgb8") == 0)             return GL_SRGB8;
    else if (string.compareIgnoreCase("rgb565") == 0)            return GL_RGB565;
    else if (string.compareIgnoreCase("rgb8_snorm") == 0)        return GL_RGB8_SNORM;
    else if (string.compareIgnoreCase("r11f_g11f_b10f") == 0)    return GL_R11F_G11F_B10F;
    else if (string.compareIgnoreCase("rgb9_e5") == 0)           return GL_RGB9_E5;
    else if (string.compareIgnoreCase("rgb16f") == 0)            return GL_RGB16F;
    else if (string.compareIgnoreCase("rgb32f") == 0)            return GL_RGB32F;
    else if (string.compareIgnoreCase("rgb8ui") == 0)            return GL_RGB8UI;
    else if (string.compareIgnoreCase("rgb8i") == 0)             return GL_RGB8I;
    else if (string.compareIgnoreCase("rgb16ui") == 0)           return GL_RGB16UI;
    else if (string.compareIgnoreCase("rgb16i") == 0)            return GL_RGB16I;
    else if (string.compareIgnoreCase("rgb32ui") == 0)           return GL_RGB32UI;
    else if (string.compareIgnoreCase("rgb32i") == 0)            return GL_RGB32I;
    else if (string.compareIgnoreCase("rgba8") == 0)             return GL_RGBA8;
    else if (string.compareIgnoreCase("srgb8_alpha8") == 0)      return GL_SRGB8_ALPHA8;
    else if (string.compareIgnoreCase("rgba8_snorm") == 0)       return GL_RGBA8_SNORM;
    else if (string.compareIgnoreCase("rgb5_a1") == 0)           return GL_RGB5_A1;
    else if (string.compareIgnoreCase("rgba4") == 0)             return GL_RGBA4;
    else if (string.compareIgnoreCase("rgb10_a2") == 0)          return GL_RGB10_A2;
    else if (string.compareIgnoreCase("rgba16f") == 0)           return GL_RGBA16F;
    else if (string.compareIgnoreCase("rgba32f") == 0)           return GL_RGBA32F;
    else if (string.compareIgnoreCase("rgba8ui") == 0)           return GL_RGBA8UI;
    else if (string.compareIgnoreCase("rgba8i") == 0)            return GL_RGBA8I;
    else if (string.compareIgnoreCase("rgb10_a2ui") == 0)        return GL_RGB10_A2UI;
    else if (string.compareIgnoreCase("rgba16ui") == 0)          return GL_RGBA16UI;
    else if (string.compareIgnoreCase("rgba16i") == 0)           return GL_RGBA16I;
    else if (string.compareIgnoreCase("rgba32i") == 0)           return GL_RGBA32I;
    else if (string.compareIgnoreCase("rgba32ui") == 0)          return GL_RGBA32UI;

    else die("invalid GL internal format string representation: %s", string.toRawUTF8());
}

treejuce::String gl_internal_format_to_string(GLenum arg)
{
    switch(arg)
    {
    case GL_ALPHA:           return "alpha";
    case GL_BLUE:            return "blue";
    case GL_GREEN:           return "green";
    case GL_RED:             return "red";
    case GL_RGB:             return "rgb";
    case GL_RGBA:            return "rgba";
    case GL_LUMINANCE_ALPHA: return "luminance_alpha";
    case GL_LUMINANCE:       return "luminance";

    case GL_R8:             return "r8";
    case GL_R8_SNORM:       return "r8_snorm";
    case GL_R16F:           return "r16f";
    case GL_R32F:           return "r32f";
    case GL_R16UI:          return "r16ui";
    case GL_R16I:           return "r16i";
    case GL_R32UI:          return "r32ui";
    case GL_R32I:           return "r32i";
    case GL_RG8:            return "rg8";
    case GL_RG8_SNORM:      return "rg8_snorm";
    case GL_RG16F:          return "rg16f";
    case GL_RG32F:          return "rg32f";
    case GL_RG8UI:          return "rg8ui";
    case GL_RG8I:           return "rg8i";
    case GL_RG16UI:         return "rg16ui";
    case GL_RG16I:          return "rg16i";
    case GL_RG32UI:         return "rg32ui";
    case GL_RG32I:          return "rg32i";
    case GL_RGB8:           return "rgb8";
    case GL_SRGB8:          return "srgb8";
    case GL_RGB565:         return "rgb565";
    case GL_RGB8_SNORM:     return "rgb8_snorm";
    case GL_R11F_G11F_B10F: return "r11f_g11f_b10f";
    case GL_RGB9_E5:        return "rgb9_e5";
    case GL_RGB16F:         return "rgb16f";
    case GL_RGB32F:         return "rgb32f";
    case GL_RGB8UI:         return "rgb8ui";
    case GL_RGB8I:          return "rgb8i";
    case GL_RGB16UI:        return "rgb16ui";
    case GL_RGB16I:         return "rgb16i";
    case GL_RGB32UI:        return "rgb32ui";
    case GL_RGB32I:         return "rgb32i";
    case GL_RGBA8:          return "rgba8";
    case GL_SRGB8_ALPHA8:   return "srgb8_alpha8";
    case GL_RGBA8_SNORM:    return "rgba8_snorm";
    case GL_RGB5_A1:        return "rgb5_a1";
    case GL_RGBA4:          return "rgba4";
    case GL_RGB10_A2:       return "rgb10_a2";
    case GL_RGBA16F:        return "rgba16f";
    case GL_RGBA32F:        return "rgba32f";
    case GL_RGBA8UI:        return "rgba8ui";
    case GL_RGBA8I:         return "rgba8i";
    case GL_RGB10_A2UI:     return "rgb10_a2ui";
    case GL_RGBA16UI:       return "rgba16ui";
    case GL_RGBA16I:        return "rgba16i";
    case GL_RGBA32I:        return "rgba32i";
    case GL_RGBA32UI:       return "rgba32ui";

    case GL_DEPTH_COMPONENT16:  return "depth16";
    case GL_DEPTH_COMPONENT24:  return "depth24";
    case GL_DEPTH_COMPONENT32F: return "depth32f";
    case GL_DEPTH24_STENCIL8:   return "depth24_stencil8";
    case GL_DEPTH32F_STENCIL8:  return "depth32f_stencil8";
    default:
        die("invalid GL internal color enum: %d", arg);
    }
}

GLenum gl_type_from_string(const treejuce::String& str)
{
    if (str.compareIgnoreCase("float") == 0)                              return GL_FLOAT                        ;
    else if (str.compareIgnoreCase("byte") == 0)                          return GL_BYTE                         ;
    else if (str.compareIgnoreCase("unsigned_byte") == 0)                 return GL_UNSIGNED_BYTE                ;
    else if (str.compareIgnoreCase("float_vec2") == 0)                    return GL_FLOAT_VEC2                   ;
    else if (str.compareIgnoreCase("float_vec3") == 0)                    return GL_FLOAT_VEC3                   ;
    else if (str.compareIgnoreCase("float_vec4") == 0)                    return GL_FLOAT_VEC4                   ;
    else if (str.compareIgnoreCase("int") == 0)                           return GL_INT                          ;
    else if (str.compareIgnoreCase("int_vec2") == 0)                      return GL_INT_VEC2                     ;
    else if (str.compareIgnoreCase("int_vec3") == 0)                      return GL_INT_VEC3                     ;
    else if (str.compareIgnoreCase("int_vec4") == 0)                      return GL_INT_VEC4                     ;
    else if (str.compareIgnoreCase("unsigned_int") == 0)                  return GL_UNSIGNED_INT                 ;
    else if (str.compareIgnoreCase("unsigned_int_vec2") == 0)             return GL_UNSIGNED_INT_VEC2            ;
    else if (str.compareIgnoreCase("unsigned_int_vec3") == 0)             return GL_UNSIGNED_INT_VEC3            ;
    else if (str.compareIgnoreCase("unsigned_int_vec4") == 0)             return GL_UNSIGNED_INT_VEC4            ;
    else if (str.compareIgnoreCase("bool") == 0)                          return GL_BOOL                         ;
    else if (str.compareIgnoreCase("bool_vec2") == 0)                     return GL_BOOL_VEC2                    ;
    else if (str.compareIgnoreCase("bool_vec3") == 0)                     return GL_BOOL_VEC3                    ;
    else if (str.compareIgnoreCase("bool_vec4") == 0)                     return GL_BOOL_VEC4                    ;
    else if (str.compareIgnoreCase("float_mat2") == 0)                    return GL_FLOAT_MAT2                   ;
    else if (str.compareIgnoreCase("float_mat3") == 0)                    return GL_FLOAT_MAT3                   ;
    else if (str.compareIgnoreCase("float_mat4") == 0)                    return GL_FLOAT_MAT4                   ;
    else if (str.compareIgnoreCase("float_mat2x3") == 0)                  return GL_FLOAT_MAT2x3                 ;
    else if (str.compareIgnoreCase("float_mat2x4") == 0)                  return GL_FLOAT_MAT2x4                 ;
    else if (str.compareIgnoreCase("float_mat3x2") == 0)                  return GL_FLOAT_MAT3x2                 ;
    else if (str.compareIgnoreCase("float_mat3x4") == 0)                  return GL_FLOAT_MAT3x4                 ;
    else if (str.compareIgnoreCase("float_mat4x2") == 0)                  return GL_FLOAT_MAT4x2                 ;
    else if (str.compareIgnoreCase("float_mat4x3") == 0)                  return GL_FLOAT_MAT4x3                 ;
    else if (str.compareIgnoreCase("sampler_2d") == 0)                    return GL_SAMPLER_2D                   ;
    else if (str.compareIgnoreCase("sampler_3d") == 0)                    return GL_SAMPLER_3D                   ;
    else if (str.compareIgnoreCase("sampler_cube") == 0)                  return GL_SAMPLER_CUBE                 ;
    else if (str.compareIgnoreCase("sampler_2d_shadow") == 0)             return GL_SAMPLER_2D_SHADOW            ;
    else if (str.compareIgnoreCase("sampler_2d_array") == 0)              return GL_SAMPLER_2D_ARRAY             ;
    else if (str.compareIgnoreCase("sampler_2d_array_shadow") == 0)       return GL_SAMPLER_2D_ARRAY_SHADOW      ;
    else if (str.compareIgnoreCase("sampler_cube_shadow") == 0)           return GL_SAMPLER_CUBE_SHADOW          ;
    else if (str.compareIgnoreCase("int_sampler_2d") == 0)                return GL_INT_SAMPLER_2D               ;
    else if (str.compareIgnoreCase("int_sampler_3d") == 0)                return GL_INT_SAMPLER_3D               ;
    else if (str.compareIgnoreCase("int_sampler_cube") == 0)              return GL_INT_SAMPLER_CUBE             ;
    else if (str.compareIgnoreCase("int_sampler_2d_array") == 0)          return GL_INT_SAMPLER_2D_ARRAY         ;
    else if (str.compareIgnoreCase("unsigned_int_sampler_2d") == 0)       return GL_UNSIGNED_INT_SAMPLER_2D      ;
    else if (str.compareIgnoreCase("unsigned_int_sampler_3d") == 0)       return GL_UNSIGNED_INT_SAMPLER_3D      ;
    else if (str.compareIgnoreCase("unsigned_int_sampler_cube") == 0)     return GL_UNSIGNED_INT_SAMPLER_CUBE    ;
    else if (str.compareIgnoreCase("unsigned_int_sampler_2d_array") == 0) return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY;
    else die("invalid GL type string representation: %s", str.toRawUTF8());
}

treejuce::String gl_type_to_string(GLenum arg)
{// TODO short?
    switch (arg)
    {
    case GL_BYTE                         : return "byte";
    case GL_UNSIGNED_BYTE                : return "unsigned_byte";
    case GL_FLOAT                        : return "float";
    case GL_FLOAT_VEC2                   : return "float_vec2";
    case GL_FLOAT_VEC3                   : return "float_vec3";
    case GL_FLOAT_VEC4                   : return "float_vec4";
    case GL_INT                          : return "int";
    case GL_INT_VEC2                     : return "int_vec2";
    case GL_INT_VEC3                     : return "int_vec3";
    case GL_INT_VEC4                     : return "int_vec4";
    case GL_UNSIGNED_INT                 : return "unsigned_int";
    case GL_UNSIGNED_INT_VEC2            : return "unsigned_int_vec2";
    case GL_UNSIGNED_INT_VEC3            : return "unsigned_int_vec3";
    case GL_UNSIGNED_INT_VEC4            : return "unsigned_int_vec4";
    case GL_BOOL                         : return "bool";
    case GL_BOOL_VEC2                    : return "bool_vec2";
    case GL_BOOL_VEC3                    : return "bool_vec3";
    case GL_BOOL_VEC4                    : return "bool_vec4";
    case GL_FLOAT_MAT2                   : return "float_mat2";
    case GL_FLOAT_MAT3                   : return "float_mat3";
    case GL_FLOAT_MAT4                   : return "float_mat4";
    case GL_FLOAT_MAT2x3                 : return "float_mat2x3";
    case GL_FLOAT_MAT2x4                 : return "float_mat2x4";
    case GL_FLOAT_MAT3x2                 : return "float_mat3x2";
    case GL_FLOAT_MAT3x4                 : return "float_mat3x4";
    case GL_FLOAT_MAT4x2                 : return "float_mat4x2";
    case GL_FLOAT_MAT4x3                 : return "float_mat4x3";
    case GL_SAMPLER_2D                   : return "sampler_2d";
    case GL_SAMPLER_3D                   : return "sampler_3d";
    case GL_SAMPLER_CUBE                 : return "sampler_cube";
    case GL_SAMPLER_2D_SHADOW            : return "sampler_2d_shadow";
    case GL_SAMPLER_2D_ARRAY             : return "sampler_2d_array";
    case GL_SAMPLER_2D_ARRAY_SHADOW      : return "sampler_2d_array_shadow";
    case GL_SAMPLER_CUBE_SHADOW          : return "sampler_cube_shadow";
    case GL_INT_SAMPLER_2D               : return "int_sampler_2d";
    case GL_INT_SAMPLER_3D               : return "int_sampler_3d";
    case GL_INT_SAMPLER_CUBE             : return "int_sampler_cube";
    case GL_INT_SAMPLER_2D_ARRAY         : return "int_sampler_2d_array";
    case GL_UNSIGNED_INT_SAMPLER_2D      : return "unsigned_int_sampler_2d";
    case GL_UNSIGNED_INT_SAMPLER_3D      : return "unsigned_int_sampler_3d";
    case GL_UNSIGNED_INT_SAMPLER_CUBE    : return "unsigned_int_sampler_cube";
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "unsigned_int_sampler_2d_array";
    default:
        die("invalid GL type enum: %x", arg);
    }
}

GLenum gl_primitive_from_string(const treejuce::String& str)
{
    if      (str.compareIgnoreCase("points"        ) == 0) return GL_POINTS        ;
    else if (str.compareIgnoreCase("lines"         ) == 0) return GL_LINES         ;
    else if (str.compareIgnoreCase("line_strip"    ) == 0) return GL_LINE_STRIP    ;
    else if (str.compareIgnoreCase("line_loop"     ) == 0) return GL_LINE_LOOP     ;
    else if (str.compareIgnoreCase("triangles"     ) == 0) return GL_TRIANGLES     ;
    else if (str.compareIgnoreCase("triangle_strip") == 0) return GL_TRIANGLE_STRIP;
    else if (str.compareIgnoreCase("triangle_fan"  ) == 0) return GL_TRIANGLE_FAN  ;
    else
        die("invalid GL primitive string representation: %s", str.toRawUTF8());
}

treejuce::String gl_primitive_to_string(GLenum arg)
{
    switch (arg)
    {
    case GL_POINTS        : return "points"        ;
    case GL_LINES         : return "lines"         ;
    case GL_LINE_STRIP    : return "line_strip"    ;
    case GL_LINE_LOOP     : return "line_loop"     ;
    case GL_TRIANGLES     : return "triangles"     ;
    case GL_TRIANGLE_STRIP: return "triangle_strip";
    case GL_TRIANGLE_FAN  : return "triangle_fan"  ;
    default:
        die("invalid GL primitive enum: %x", arg);
    }
}

GLenum gl_tex_wrap_from_string(const treejuce::String& str)
{
    if (str.compareIgnoreCase("clamp_to_edge") == 0)
        return GL_CLAMP_TO_EDGE;
    else if (str.compareIgnoreCase("mirrored_repeat") == 0)
        return GL_MIRRORED_REPEAT;
    else if (str.compareIgnoreCase("repeat") == 0)
        return GL_REPEAT;
    else
        die("invalid GL texture wrap string representation: %s", str.toRawUTF8());
}

treejuce::String gl_tex_wrap_to_string(GLenum arg)
{
    switch (arg)
    {
    case GL_CLAMP_TO_EDGE:   return "clamp_to_edge";
    case GL_MIRRORED_REPEAT: return "mirrored_repeat";
    case GL_REPEAT:          return "repeat";
    default:
        die("invalid GL texture wrap enum: %x", arg);
    }
}


TREEFACE_NAMESPACE_END
