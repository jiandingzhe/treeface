#include "TestFramework.h"

#include "treeface/packagemanager.h"

#include "resources.h"

#include <treecore/File.h>
#include <treecore/InputStream.h>
#include <treecore/ZipFile.h>

using namespace treecore;
using namespace treeface;

void TestFramework::content()
{
    PackageManager::getInstance()->add_package(resources::resources1_zip, resources::resources1_zipSize, PackageManager::USE_OLDER);

    char buffer[512];
    OK(PackageManager::getInstance()->has_resource("foo"));
    {
        InputStream* stream = PackageManager::getInstance()->get_item_stream("foo");
        OK(stream);
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'f');
        IS(buffer[1], 'o');
        IS(buffer[2], 'o');
        IS(buffer[3], '\n');
    }

    OK(PackageManager::getInstance()->has_resource("bar"));
    {
        InputStream* stream = PackageManager::getInstance()->get_item_stream("bar");
        OK(stream);
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'r');
        IS(buffer[3], '\n');
    }

    PackageManager::getInstance()->add_package(resources::resources2_zip, resources::resources2_zipSize, PackageManager::USE_OLDER);
    OK(PackageManager::getInstance()->has_resource("foo"));
    {
        InputStream* stream = PackageManager::getInstance()->get_item_stream("foo");
        OK(stream);
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'f');
        IS(buffer[1], 'o');
        IS(buffer[2], 'o');
        IS(buffer[3], '\n');
    }

    OK(PackageManager::getInstance()->has_resource("bar"));
    {
        InputStream* stream = PackageManager::getInstance()->get_item_stream("bar");
        OK(stream);
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'r');
        IS(buffer[3], '\n');
    }

    OK(PackageManager::getInstance()->has_resource("baz"));
    {
        InputStream* stream = PackageManager::getInstance()->get_item_stream("baz");
        OK(stream);
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'z');
        IS(buffer[3], '\n');
    }


}
