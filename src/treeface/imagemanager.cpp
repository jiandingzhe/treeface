#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/packagemanager.h"
#include "treeface/stringcast.h"

#include <FreeImage.h>

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/ScopedPointer.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
template <>
struct ContainerDeletePolicy<uint8>
{
    static void destroy (uint8* ptr)
    {
        free(ptr);
    }
};
TREEFACE_JUCE_NAMESPACE_END


using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

juce_ImplementSingleton(ImageManager);

struct ImageManager::Impl
{
    HashMap<String, Holder<Image> > items;
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
    else
    {
        ArrayRef<uint8> data = PackageManager::getInstance()->get_item_data(name);
        ScopedPointer<uint8> data_holder(data.get_data());
        if (!data.get_data())
        {
            *img = nullptr;
            return Result::fail("failed to get image data from package manager using name \""+name+"\"");
        }

        FIMEMORY* mem_stream = FreeImage_OpenMemory(data.get_data(), data.size());
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

TREEFACE_NAMESPACE_END
