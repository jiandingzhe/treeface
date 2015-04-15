#include "treeface/stringcast.h"

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
    else                                              return FIF_UNKNOWN;
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
    default:          return "unknown";
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
    if      (string.compareIgnoreCase("miniswhite")) return FIC_MINISWHITE;
    else if (string.compareIgnoreCase("minisblack")) return FIC_MINISBLACK;
    else if (string.compareIgnoreCase("rgb"))        return FIC_RGB;
    else if (string.compareIgnoreCase("palette"))    return FIC_PALETTE;
    else if (string.compareIgnoreCase("rgbalpha"))   return FIC_RGBALPHA;
    else if (string.compareIgnoreCase("rgba"))       return FIC_RGBALPHA;
    else if (string.compareIgnoreCase("cmyk"))       return FIC_CMYK;
    else return FIC_RGBALPHA;
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
    default:             return "";
    }
}


TREEFACE_NAMESPACE_END
