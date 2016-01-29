#include "treeface/graphics/Image.h"
#include "treeface/gl/ImageRef.h"
#include "treeface/base/PackageManager.h"
#include "treeface/graphics/ImageManager.h"

#include <treecore/File.h>

#include "TestFramework.h"

using namespace treeface;
using namespace treecore;

void TestFramework::content()
{
    PackageManager* pkg_mgr = PackageManager::getInstance();
    pkg_mgr->add_package( File( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );

    OK( pkg_mgr->has_resource( "earth.png" ) );
    OK( pkg_mgr->has_resource( "moon.jpg" ) );
    OK( pkg_mgr->has_resource( "moon.pfm" ) );
    OK( pkg_mgr->has_resource( "moonbump.jpg" ) );
    OK( pkg_mgr->has_resource( "moonbump.pfm" ) );

    ImageManager* img_mgr = ImageManager::getInstance();
    OK( img_mgr );

    // 4 channel ubyte data
    {
        Image* img = img_mgr->get_image( "earth.png" );
        OK( img != nullptr );
        OK( img_mgr->image_is_cached( "earth.png" ) );
        IS( img->get_num_channel(), 4 );
        IS( img->get_data_type(),   TFGL_IMAGE_DATA_UNSIGNED_BYTE );
        IS( img->get_width(),       2048 );
        IS( img->get_height(),      1024 );

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d();
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGBA );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGBA8 );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_quad( TEXTURE_IMAGE_INT_AS_SIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGBA_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_quad( TEXTURE_IMAGE_INT_AS_UNSIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGBA_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI );
        }
    }

    // 3 channel ubyte data
    {
        Image* img = img_mgr->get_image( "moon.jpg" );
        OK( img != nullptr );
        OK( img_mgr->image_is_cached( "moon.jpg" ) );
        IS( img->get_num_channel(), 3 );
        IS( img->get_data_type(),   TFGL_IMAGE_DATA_UNSIGNED_BYTE );
        IS( img->get_width(),       2048 );
        IS( img->get_height(),      1024 );

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d();
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGB );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGB8 );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_triple( TEXTURE_IMAGE_INT_AS_SIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGB_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGB8I );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_triple( TEXTURE_IMAGE_INT_AS_UNSIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGB_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI );
        }
    }

    // 3 channel float
    {
        Image* img = img_mgr->get_image( "moon.pfm" );
        OK( img != nullptr );
        OK( img_mgr->image_is_cached( "moon.pfm" ) );

        IS( img->get_num_channel(), 3 );
        IS( img->get_data_type(),   TFGL_IMAGE_DATA_FLOAT );
        IS( img->get_width(),       2048 );
        IS( img->get_height(),      1024 );

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d();
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RGB );
            IS( ref_img.type,            TFGL_IMAGE_DATA_FLOAT );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_RGB32F );
        }
    }

    // single channel ubyte data
    {
        Image* img = img_mgr->get_image( "moonbump.jpg" );
        OK( img != nullptr );
        OK( img_mgr->image_is_cached( "moonbump.jpg" ) );
        IS( img->get_num_channel(), 1 );
        IS( img->get_data_type(),   TFGL_IMAGE_DATA_UNSIGNED_BYTE );
        IS( img->get_width(),       2048 );
        IS( img->get_height(),      1024 );

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_RED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RED );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_R8 );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_RED, TEXTURE_IMAGE_INT_AS_SIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RED_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_R8I );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_RED, TEXTURE_IMAGE_INT_AS_UNSIGNED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RED_INT );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_R8UI );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_LUMINANCE );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_LUMINANCE );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_ALPHA );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_ALPHA );
            IS( ref_img.type,            TFGL_IMAGE_DATA_UNSIGNED_BYTE );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_ALPHA );
        }
    }

    // single channel float data
    {
        Image* img = img_mgr->get_image( "moonbump.pfm" );
        OK( img != nullptr );
        OK( img_mgr->image_is_cached( "moonbump.pfm" ) );
        IS( img->get_num_channel(), 1 );
        IS( img->get_data_type(),   TFGL_IMAGE_DATA_FLOAT );
        IS( img->get_width(),       2048 );
        IS( img->get_height(),      1024 );

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_RED );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_RED );
            IS( ref_img.type,            TFGL_IMAGE_DATA_FLOAT );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_R32F );
        }

        {
            TextureCompatibleImageRef ref_img = img->get_texture_compatible_2d_solo( TEXTURE_IMAGE_SOLO_AS_DEPTH );
            IS( ref_img.format,          TFGL_IMAGE_FORMAT_DEPTH );
            IS( ref_img.type,            TFGL_IMAGE_DATA_FLOAT );
            IS( ref_img.internal_format, TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F );
        }
    }

    // delete managers, which will release object hold
    // otherwise valgrind will report many possible losts
    PackageManager::releaseInstance();
    ImageManager::releaseInstance();
}
