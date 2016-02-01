#include "treeface/misc/StringCast.h"

#include <stdexcept>

using namespace treeface;

namespace treecore
{

template<>
bool fromString<treeface::MaterialType>( const treecore::String& string, treeface::MaterialType& result )
{
    String str_lc = string.toLowerCase();
    if      (str_lc == "raw")             result = treeface::MATERIAL_RAW;
    else if (str_lc == "scene_graph")     result = treeface::MATERIAL_SCENE_GRAPH;
    else if (str_lc == "screen_space")    result = treeface::MATERIAL_SCREEN_SPACE;
    else if (str_lc == "vector_graphics") result = treeface::MATERIAL_VECTOR_GRAPHICS;
    else return false;

    return true;
}

template<>
treecore::String toString<treeface::MaterialType>( treeface::MaterialType value )
{
    switch (value)
    {
    case treeface::MATERIAL_RAW:             return "raw";
    case treeface::MATERIAL_SCENE_GRAPH:     return "scene_graph";
    case treeface::MATERIAL_SCREEN_SPACE:    return "screen_space";
    case treeface::MATERIAL_VECTOR_GRAPHICS: return "vector_graphics";
    default:
        throw std::invalid_argument( ( "invalid treeface material type enum: " + String( int(value) ) ).toRawUTF8() );
    }
}

template<>
bool fromString<FREE_IMAGE_FORMAT>( const treecore::String& string, FREE_IMAGE_FORMAT& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "unknown")      result = FIF_UNKNOWN;
    else if (str_lc == "bmp")     result = FIF_BMP;
    else if (str_lc == "ico")     result = FIF_ICO;
    else if (str_lc == "jpg")     result = FIF_JPEG;
    else if (str_lc == "jpe")     result = FIF_JPEG;
    else if (str_lc == "jif")     result = FIF_JPEG;
    else if (str_lc == "jpeg")    result = FIF_JPEG;
    else if (str_lc == "jng")     result = FIF_JNG;
    else if (str_lc == "koa")     result = FIF_KOALA;
    else if (str_lc == "koala")   result = FIF_KOALA;
    else if (str_lc == "iff")     result = FIF_IFF;
    else if (str_lc == "lbm")     result = FIF_LBM;
    else if (str_lc == "mng")     result = FIF_MNG;
    else if (str_lc == "pbm")     result = FIF_PBM;
    else if (str_lc == "pbmraw")  result = FIF_PBMRAW;
    else if (str_lc == "pcd")     result = FIF_PCD;
    else if (str_lc == "pcx")     result = FIF_PCX;
    else if (str_lc == "pgm")     result = FIF_PGM;
    else if (str_lc == "pgmraw")  result = FIF_PGMRAW;
    else if (str_lc == "png")     result = FIF_PNG;
    else if (str_lc == "ppm")     result = FIF_PPM;
    else if (str_lc == "ppmraw")  result = FIF_PPMRAW;
    else if (str_lc == "ras")     result = FIF_RAS;
    else if (str_lc == "tga")     result = FIF_TARGA;
    else if (str_lc == "targa")   result = FIF_TARGA;
    else if (str_lc == "tif")     result = FIF_TIFF;
    else if (str_lc == "tiff")    result = FIF_TIFF;
    else if (str_lc == "wbmp")    result = FIF_WBMP;
    else if (str_lc == "psd")     result = FIF_PSD;
    else if (str_lc == "cut")     result = FIF_CUT;
    else if (str_lc == "xbm")     result = FIF_XBM;
    else if (str_lc == "xpm")     result = FIF_XPM;
    else if (str_lc == "dds")     result = FIF_DDS;
    else if (str_lc == "gif")     result = FIF_GIF;
    else if (str_lc == "hdr")     result = FIF_HDR;
    else if (str_lc == "g3")      result = FIF_FAXG3;
    else if (str_lc == "faxg3")   result = FIF_FAXG3;
    else if (str_lc == "sgi")     result = FIF_SGI;
    else if (str_lc == "exr")     result = FIF_EXR;
    else if (str_lc == "j2k")     result = FIF_J2K;
    else if (str_lc == "j2c")     result = FIF_J2K;
    else if (str_lc == "jp2")     result = FIF_JP2;
    else if (str_lc == "pfm")     result = FIF_PFM;
    else if (str_lc == "pct")     result = FIF_PICT;
    else if (str_lc == "pict")    result = FIF_PICT;
    else if (str_lc == "pic")     result = FIF_PICT;
    else if (str_lc == "raw")     result = FIF_RAW;
    else
        return false;

    return true;
}

