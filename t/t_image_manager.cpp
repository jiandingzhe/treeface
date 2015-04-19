#include "treeface/image.h"
#include "treeface/gl/imageref.h"
#include "treeface/packagemanager.h"
#include "treeface/imagemanager.h"

#include <treejuce/File.h>

#include "TestFramework.h"

using namespace treeface;
using namespace treejuce;

void TestFramework::content()
{
    PackageManager* pkg_mgr = PackageManager::getInstance();
    pkg_mgr->add_package(File("images.zip"), PackageManager::KEEP_EXISTING);

    OK(pkg_mgr->has_resource("earth.png"));
    OK(pkg_mgr->has_resource("moon.jpg"));
    OK(pkg_mgr->has_resource("moon.pfm"));
    OK(pkg_mgr->has_resource("moonbump.jpg"));
    OK(pkg_mgr->has_resource("moonbump.pfm"));

    ImageManager* img_mgr = ImageManager::getInstance();
    OK(img_mgr);

    {
        Image* img = nullptr;;
        OK(img_mgr->get_image("earth.png", &img));
        OK(img_mgr->image_is_cached("earth.png"));

        ImageRef ref_img = img->get_image_ref();

        IS(ref_img.format, GL_RGBA);
        IS(ref_img.type, GL_UNSIGNED_BYTE);
    }

    {
        Image* img = nullptr;
        OK(img_mgr->get_image("moon.jpg", &img));
        OK(img_mgr->image_is_cached("moon.jpg"));

        ImageRef ref_img = img->get_image_ref();

        IS(ref_img.format, GL_RGB);
        IS(ref_img.type, GL_UNSIGNED_BYTE);
    }

    {
        Image* img = nullptr;
        OK(img_mgr->get_image("moon.pfm", &img));
        OK(img_mgr->image_is_cached("moon.pfm"));

        ImageRef ref_img = img->get_image_ref();

        IS(ref_img.format, GL_RGB);
        IS(ref_img.type, GL_FLOAT);
    }

    {
        Image* img = nullptr;
        OK(img_mgr->get_image("moonbump.jpg", &img));
        OK(img_mgr->image_is_cached("moonbump.jpg"));

        ImageRef ref_img = img->get_image_ref();

        IS(ref_img.format, GL_ALPHA);
        IS(ref_img.type, GL_UNSIGNED_BYTE);
    }

    {
        Image* img = nullptr;
        OK(img_mgr->get_image("moonbump.pfm", &img));
        OK(img_mgr->image_is_cached("moonbump.pfm"));

        ImageRef ref_img = img->get_image_ref();

        IS(ref_img.format, GL_ALPHA);
        IS(ref_img.type, GL_FLOAT);
    }
}
