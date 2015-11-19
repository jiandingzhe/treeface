#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/packagemanager.h"
#include "treeface/misc/stringcast.h"

#include <FreeImage.h>

#include <treecore/HashMap.h>
#include <treecore/RefCountHolder.h>
#include <treecore/MemoryBlock.h>
#include <treecore/ScopedPointer.h>

using namespace treecore;

namespace treeface {

juce_ImplementSingleton(ImageManager);

struct ImageManager::Impl
{
    HashMap<String, RefCountHolder<Image> > items;
};

ImageManager::ImageManager(): m_impl(new Impl())
{
}

ImageManager::~ImageManager()
{
    if (m_impl)
        delete m_impl;
}

Result ImageManager::get_image(const String& name, Image** img)
{
    if (m_impl->items.contains(name))
    {
        *img = m_impl->items[name].get();
        return Result::ok();
    }

    MemoryBlock data;
    Result item_re = PackageManager::getInstance()->get_item_data(name, data);
    if (!item_re)
    {
        *img = nullptr;
        return Result::fail("failed to get image data:\n" +
                            item_re.getErrorMessage());
    }

    FIMEMORY* mem_stream = FreeImage_OpenMemory((BYTE*)data.getData(), data.getSize()-1);
    if (!mem_stream)
    {
        *img = nullptr;
        return Result::fail("failed to create FreeImage memory handle from data of \""+name+"\"");
    }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(mem_stream);
    FIBITMAP* fi_img = FreeImage_LoadFromMemory(format, mem_stream);

    FreeImage_CloseMemory(mem_stream);

    if (!fi_img)
    {
        *img =  nullptr;
        return Result::fail("failed to load FreeImage from data of \""+name+"\" (format is "+to_string(format)+")");
    }

    *img = new Image(fi_img);
    m_impl->items.set(name, *img);

    return Result::ok();
}

bool ImageManager::image_is_cached(const String& name) const
{
    return m_impl->items.contains(name);
}

bool ImageManager::release_image_hold(const String& name)
{
    if (m_impl->items.contains(name))
    {
        m_impl->items.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace treeface
