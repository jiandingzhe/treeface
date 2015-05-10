#include "treeface/scene/scenenode.h"
#include "treeface/scene/visualitem.h"

#include "treeface/private/node_private.h"

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

SceneNode::SceneNode(): m_impl(new Impl())
{
}

SceneNode::~SceneNode()
{
    if (m_impl)
        delete m_impl;
}

const Mat4f& SceneNode::get_transform() NOEXCEPT
{
    return m_impl->trans;
}

const Mat4f& SceneNode::get_global_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_global;
}

const Mat4f& SceneNode::get_normal_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_normal;
}

const Mat4f& SceneNode::get_global_normal_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_normal_global;
}

void SceneNode::set_transform(const Mat4f& value) NOEXCEPT
{
    m_impl->trans_dirty = true;
    m_impl->trans = value;
}

bool SceneNode::add_visual_item(VisualItem* obj)
{
    return m_impl->visual_items.add(obj);
}

bool SceneNode::has_visual_item(VisualItem* obj) const NOEXCEPT
{
    return m_impl->visual_items.contains(obj);
}

bool SceneNode::remove_visual_item(VisualItem* obj)
{
    if (m_impl->visual_items.contains(obj))
    {
        m_impl->visual_items.removeValue(obj);
        return true;
    }
    else
    {
        return false;
    }
}

int SceneNode::get_num_visual_items() const NOEXCEPT
{
    return m_impl->visual_items.size();
}

VisualItem* SceneNode::get_visual_item_at(int idx) NOEXCEPT
{
    return m_impl->visual_items.getReference(idx);
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

bool SceneNode::has_child(SceneNode* child) const NOEXCEPT
{
    return m_impl->child_nodes.contains(child);
}

bool SceneNode::remove_child(SceneNode* child) NOEXCEPT
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

int SceneNode::get_num_children() const NOEXCEPT
{
    return m_impl->child_nodes.size();
}

SceneNode* SceneNode::get_child_at(int idx) NOEXCEPT
{
    return m_impl->child_nodes.getReference(idx);
}

SceneNode* SceneNode::get_parent() NOEXCEPT
{
    return m_impl->parent;
}

TREEFACE_NAMESPACE_END
