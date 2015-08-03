#ifndef TREEFACE_SCENE_ITEM_H
#define TREEFACE_SCENE_ITEM_H
#include "treeface/common.h"

#include <treecore/Object.h>

namespace treeface {

class SceneNode;

class SceneItem: public treecore::Object
{
    friend class SceneNode;

public:
    SceneItem();

    JUCE_DECLARE_NON_COPYABLE(SceneItem);
    JUCE_DECLARE_NON_MOVABLE(SceneItem);

    virtual ~SceneItem();

    inline SceneNode* get_node()
    {
        return m_node;
    }

protected:
    SceneNode* m_node = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_ITEM_H
