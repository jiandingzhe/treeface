#include "TestFramework.h"

#include "treeface/resourcemanager.h"

#include "resources.h"

#include <treejuce/File.h>
#include <treejuce/InputStream.h>
#include <treejuce/ZipFile.h>

using namespace treejuce;
using namespace treeface;

void TestFramework::content()
{
    ResourceManager::getInstance()->add_package(resources::resources1_zip, resources::resources1_zipSize, ResourceManager::OVERWRITE);

    char buffer[512];
    OK(ResourceManager::getInstance()->has_resource("foo"));
    {
        InputStream* stream = ResourceManager::getInstance()->get_resource("foo");
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'f');
        IS(buffer[1], 'o');
        IS(buffer[2], 'o');
        IS(buffer[3], '\n');
    }

    OK(ResourceManager::getInstance()->has_resource("bar"));
    {
        InputStream* stream = ResourceManager::getInstance()->get_resource("bar");
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'r');
        IS(buffer[3], '\n');
    }

    ResourceManager::getInstance()->add_package(resources::resources2_zip, resources::resources2_zipSize, ResourceManager::OVERWRITE);
    OK(ResourceManager::getInstance()->has_resource("foo"));
    {
        InputStream* stream = ResourceManager::getInstance()->get_resource("foo");
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'f');
        IS(buffer[1], 'o');
        IS(buffer[2], 'o');
        IS(buffer[3], '\n');
    }

    OK(ResourceManager::getInstance()->has_resource("bar"));
    {
        InputStream* stream = ResourceManager::getInstance()->get_resource("bar");
        int64 size = stream->getTotalLength();
        IS(size, 8);
        IS(stream->read(buffer, 512), 8);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'r');
        IS(buffer[3], '\n');
        IS(buffer[4], 'b');
        IS(buffer[5], 'a');
        IS(buffer[6], 'r');
        IS(buffer[7], '\n');
    }

    OK(ResourceManager::getInstance()->has_resource("baz"));
    {
        InputStream* stream = ResourceManager::getInstance()->get_resource("baz");
        int64 size = stream->getTotalLength();
        IS(size, 4);
        IS(stream->read(buffer, 512), 4);
        IS(buffer[0], 'b');
        IS(buffer[1], 'a');
        IS(buffer[2], 'z');
        IS(buffer[3], '\n');
    }


}
