#ifndef TREEFACE_IMAGE_MANAGER_H
#define TREEFACE_IMAGE_MANAGER_H

#include "treeface/common.h"

#include <treecore/Result.h>
#include <treecore/Singleton.h>
#include <treecore/String.h>

namespace treeface {

class Image;

class ImageManager
{
public:
    JUCE_DECLARE_NON_COPYABLE(ImageManager);
    JUCE_DECLARE_NON_MOVABLE(ImageManager);
    juce_DeclareSingleton(ImageManager, false);

    treecore::Result get_image(const treecore::String& name, Image** img);
    bool image_is_cached(const treecore::String& name) const;
    bool release_image_hold(const treecore::String& name);

protected:
    struct Impl;

    ImageManager();
    ~ImageManager();

    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_IMAGE_MANAGER_H
