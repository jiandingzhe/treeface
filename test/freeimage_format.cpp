#include <FreeImage.h>

#include <cstdio>
#include <cstdlib>

#include "treeface/misc/StringCast.h"

using namespace std;
using namespace treecore;
using namespace treeface;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("usage: ./freeimage_format image_file\n");
        exit(1);
    }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(argv[1]);
    if (format == FIF_UNKNOWN)
    {
        printf("failed to determine image format from %s\n", argv[1]);
        exit(1);
    }
    printf("format is %s\n", toString(format).toRawUTF8());

    if (!FreeImage_IsPluginEnabled(format))
    {
        printf("format plugin %d is not enabled\n", format);
        exit(1);
    }

    FIBITMAP* img = FreeImage_Load(format, argv[1]);
    if (!img)
    {
        printf("failed to load image %s\n", argv[1]);
        exit(1);
    }

    FREE_IMAGE_TYPE img_type = FreeImage_GetImageType(img);
    printf("image type: %s\n", toString(img_type).toRawUTF8());

    unsigned int n_color = FreeImage_GetColorsUsed(img);
    printf("palette: %u\n", n_color);

    unsigned int bit_per_pixel = FreeImage_GetBPP(img);
    printf("bits per pixel: %u\n", bit_per_pixel);

    FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType(img);
    printf("color type: %s\n", toString(color_type).toRawUTF8());
}
