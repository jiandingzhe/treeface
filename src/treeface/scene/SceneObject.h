#ifndef TREEFACE_SCENE_ITEM_H
#define TREEFACE_SCENE_ITEM_H
#include "treeface/base/Common.h"

#include <treecore/RefCountObject.h>

namespace treeface {

class SceneNode;

class SceneObject: public treecore::RefCountObject
{
    friend class SceneNode;

public:
    SceneObject();

    TREECORE_DECLARE_NON_COPYABLE(SceneObject)
    TREECORE_DECLARE_NON_MOVABLE(SceneObject)

    virtual ~SceneObject();

    inline SceneNode* get_node()
    {
        return m_node;
    }

protected:
    SceneNode* m_node = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_ITEM_H
