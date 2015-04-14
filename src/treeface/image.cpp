#include "treeface/image.h"

#include <treejuce/IntTypes.h>
#include <treejuce/Result.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

Result convert_fi_image_info_to_gl(FIBITMAP* fi_img, ImageRef& info)
{
    FREE_IMAGE_TYPE fi_type = FreeImage_GetImageType(fi_img);
    uint32 bits_per_pixel = FreeImage_GetBPP(fi_img);
    FREE_IMAGE_COLOR_TYPE fi_color_type = FreeImage_GetColorType(fi_img);

    switch (fi_type)
    {
    case FIT_BITMAP:
    case FIT_RGB16:
    case FIT_RGBA16:
    case FIT_RGBF:
    case FIT_RGBAF:
    default:
        return Result::fail("unknown FreeImage type: "+String(int(fi_type)));
    }
}

Image::Image()
{}

Image::Image(FIBITMAP* fi_img)
    : m_fi_img(fi_img)
{
}


TREEFACE_NAMESPACE_END
