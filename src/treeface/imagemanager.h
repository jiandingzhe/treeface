#ifndef TREEFACE_IMAGE_MANAGER_H
#define TREEFACE_IMAGE_MANAGER_H

#include "treeface/common.h"

#include <treecore/Result.h>
#include <treecore/RefCountObject.h>
#include <treecore/RefCountSingleton.h>
#include <treecore/Identifier.h>

namespace treeface {

class Image;

class ImageManager: public treecore::RefCountObject, public treecore::RefCountSingleton<ImageManager>
{
    friend class treecore::RefCountSingleton<ImageManager>;

public:
    TREECORE_DECLARE_NON_COPYABLE( ImageManager );
    TREECORE_DECLARE_NON_MOVABLE( ImageManager );

    Image* get_image( const treecore::Identifier& name );
    bool   image_is_cached( const treecore::Identifier& name ) const;
    bool   release_image_hold( const treecore::Identifier& name );

protected:
    struct Impl;

    ImageManager();
    ~ImageManager();

    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_IMAGE_MANAGER_H