template<>
treecore::String toString<FREE_IMAGE_FORMAT>( FREE_IMAGE_FORMAT arg )
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
    default: throw std::invalid_argument( ( "invalid FreeImage format enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
bool fromString<FREE_IMAGE_TYPE>( const treecore::String& string, FREE_IMAGE_TYPE& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "unknown") result = FIT_UNKNOWN;
    else if (str_lc == "bitmap")  result = FIT_BITMAP;
    else if (str_lc == "uint16")  result = FIT_UINT16;
    else if (str_lc == "int16")   result = FIT_INT16;
    else if (str_lc == "uint32")  result = FIT_UINT32;
    else if (str_lc == "int32")   result = FIT_INT32;
    else if (str_lc == "float")   result = FIT_FLOAT;
    else if (str_lc == "double")  result = FIT_DOUBLE;
    else if (str_lc == "complex") result = FIT_COMPLEX;
    else if (str_lc == "rgb16")   result = FIT_RGB16;
    else if (str_lc == "rgba16")  result = FIT_RGBA16;
    else if (str_lc == "rgbf")    result = FIT_RGBF;
    else if (str_lc == "rgbaf")   result = FIT_RGBAF;
    else
        return false;

    return true;
}

template<>
treecore::String toString<FREE_IMAGE_TYPE>( FREE_IMAGE_TYPE arg )
{
    switch (arg)
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
    default:
        throw std::invalid_argument( ( "invalid FreeImage type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
bool fromString<FREE_IMAGE_COLOR_TYPE>( const treecore::String& string, FREE_IMAGE_COLOR_TYPE& result )
{
    String str_lc = string.toLowerCase();
    if      (str_lc == "miniswhite")  result = FIC_MINISWHITE;
    else if (str_lc == "minisblack")  result = FIC_MINISBLACK;
    else if (str_lc == "rgb")         result = FIC_RGB;
    else if (str_lc == "palette")     result = FIC_PALETTE;
    else if (str_lc == "rgbalpha")    result = FIC_RGBALPHA;
    else if (str_lc == "rgba")        result = FIC_RGBALPHA;
    else if (str_lc == "cmyk")        result = FIC_CMYK;
    else
        return false;

    return true;
}

template<>
treecore::String toString<FREE_IMAGE_COLOR_TYPE>( FREE_IMAGE_COLOR_TYPE arg )
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
        throw std::invalid_argument( ( "invalid FreeImage color type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
bool fromString<LineCap>( const String& str, LineCap& result )
{
    String value_lc = str.toLowerCase();

    if      ( value_lc.contains( "butt" ) )   result = LINE_CAP_BUTT;
    else if ( value_lc.contains( "round" ) )  result = LINE_CAP_ROUND;
    else if ( value_lc.contains( "square" ) ) result = LINE_CAP_SQUARE;
    else
        return false;

    return true;
}

template<>
bool fromString<LineJoin>( const String& str, LineJoin& result )
{
    String value_lc = str.toLowerCase();

    if ( value_lc.contains( "bevel" ) ) result = LINE_JOIN_BEVEL;
    else if ( value_lc.contains( "miter" ) ) result = LINE_JOIN_MITER;
    else if ( value_lc.contains( "round" ) ) result = LINE_JOIN_ROUND;
    else
        return false;

    return true;
}

template<>
treecore::String toString<LineCap>( LineCap cap )
{
    switch (cap)
    {
    case LINE_CAP_BUTT: return "butt";
    case LINE_CAP_ROUND: return "round";
    case LINE_CAP_SQUARE: return "square";
    default:
        throw std::invalid_argument( ( "invalid treeface line cap enum: " + String( int(cap) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<LineJoin>( LineJoin join )
{
    switch (join)
    {
    case LINE_JOIN_BEVEL: return "bevel";
    case LINE_JOIN_MITER: return "miter";
    case LINE_JOIN_ROUND: return "round";
    default:
        throw std::invalid_argument( ( "invalid treeface line join enum: " + String( int(join) ) ).toRawUTF8() );
    }
}

template<>
bool fromString<treeface::GLBufferType>( const treecore::String& string, treeface::GLBufferType& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "vertex" ||
             str_lc == "array")              result = TFGL_BUFFER_VERTEX;
    else if (str_lc == "index" ||
             str_lc == "element_array")      result = TFGL_BUFFER_INDEX;
    else if (str_lc == "pack" ||
             str_lc == "pixel_pack")         result = TFGL_BUFFER_PACK;
    else if (str_lc == "unpack" ||
             str_lc == "pixel_unpack")       result = TFGL_BUFFER_UNPACK;
    else if (str_lc == "read" ||
             str_lc == "copy_read")          result = TFGL_BUFFER_READ;
    else if (str_lc == "write" ||
             str_lc == "copy_write")         result = TFGL_BUFFER_WRITE;
    else if (str_lc == "feedback" ||
             str_lc == "transform_feedback") result = TFGL_BUFFER_FEEDBACK;
    else if (str_lc == "uniform")            result = TFGL_BUFFER_UNIFORM;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLBufferUsage>( const treecore::String& string, treeface::GLBufferUsage& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "stream_draw")  result = TFGL_BUFFER_STREAM_DRAW;
    else if (str_lc == "stream_read")  result = TFGL_BUFFER_STREAM_READ;
    else if (str_lc == "stream_copy")  result = TFGL_BUFFER_STREAM_COPY;
    else if (str_lc == "static_draw")  result = TFGL_BUFFER_STATIC_DRAW;
    else if (str_lc == "static_read")  result = TFGL_BUFFER_STATIC_READ;
    else if (str_lc == "static_copy")  result = TFGL_BUFFER_STATIC_COPY;
    else if (str_lc == "dynamic_draw") result = TFGL_BUFFER_DYNAMIC_DRAW;
    else if (str_lc == "dynamic_read") result = TFGL_BUFFER_DYNAMIC_READ;
    else if (str_lc == "dynamic_copy") result = TFGL_BUFFER_DYNAMIC_COPY;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLImageFormat>( const treecore::String& string, treeface::GLImageFormat& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "alpha")           result = TFGL_IMAGE_FORMAT_ALPHA;
    else if (str_lc == "rgba")            result = TFGL_IMAGE_FORMAT_RGBA;
    else if (str_lc == "rgb")             result = TFGL_IMAGE_FORMAT_RGB;
    else if (str_lc == "rg")              result = TFGL_IMAGE_FORMAT_RG;
    else if (str_lc == "red")             result = TFGL_IMAGE_FORMAT_RED;
    else if (str_lc == "rgba_int")        result = TFGL_IMAGE_FORMAT_RGBA_INT;
    else if (str_lc == "rgb_int")         result = TFGL_IMAGE_FORMAT_RGB_INT;
    else if (str_lc == "rg_int")          result = TFGL_IMAGE_FORMAT_RG_INT;
    else if (str_lc == "red_int")         result = TFGL_IMAGE_FORMAT_RED_INT;
    else if (str_lc == "depth")           result = TFGL_IMAGE_FORMAT_DEPTH;
    else if (str_lc == "depth_stencil")   result = TFGL_IMAGE_FORMAT_DEPTH_STENCIL;
    else if (str_lc == "luminance_")      result = TFGL_IMAGE_FORMAT_LUMINANCE;
    else if (str_lc == "luminance_alpha") result = TFGL_IMAGE_FORMAT_LUMINANCE_ALPHA;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLInternalImageFormat>( const treecore::String& string, treeface::GLInternalImageFormat& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "alpha")             result = TFGL_INTERNAL_IMAGE_FORMAT_ALPHA;
    else if (str_lc == "blue")              result = TFGL_INTERNAL_IMAGE_FORMAT_BLUE;
    else if (str_lc == "green")             result = TFGL_INTERNAL_IMAGE_FORMAT_GREEN;
    else if (str_lc == "red")               result = TFGL_INTERNAL_IMAGE_FORMAT_RED;
    else if (str_lc == "rgb")               result = TFGL_INTERNAL_IMAGE_FORMAT_RGB;
    else if (str_lc == "rgba")              result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA;
    else if (str_lc == "luminance_alpha")   result = TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE_ALPHA;
    else if (str_lc == "luminance")         result = TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE;
    else if (str_lc == "depth_component16" ||
             str_lc == "depth16")           result = TFGL_INTERNAL_IMAGE_FORMAT_DEPTH16;
    else if (str_lc == "depth_component24" ||
             str_lc == "depth24")           result = TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24;
    else if (str_lc == "depth_component32f" ||
             str_lc == "depth32f")          result = TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F;
    else if (str_lc == "depth24_stencil8")  result = TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24_STENCIL8;
    else if (str_lc == "depth32f_stencil8") result = TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F_STENCIL8;
    else if (str_lc == "r8" ||
             str_lc == "red8")              result = TFGL_INTERNAL_IMAGE_FORMAT_R8;
    else if (str_lc == "r8_snorm" ||
             str_lc == "red8_snorm")        result = TFGL_INTERNAL_IMAGE_FORMAT_R8_SNORM;
    else if (str_lc == "r16f" ||
             str_lc == "red16f")            result = TFGL_INTERNAL_IMAGE_FORMAT_R16F;
    else if (str_lc == "r32f" ||
             str_lc == "red32f")            result = TFGL_INTERNAL_IMAGE_FORMAT_R32F;
    else if (str_lc == "r16ui" ||
             str_lc == "red16ui")           result = TFGL_INTERNAL_IMAGE_FORMAT_R16UI;
    else if (str_lc == "r16i" ||
             str_lc == "red16i")            result = TFGL_INTERNAL_IMAGE_FORMAT_R16I;
    else if (str_lc == "r32ui" ||
             str_lc == "red32ui")           result = TFGL_INTERNAL_IMAGE_FORMAT_R32UI;
    else if (str_lc == "r32i" ||
             str_lc == "red32i")            result = TFGL_INTERNAL_IMAGE_FORMAT_R32I;
    else if (str_lc == "rg8")               result = TFGL_INTERNAL_IMAGE_FORMAT_RG8;
    else if (str_lc == "rg8_snorm")         result = TFGL_INTERNAL_IMAGE_FORMAT_RG8_SNORM;
    else if (str_lc == "rg16f")             result = TFGL_INTERNAL_IMAGE_FORMAT_RG16F;
    else if (str_lc == "rg32f")             result = TFGL_INTERNAL_IMAGE_FORMAT_RG32F;
    else if (str_lc == "rg8ui")             result = TFGL_INTERNAL_IMAGE_FORMAT_RG8UI;
    else if (str_lc == "rg8i")              result = TFGL_INTERNAL_IMAGE_FORMAT_RG8I;
    else if (str_lc == "rg16ui")            result = TFGL_INTERNAL_IMAGE_FORMAT_RG16UI;
    else if (str_lc == "rg16i")             result = TFGL_INTERNAL_IMAGE_FORMAT_RG16I;
    else if (str_lc == "rg32ui")            result = TFGL_INTERNAL_IMAGE_FORMAT_RG32UI;
    else if (str_lc == "rg32i")             result = TFGL_INTERNAL_IMAGE_FORMAT_RG32I;
    else if (str_lc == "rgb8")              result = TFGL_INTERNAL_IMAGE_FORMAT_RGB8;
    else if (str_lc == "srgb8")             result = TFGL_INTERNAL_IMAGE_FORMAT_SRGB8;
    else if (str_lc == "rgb565")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB565;
    else if (str_lc == "rgb8_snorm")        result = TFGL_INTERNAL_IMAGE_FORMAT_RGB8_SNORM;
    else if (str_lc == "r11f_g11f_b10f")    result = TFGL_INTERNAL_IMAGE_FORMAT_R11F_G11F_B10F;
    else if (str_lc == "rgb9_e5")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGB9_E5;
    else if (str_lc == "rgb16f")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB16F;
    else if (str_lc == "rgb32f")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB32F;
    else if (str_lc == "rgb8ui")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI;
    else if (str_lc == "rgb8i")             result = TFGL_INTERNAL_IMAGE_FORMAT_RGB8I;
    else if (str_lc == "rgb16ui")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGB16UI;
    else if (str_lc == "rgb16i")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB16I;
    else if (str_lc == "rgb32ui")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGB32UI;
    else if (str_lc == "rgb32i")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGB32I;
    else if (str_lc == "rgba8")             result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA8;
    else if (str_lc == "srgb8_alpha8")      result = TFGL_INTERNAL_IMAGE_FORMAT_SRGB8_ALPHA8;
    else if (str_lc == "rgba8_snorm")       result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA8_SNORM;
    else if (str_lc == "rgb5_a1")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGB5_A1;
    else if (str_lc == "rgba4")             result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA4;
    else if (str_lc == "rgb10_a2")          result = TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2;
    else if (str_lc == "rgba16f")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA16F;
    else if (str_lc == "rgba32f")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA32F;
    else if (str_lc == "rgba8ui")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI;
    else if (str_lc == "rgba8i")            result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I;
    else if (str_lc == "rgb10_a2ui")        result = TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2UI;
    else if (str_lc == "rgba16ui")          result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA16UI;
    else if (str_lc == "rgba16i")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA16I;
    else if (str_lc == "rgba32i")           result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA32I;
    else if (str_lc == "rgba32ui")          result = TFGL_INTERNAL_IMAGE_FORMAT_RGBA32UI;
    else
        return false;
    return true;
}

template<>
bool fromString<treeface::GLImageDataType>( const String& str, treeface::GLImageDataType& result )
{
    String str_lc = str.toLowerCase();

    if      (str_lc == "byte")                              result = TFGL_IMAGE_DATA_BYTE;
    else if (str_lc == "unsigned_byte")                     result = TFGL_IMAGE_DATA_UNSIGNED_BYTE;
    else if (str_lc == "short")                             result = TFGL_IMAGE_DATA_SHORT;
    else if (str_lc == "unsigned_short" || str == "ushort") result = TFGL_IMAGE_DATA_UNSIGNED_SHORT;
    else if (str_lc == "5_6_5")                             result = TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_6_5;
    else if (str_lc == "4_4_4_4")                           result = TFGL_IMAGE_DATA_UNSIGNED_SHORT_4_4_4_4;
    else if (str_lc == "5_5_5_1")                           result = TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_5_5_1;
    else if (str_lc == "int")                               result = TFGL_IMAGE_DATA_INT;
    else if (str_lc == "unsigned_int" || str == "uint")     result = TFGL_IMAGE_DATA_UNSIGNED_INT;
    else if (str_lc == "10f_11f_11f_rev")                   result = TFGL_IMAGE_DATA_UNSIGNED_INT_10F_11F_11F_REV;
    else if (str_lc == "5_9_9_9_rev")                       result = TFGL_IMAGE_DATA_UNSIGNED_INT_5_9_9_9_REV;
    else if (str_lc == "2_10_10_10_rev")                    result = TFGL_IMAGE_DATA_UNSIGNED_INT_2_10_10_10_REV;
    else if (str_lc == "24_8")                              result = TFGL_IMAGE_DATA_UNSIGNED_INT_24_8;
    else if (str_lc == "half_float")                        result = TFGL_IMAGE_DATA_HALF_FLOAT;
    else if (str_lc == "float")                             result = TFGL_IMAGE_DATA_FLOAT;
    else if (str_lc == "f32_24_8")                          result = TFGL_IMAGE_DATA_FLOAT_32_UNSIGNED_INT_24_8_REV;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLPrimitive>( const treecore::String& string, treeface::GLPrimitive& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "points")         result = TFGL_PRIMITIVE_POINTS;
    else if (str_lc == "lines")          result = TFGL_PRIMITIVE_LINES;
    else if (str_lc == "line_strip")     result = TFGL_PRIMITIVE_LINE_STRIP;
    else if (str_lc == "line_loop")      result = TFGL_PRIMITIVE_LINE_LOOP;
    else if (str_lc == "triangles")      result = TFGL_PRIMITIVE_TRIANGLES;
    else if (str_lc == "triangle_strip") result = TFGL_PRIMITIVE_TRIANGLE_STRIP;
    else if (str_lc == "triangle_fan")   result = TFGL_PRIMITIVE_TRIANGLE_FAN;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLTextureType>( const treecore::String& string, treeface::GLTextureType& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "2d")       result = TFGL_TEXTURE_2D;
    else if (str_lc == "3d")       result = TFGL_TEXTURE_3D;
    else if (str_lc == "2d_array") result = TFGL_TEXTURE_2D_ARRAY;
    else if (str_lc == "cube")     result = TFGL_TEXTURE_CUBE;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLTextureFilter>( const treecore::String& string, treeface::GLTextureFilter& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "nearest")                result = TFGL_TEXTURE_NEAREST;
    else if (str_lc == "linear")                 result = TFGL_TEXTURE_LINEAR;
    else if (str_lc == "nearest_mipmap_nearest") result = TFGL_TEXTURE_NEAREST_MIPMAP_NEAREST;
    else if (str_lc == "nearest_mipmap_linear")  result = TFGL_TEXTURE_NEAREST_MIPMAP_LINEAR;
    else if (str_lc == "linear_mipmap_nearest")  result = TFGL_TEXTURE_LINEAR_MIPMAP_NEAREST;
    else if (str_lc == "linear_mipmap_linear")   result = TFGL_TEXTURE_LINEAR_MIPMAP_LINEAR;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLTextureWrap>( const treecore::String& string, treeface::GLTextureWrap& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "clamp_to_edge")   result = TFGL_TEXTURE_CLAMP_TO_EDGE;
    else if (str_lc == "mirrored_repeat") result = TFGL_TEXTURE_MIRRORED_REPEAT;
    else if (str_lc == "repeat")          result = TFGL_TEXTURE_REPEAT;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::GLType>( const treecore::String& string, treeface::GLType& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "byte")                                   result = TFGL_TYPE_BYTE;
    else if (str_lc == "unsigned_byte" || str_lc == "ubyte")      result = TFGL_TYPE_UNSIGNED_BYTE;
    else if (str_lc == "float")                                  result = TFGL_TYPE_FLOAT;
    else if (str_lc == "float_vec2" || str_lc == "vec2f")        result = TFGL_TYPE_FLOAT_VEC2;
    else if (str_lc == "float_vec3" || str_lc == "vec3f")        result = TFGL_TYPE_FLOAT_VEC3;
    else if (str_lc == "float_vec4" || str_lc == "vec3f")        result = TFGL_TYPE_FLOAT_VEC4;
    else if (str_lc == "int")                                    result = TFGL_TYPE_INT;
    else if (str_lc == "int_vec2" || str_lc == "vec2i")          result = TFGL_TYPE_INT_VEC2;
    else if (str_lc == "int_vec3" || str_lc == "vec3i")          result = TFGL_TYPE_INT_VEC3;
    else if (str_lc == "int_vec4" || str_lc == "vec4i")          result = TFGL_TYPE_INT_VEC4;
    else if (str_lc == "unsigned_int" || str_lc == "uint")       result = TFGL_TYPE_UNSIGNED_INT;
    else if (str_lc == "unsigned_int_vec2" || str_lc == "vec2u") result = TFGL_TYPE_UNSIGNED_INT_VEC2;
    else if (str_lc == "unsigned_int_vec3" || str_lc == "vec3u") result = TFGL_TYPE_UNSIGNED_INT_VEC3;
    else if (str_lc == "unsigned_int_vec4" || str_lc == "vec4u") result = TFGL_TYPE_UNSIGNED_INT_VEC4;
    else if (str_lc == "bool")                                   result = TFGL_TYPE_BOOL;
    else if (str_lc == "bool_vec2" || str_lc == "vec2b")         result = TFGL_TYPE_BOOL_VEC2;
    else if (str_lc == "bool_vec3" || str_lc == "vec3b")         result = TFGL_TYPE_BOOL_VEC3;
    else if (str_lc == "bool_vec4" || str_lc == "vec4b")         result = TFGL_TYPE_BOOL_VEC4;
    else if (str_lc == "float_mat2" || str_lc == "mat2f")        result = TFGL_TYPE_FLOAT_MAT2;
    else if (str_lc == "float_mat3" || str_lc == "mat3f")        result = TFGL_TYPE_FLOAT_MAT3;
    else if (str_lc == "float_mat4" || str_lc == "mat4f")        result = TFGL_TYPE_FLOAT_MAT4;
    else if (str_lc == "float_mat2x3" || str_lc == "mat2x3f")    result = TFGL_TYPE_FLOAT_MAT2x3;
    else if (str_lc == "float_mat2x4" || str_lc == "mat2x4f")    result = TFGL_TYPE_FLOAT_MAT2x4;
    else if (str_lc == "float_mat3x2" || str_lc == "mat3x2f")    result = TFGL_TYPE_FLOAT_MAT3x2;
    else if (str_lc == "float_mat3x4" || str_lc == "mat3x4f")    result = TFGL_TYPE_FLOAT_MAT3x4;
    else if (str_lc == "float_mat4x2" || str_lc == "mat4x2f")    result = TFGL_TYPE_FLOAT_MAT4x2;
    else if (str_lc == "float_mat4x3" || str_lc == "mat4x3f")    result = TFGL_TYPE_FLOAT_MAT4x3;
    else if (str_lc == "sampler_2d")                             result = TFGL_TYPE_SAMPLER_2D;
    else if (str_lc == "sampler_3d")                             result = TFGL_TYPE_SAMPLER_3D;
    else if (str_lc == "sampler_cube")                           result = TFGL_TYPE_SAMPLER_CUBE;
    else if (str_lc == "sampler_2d_shadow")                      result = TFGL_TYPE_SAMPLER_2D_SHADOW;
    else if (str_lc == "sampler_2d_array")                       result = TFGL_TYPE_SAMPLER_2D_ARRAY;
    else if (str_lc == "sampler_2d_array_shadow")                result = TFGL_TYPE_SAMPLER_2D_ARRAY_SHADOW;
    else if (str_lc == "sampler_cube_shadow")                    result = TFGL_TYPE_SAMPLER_CUBE_SHADOW;
    else if (str_lc == "int_sampler_2d")                         result = TFGL_TYPE_INT_SAMPLER_2D;
    else if (str_lc == "int_sampler_3d")                         result = TFGL_TYPE_INT_SAMPLER_3D;
    else if (str_lc == "int_sampler_cube")                       result = TFGL_TYPE_INT_SAMPLER_CUBE;
    else if (str_lc == "int_sampler_2d_array")                   result = TFGL_TYPE_INT_SAMPLER_2D_ARRAY;
    else if (str_lc == "unsigned_int_sampler_2d")                result = TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D;
    else if (str_lc == "unsigned_int_sampler_3d")                result = TFGL_TYPE_UNSIGNED_INT_SAMPLER_3D;
    else if (str_lc == "unsigned_int_sampler_cube")              result = TFGL_TYPE_UNSIGNED_INT_SAMPLER_CUBE;
    else if (str_lc == "unsigned_int_sampler_2d_array")          result = TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D_ARRAY;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::TextureImageSoloChannelPolicy>( const treecore::String& string, treeface::TextureImageSoloChannelPolicy& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "red")       result = TEXTURE_IMAGE_SOLO_AS_RED;
    else if (str_lc == "luminance") result = TEXTURE_IMAGE_SOLO_AS_LUMINANCE;
    else if (str_lc == "alpha")     result = TEXTURE_IMAGE_SOLO_AS_ALPHA;
    else if (str_lc == "depth")     result = TEXTURE_IMAGE_SOLO_AS_DEPTH;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::TextureImageDualChannelPolicy>( const treecore::String& string, treeface::TextureImageDualChannelPolicy& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "red_green")       result = TEXTURE_IMAGE_DUAL_AS_RED_GREEN;
    else if (str_lc == "luminance_alpha") result = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA;
    else if (str_lc == "depth_stencil")   result = TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL;
    else
        return false;

    return true;
}

template<>
bool fromString<treeface::TextureImageIntDataPolicy>( const treecore::String& string, treeface::TextureImageIntDataPolicy& result )
{
    String str_lc = string.toLowerCase();

    if      (str_lc == "float")    result = TEXTURE_IMAGE_INT_TO_FLOAT;
    else if (str_lc == "signed")   result = TEXTURE_IMAGE_INT_AS_SIGNED;
    else if (str_lc == "unsigned") result = TEXTURE_IMAGE_INT_AS_UNSIGNED;
    else
        return false;

    return true;
}

template<>
treecore::String toString<treeface::GLBufferType>( treeface::GLBufferType arg )
{
    switch (arg)
    {
    case TFGL_BUFFER_VERTEX:   return "vertex";
    case TFGL_BUFFER_INDEX:    return "index";
    case TFGL_BUFFER_PACK:     return "pack";
    case TFGL_BUFFER_UNPACK:   return "unpack";
    case TFGL_BUFFER_READ:     return "read";
    case TFGL_BUFFER_WRITE:    return "write";
    case TFGL_BUFFER_FEEDBACK: return "feedback";
    case TFGL_BUFFER_UNIFORM:  return "uniform";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL buffer type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLBufferUsage>( treeface::GLBufferUsage arg )
{
    switch (arg)
    {
    case TFGL_BUFFER_STREAM_DRAW:  return "stream_draw";
    case TFGL_BUFFER_STREAM_READ:  return "stream_read";
    case TFGL_BUFFER_STREAM_COPY:  return "stream_copy";
    case TFGL_BUFFER_STATIC_DRAW:  return "static_draw";
    case TFGL_BUFFER_STATIC_READ:  return "static_read";
    case TFGL_BUFFER_STATIC_COPY:  return "static_copy";
    case TFGL_BUFFER_DYNAMIC_DRAW: return "dynamic_draw";
    case TFGL_BUFFER_DYNAMIC_READ: return "dynamic_read";
    case TFGL_BUFFER_DYNAMIC_COPY: return "dynamic_copy";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL buffer usage enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLImageFormat>( treeface::GLImageFormat arg )
{
    switch (arg)
    {
    case TFGL_IMAGE_FORMAT_ALPHA:           return "alpha";
    case TFGL_IMAGE_FORMAT_RGBA:            return "rgba";
    case TFGL_IMAGE_FORMAT_RGB:             return "rgb";
    case TFGL_IMAGE_FORMAT_RG:              return "rg";
    case TFGL_IMAGE_FORMAT_RED:             return "red";
    case TFGL_IMAGE_FORMAT_RGBA_INT:        return "rgba_int";
    case TFGL_IMAGE_FORMAT_RGB_INT:         return "rgb_int";
    case TFGL_IMAGE_FORMAT_RG_INT:          return "rg_int";
    case TFGL_IMAGE_FORMAT_RED_INT:         return "red_int";
    case TFGL_IMAGE_FORMAT_DEPTH:           return "depth";
    case TFGL_IMAGE_FORMAT_DEPTH_STENCIL:   return "depth_stencil";
    case TFGL_IMAGE_FORMAT_LUMINANCE:       return "luminance_";
    case TFGL_IMAGE_FORMAT_LUMINANCE_ALPHA: return "luminance_alpha";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL image format enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLInternalImageFormat>( treeface::GLInternalImageFormat arg )
{
    switch (arg)
    {
    case TFGL_INTERNAL_IMAGE_FORMAT_ALPHA:             return "alpha";
    case TFGL_INTERNAL_IMAGE_FORMAT_BLUE:              return "blue";
    case TFGL_INTERNAL_IMAGE_FORMAT_GREEN:             return "green";
    case TFGL_INTERNAL_IMAGE_FORMAT_RED:               return "red";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB:               return "rgb";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA:              return "rgba";
    case TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE_ALPHA:   return "luminance_alpha";
    case TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE:         return "luminance";
    case TFGL_INTERNAL_IMAGE_FORMAT_R8:                return "r8";
    case TFGL_INTERNAL_IMAGE_FORMAT_R8_SNORM:          return "r8_snorm";
    case TFGL_INTERNAL_IMAGE_FORMAT_R16F:              return "r16f";
    case TFGL_INTERNAL_IMAGE_FORMAT_R32F:              return "r32f";
    case TFGL_INTERNAL_IMAGE_FORMAT_R16UI:             return "r16ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_R16I:              return "r16i";
    case TFGL_INTERNAL_IMAGE_FORMAT_R32UI:             return "r32ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_R32I:              return "r32i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG8:               return "rg8";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG8_SNORM:         return "rg8_snorm";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG16F:             return "rg16f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG32F:             return "rg32f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG8UI:             return "rg8ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG8I:              return "rg8i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG16UI:            return "rg16ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG16I:             return "rg16i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG32UI:            return "rg32ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RG32I:             return "rg32i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB8:              return "rgb8";
    case TFGL_INTERNAL_IMAGE_FORMAT_SRGB8:             return "srgb8";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB565:            return "rgb565";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB8_SNORM:        return "rgb8_snorm";
    case TFGL_INTERNAL_IMAGE_FORMAT_R11F_G11F_B10F:    return "r11f_g11f_b10f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB9_E5:           return "rgb9_e5";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB16F:            return "rgb16f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB32F:            return "rgb32f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI:            return "rgb8ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB8I:             return "rgb8i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB16UI:           return "rgb16ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB16I:            return "rgb16i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB32UI:           return "rgb32ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB32I:            return "rgb32i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA8:             return "rgba8";
    case TFGL_INTERNAL_IMAGE_FORMAT_SRGB8_ALPHA8:      return "srgb8_alpha8";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA8_SNORM:       return "rgba8_snorm";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB5_A1:           return "rgb5_a1";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA4:             return "rgba4";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2:          return "rgb10_a2";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA16F:           return "rgba16f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA32F:           return "rgba32f";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI:           return "rgba8ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I:            return "rgba8i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2UI:        return "rgb10_a2ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA16UI:          return "rgba16ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA16I:           return "rgba16i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA32I:           return "rgba32i";
    case TFGL_INTERNAL_IMAGE_FORMAT_RGBA32UI:          return "rgba32ui";
    case TFGL_INTERNAL_IMAGE_FORMAT_DEPTH16:           return "depth16";
    case TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24:           return "depth24";
    case TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F:          return "depth32f";
    case TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24_STENCIL8:  return "depth24_stencil8";
    case TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F_STENCIL8: return "depth32f_stencil8";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL internal image format enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLImageDataType>( treeface::GLImageDataType arg )
{
    switch (arg)
    {
    case TFGL_IMAGE_DATA_BYTE: return "byte";
    case TFGL_IMAGE_DATA_UNSIGNED_BYTE: return "unsigned_byte";
    case TFGL_IMAGE_DATA_SHORT: return "short";
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT: return "unsigned_short";
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_6_5: return "5_6_5";
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_4_4_4_4: return "4_4_4_4";
    case TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_5_5_1: return "5_5_5_1";
    case TFGL_IMAGE_DATA_INT: return "int";
    case TFGL_IMAGE_DATA_UNSIGNED_INT: return "unsigned_int";
    case TFGL_IMAGE_DATA_UNSIGNED_INT_10F_11F_11F_REV: return "10f_11f_11f_rev";
    case TFGL_IMAGE_DATA_UNSIGNED_INT_5_9_9_9_REV: return "5_9_9_9_rev";
    case TFGL_IMAGE_DATA_UNSIGNED_INT_2_10_10_10_REV: return "2_10_10_10_rev";
    case TFGL_IMAGE_DATA_UNSIGNED_INT_24_8: return "24_8";
    case TFGL_IMAGE_DATA_HALF_FLOAT: return "half_float";
    case TFGL_IMAGE_DATA_FLOAT: return "float";
    case TFGL_IMAGE_DATA_FLOAT_32_UNSIGNED_INT_24_8_REV: return "f32_24_8";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL image data type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLPrimitive>( treeface::GLPrimitive arg )
{
    switch (arg)
    {
    case TFGL_PRIMITIVE_POINTS:         return "points";
    case TFGL_PRIMITIVE_LINES:          return "lines";
    case TFGL_PRIMITIVE_LINE_STRIP:     return "line_strip";
    case TFGL_PRIMITIVE_LINE_LOOP:      return "line_loop";
    case TFGL_PRIMITIVE_TRIANGLES:      return "triangles";
    case TFGL_PRIMITIVE_TRIANGLE_STRIP: return "triangle_strip";
    case TFGL_PRIMITIVE_TRIANGLE_FAN:   return "triangle_fan";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL primitive enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLTextureFilter>( treeface::GLTextureFilter arg )
{
    switch (arg)
    {
    case TFGL_TEXTURE_LINEAR:                 return "linear";
    case TFGL_TEXTURE_LINEAR_MIPMAP_LINEAR:   return "linear_mipmap_linear";
    case TFGL_TEXTURE_LINEAR_MIPMAP_NEAREST:  return "linear_mipmap_nearest";
    case TFGL_TEXTURE_NEAREST:                return "nearest";
    case TFGL_TEXTURE_NEAREST_MIPMAP_LINEAR:  return "nearest_mipmap_linear";
    case TFGL_TEXTURE_NEAREST_MIPMAP_NEAREST: return "nearest mipmap_nearest";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL texture filter enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLTextureType>( treeface::GLTextureType arg )
{
    switch (arg)
    {
    case TFGL_TEXTURE_2D: return "2d";
    case TFGL_TEXTURE_2D_ARRAY: return "2d_array";
    case TFGL_TEXTURE_3D: return "3d";
    case TFGL_TEXTURE_CUBE: return "cube";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL texture type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLTextureWrap>( treeface::GLTextureWrap arg )
{
    switch (arg)
    {
    case TFGL_TEXTURE_CLAMP_TO_EDGE:   return "clamp_to_edge";
    case TFGL_TEXTURE_MIRRORED_REPEAT: return "mirrored_repeat";
    case TFGL_TEXTURE_REPEAT:          return "repeat";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL texture wrap enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::GLType>( treeface::GLType arg )
{
    switch (arg)
    {
    case TFGL_TYPE_BYTE:                          return "byte";
    case TFGL_TYPE_UNSIGNED_BYTE:                 return "unsigned_byte";
    case TFGL_TYPE_FLOAT:                         return "float";
    case TFGL_TYPE_FLOAT_VEC2:                    return "float_vec2";
    case TFGL_TYPE_FLOAT_VEC3:                    return "float_vec3";
    case TFGL_TYPE_FLOAT_VEC4:                    return "float_vec4";
    case TFGL_TYPE_INT:                           return "int";
    case TFGL_TYPE_INT_VEC2:                      return "int_vec2";
    case TFGL_TYPE_INT_VEC3:                      return "int_vec3";
    case TFGL_TYPE_INT_VEC4:                      return "int_vec4";
    case TFGL_TYPE_UNSIGNED_INT:                  return "unsigned_int";
    case TFGL_TYPE_UNSIGNED_INT_VEC2:             return "unsigned_int_vec2";
    case TFGL_TYPE_UNSIGNED_INT_VEC3:             return "unsigned_int_vec3";
    case TFGL_TYPE_UNSIGNED_INT_VEC4:             return "unsigned_int_vec4";
    case TFGL_TYPE_BOOL:                          return "bool";
    case TFGL_TYPE_BOOL_VEC2:                     return "bool_vec2";
    case TFGL_TYPE_BOOL_VEC3:                     return "bool_vec3";
    case TFGL_TYPE_BOOL_VEC4:                     return "bool_vec4";
    case TFGL_TYPE_FLOAT_MAT2:                    return "float_mat2";
    case TFGL_TYPE_FLOAT_MAT3:                    return "float_mat3";
    case TFGL_TYPE_FLOAT_MAT4:                    return "float_mat4";
    case TFGL_TYPE_FLOAT_MAT2x3:                  return "float_mat2x3";
    case TFGL_TYPE_FLOAT_MAT2x4:                  return "float_mat2x4";
    case TFGL_TYPE_FLOAT_MAT3x2:                  return "float_mat3x2";
    case TFGL_TYPE_FLOAT_MAT3x4:                  return "float_mat3x4";
    case TFGL_TYPE_FLOAT_MAT4x2:                  return "float_mat4x2";
    case TFGL_TYPE_FLOAT_MAT4x3:                  return "float_mat4x3";
    case TFGL_TYPE_SAMPLER_2D:                    return "sampler_2d";
    case TFGL_TYPE_SAMPLER_3D:                    return "sampler_3d";
    case TFGL_TYPE_SAMPLER_CUBE:                  return "sampler_cube";
    case TFGL_TYPE_SAMPLER_2D_SHADOW:             return "sampler_2d_shadow";
    case TFGL_TYPE_SAMPLER_2D_ARRAY:              return "sampler_2d_array";
    case TFGL_TYPE_SAMPLER_2D_ARRAY_SHADOW:       return "sampler_2d_array_shadow";
    case TFGL_TYPE_SAMPLER_CUBE_SHADOW:           return "sampler_cube_shadow";
    case TFGL_TYPE_INT_SAMPLER_2D:                return "int_sampler_2d";
    case TFGL_TYPE_INT_SAMPLER_3D:                return "int_sampler_3d";
    case TFGL_TYPE_INT_SAMPLER_CUBE:              return "int_sampler_cube";
    case TFGL_TYPE_INT_SAMPLER_2D_ARRAY:          return "int_sampler_2d_array";
    case TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D:       return "unsigned_int_sampler_2d";
    case TFGL_TYPE_UNSIGNED_INT_SAMPLER_3D:       return "unsigned_int_sampler_3d";
    case TFGL_TYPE_UNSIGNED_INT_SAMPLER_CUBE:     return "unsigned_int_sampler_cube";
    case TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "unsigned_int_sampler_2d_array";
    default:
        throw std::invalid_argument( ( "invalid treeface OpenGL type enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::TextureImageSoloChannelPolicy>( treeface::TextureImageSoloChannelPolicy arg )
{
    switch (arg)
    {
    case TEXTURE_IMAGE_SOLO_AS_RED:       return "red";
    case TEXTURE_IMAGE_SOLO_AS_LUMINANCE: return "luminance";
    case TEXTURE_IMAGE_SOLO_AS_ALPHA:     return "alpha";
    case TEXTURE_IMAGE_SOLO_AS_DEPTH:     return "depth";
    default:
        throw std::invalid_argument( ( "invalid texture image 1-channel policy enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::TextureImageDualChannelPolicy>( treeface::TextureImageDualChannelPolicy arg )
{
    switch (arg)
    {
    case TEXTURE_IMAGE_DUAL_AS_RED_GREEN:       return "red_green";
    case TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA: return "luminance_alpha";
    case TEXTURE_IMAGE_DUAL_AS_DEPTH_STENCIL:   return "depth_stencil";
    default:
        throw std::invalid_argument( ( "invalid texture image 1-channel policy enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

template<>
treecore::String toString<treeface::TextureImageIntDataPolicy>( treeface::TextureImageIntDataPolicy arg )
{
    switch (arg)
    {
    case TEXTURE_IMAGE_INT_TO_FLOAT:    return "float";
    case TEXTURE_IMAGE_INT_AS_SIGNED:   return "signed";
    case TEXTURE_IMAGE_INT_AS_UNSIGNED: return "unsigned";
    default:
        throw std::invalid_argument( ( "invalid texture image 1-channel policy enum: " + String( int(arg) ) ).toRawUTF8() );
    }
}

} // namespace treeface
