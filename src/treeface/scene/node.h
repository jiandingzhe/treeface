#ifndef TREEFACE_NODE_H
#define TREEFACE_NODE_H

#include "treeface/common.h"

#include "treeface/math.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class VisualItem;

class Node: public treejuce::Object
{
public:
    Node();

    JUCE_DECLARE_NON_COPYABLE(Node);

    Node(Node&& other): m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    Node& operator = (Node&& other)
    {
        jassert(m_impl == nullptr);
        m_impl = other.m_impl;
        other.m_impl = nullptr;
    }

    virtual ~Node();

    treejuce::Result build(const treejuce::var& node_var);

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

    bool add_child(Node* child);
    bool has_child(Node* child) const NOEXCEPT;
    bool remove_child(Node* child) NOEXCEPT;
    int get_num_child() const NOEXCEPT;
    Node* get_child_at(int idx) NOEXCEPT;

    Node* get_parent() NOEXCEPT;
private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_NODE_H
