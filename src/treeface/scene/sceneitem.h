#ifndef TREEFACE_SCENE_ITEM_H
#define TREEFACE_SCENE_ITEM_H
#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_NAMESPACE_BEGIN

class SceneNode;

class SceneItem: public treejuce::Object
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

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_ITEM_H
