#ifndef TREEFACE_SCENE_NODE_H
#define TREEFACE_SCENE_NODE_H

#include "treeface/common.h"

#include "treeface/math.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN
class SceneNodeManager;
class VisualItem;

class SceneNode: public treejuce::Object
{
    friend class SceneNodeManager;

public:
    SceneNode();

    JUCE_DECLARE_NON_COPYABLE(SceneNode);

    SceneNode(SceneNode&& other): m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    SceneNode& operator = (SceneNode&& other)
    {
        jassert(m_impl == nullptr);
        m_impl = other.m_impl;
        other.m_impl = nullptr;
        return *this;
    }

    virtual ~SceneNode();

    const Mat4f& get_transform() NOEXCEPT;
    const Mat4f& get_global_transform() NOEXCEPT;
    const Mat4f& get_normal_transform() NOEXCEPT;
    const Mat4f& get_global_normal_transform() NOEXCEPT;

    void set_transform(const Mat4f& value) NOEXCEPT;

    bool add_visual_item(VisualItem* obj);
    bool has_visual_item(VisualItem* obj) const NOEXCEPT;
    bool remove_visual_item(VisualItem* obj);
    int get_num_visual_items() const NOEXCEPT;
    VisualItem* get_visual_item_at(int idx) NOEXCEPT;

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

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_NODE_H
