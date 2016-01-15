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

Image* ImageManager::get_image(const String& name)
{
    if (m_impl->items.contains(name))
    {
        return m_impl->items[name];
    }

    MemoryBlock data;
    if (!PackageManager::getInstance()->get_item_data(name, data, false))
        return nullptr;

    Image* img = new Image(data);
    m_impl->items.set(name, img);

    return img;
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
