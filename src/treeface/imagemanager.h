#ifndef TREEFACE_IMAGE_MANAGER_H
#define TREEFACE_IMAGE_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Result.h>
#include <treejuce/Singleton.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN

class Image;

class ImageManager
{
public:
    JUCE_DECLARE_NON_COPYABLE(ImageManager);
    JUCE_DECLARE_NON_MOVABLE(ImageManager);
    juce_DeclareSingleton(ImageManager, false);

    treejuce::Result get_image(const treejuce::String& name, Image** img);
    bool image_is_cached(const treejuce::String& name) const;
    bool release_image_hold(const treejuce::String& name);

protected:
    struct Impl;

    ImageManager();
    ~ImageManager();

    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_IMAGE_MANAGER_H
