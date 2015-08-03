#ifndef TREEFACE_SCENE_NODE_H
#define TREEFACE_SCENE_NODE_H

#include "treeface/common.h"

#include "treeface/math.h"

#include <treecore/ArrayRef.h>
#include <treecore/Object.h>

namespace treecore {
class Result;
class var;
} // namespace treecore

namespace treeface {
class SceneNodeManager;
class SceneItem;

class SceneNode: public treecore::Object
{
    friend class SceneNodeManager;

public:
    SceneNode();

    JUCE_DECLARE_NON_COPYABLE(SceneNode);
    JUCE_DECLARE_NON_MOVABLE(SceneNode);

    virtual ~SceneNode();

    const Mat4f& get_transform() NOEXCEPT;
    const Mat4f& get_global_transform() NOEXCEPT;
    const Mat4f& get_transform_inv() NOEXCEPT;
    const Mat4f& get_global_transform_inv() NOEXCEPT;

    void set_transform(const Mat4f& value) NOEXCEPT;

    bool add_item(SceneItem* obj);
    bool has_item(SceneItem* obj) const NOEXCEPT;
    bool remove_item(SceneItem* obj);
    int get_num_items() const NOEXCEPT;
    SceneItem* get_item_at(int idx) NOEXCEPT;

    bool add_child(SceneNode* child);
    bool has_child(SceneNode* child) const NOEXCEPT;
    bool remove_child(SceneNode* child) NOEXCEPT;
    int get_num_children() const NOEXCEPT;
    SceneNode* get_child_at(int idx) NOEXCEPT;

    SceneNode* get_parent() NOEXCEPT;
private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_NODE_H
