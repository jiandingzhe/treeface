#include "treeface/scene/scenenode.h"
#include "treeface/scene/visualobject.h"

#include "treeface/scene/guts/node_guts.h"

using namespace treecore;

namespace treeface {

SceneNode::SceneNode(): m_impl(new Impl())
{
}

SceneNode::~SceneNode()
{
    if (m_impl)
        delete m_impl;
}

const Mat4f& SceneNode::get_transform() noexcept
{
    return m_impl->trans;
}

const Mat4f& SceneNode::get_global_transform() noexcept
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_global;
}

const Mat4f& SceneNode::get_transform_inv() noexcept
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    return m_impl->trans_inv;
}

const Mat4f& SceneNode::get_global_transform_inv() noexcept
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_global_inv;
}

void SceneNode::set_transform(const Mat4f& value) noexcept
{
    m_impl->trans_dirty = true;
    m_impl->trans = value;
}

bool SceneNode::add_item(SceneItem* obj)
{
    if (m_impl->items.add(obj))
    {
        obj->m_node = this;
        return true;
    }
    else
    {
        return false;
    }
}

bool SceneNode::has_item(SceneItem* obj) const noexcept
{
    return m_impl->items.contains(obj);
}

bool SceneNode::remove_item(SceneItem* obj)
{
    if (m_impl->items.contains(obj))
    {
        m_impl->items.removeValue(obj);
        obj->m_node = nullptr;
        return true;
    }
    else
    {
        return false;
    }
}

int SceneNode::get_num_items() const noexcept
{
    return m_impl->items.size();
}

SceneItem* SceneNode::get_item_at(int idx) noexcept
{
    return m_impl->items[idx].get();
}

bool SceneNode::add_child(SceneNode* child)
{
    if (child->m_impl->parent)
        return false;

    bool add_re = m_impl->child_nodes.add(child);
    if (!add_re)
        return false;

    child->m_impl->parent = this;
    child->m_impl->global_dirty = true;

    return true;
}

bool SceneNode::has_child(SceneNode* child) const noexcept
{
    return m_impl->child_nodes.contains(child);
}

bool SceneNode::remove_child(SceneNode* child) noexcept
{
    if (m_impl->child_nodes.contains(child))
    {
        m_impl->child_nodes.removeValue(child);
        child->m_impl->parent = nullptr;
        child->m_impl->global_dirty = true;
        return true;
    }
    else
    {
        return false;
    }
}

int SceneNode::get_num_children() const noexcept
{
    return m_impl->child_nodes.size();
}

SceneNode* SceneNode::get_child_at(int idx) noexcept
{
    return m_impl->child_nodes[idx];
}

SceneNode* SceneNode::get_parent() noexcept
{
    return m_impl->parent;
}

} // namespace treeface
