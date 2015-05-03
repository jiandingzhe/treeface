#include "treeface/scene/node.h"
#include "treeface/scene/visualitem.h"

#include "treeface/misc/propertyvalidator.h"

#include <treejuce/CriticalSection.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/Holder.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/SortedSet.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct Node::Impl
{
    Mat4f trans;

    Mat4f trans_inv;
    Mat4f trans_global;
    Mat4f trans_global_inv;
    Mat4f trans_normal;
    Mat4f trans_normal_global;

    bool trans_dirty = true;
    bool global_dirty = true;
    SortedSet<Holder<Node> > child_nodes;
    Node* parent = nullptr;

    SortedSet<Holder<VisualItem> > visual_items;

    void update_trans_descendent();
    void update_global_descendent();
};

void Node::Impl::update_trans_descendent()
{
    trans_inv = trans;
    trans_inv.inverse();

    trans_normal = trans.get_normal_matrix();

    trans_dirty = false;

    update_global_descendent();
}

void Node::Impl::update_global_descendent()
{
    if (parent)
    {
        trans_global = parent->get_global_transform() * trans;
        trans_normal_global = parent->get_global_normal_transform() * trans_normal;
    }
    else
    {
        trans_global = trans;
        trans_normal_global = trans_normal;
    }

    trans_global_inv = trans_global;
    trans_global_inv.inverse();

    global_dirty = false;
}


Node::Node(): m_impl(new Impl())
{
}

Node::~Node()
{
    if (m_impl)
        delete m_impl;
}

#define KEY_TRANS "transform"
#define KEY_CHILD "children"
#define KEY_VISUAL "visual_items"

Result _validate_properties_(const NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_TRANS, PropertyValidator::ITEM_ARRAY, false);
        validator->add_item(KEY_CHILD, PropertyValidator::ITEM_ARRAY, false);
        validator->add_item(KEY_VISUAL, PropertyValidator::ITEM_ARRAY, false);
    }
    return validator->validate(kv);
}

treejuce::Result Node::build(const treejuce::var& node_var)
{
    if (!node_var.isObject())
        return Result::fail("root node for Node is not KV");

    const NamedValueSet& node_kv = node_var.getDynamicObject()->getProperties();
    {
        Result valid_re = _validate_properties_(node_kv);
        if (!valid_re)
            return valid_re;
    }

    //
    // node transform
    //
    if (node_kv.contains(KEY_TRANS))
    {
        const Array<var>* trans_array = node_kv[KEY_TRANS].getArray();
        if (trans_array->size() != 16)
            return Result::fail("transform is not an array of 16 numbers");

        Mat4f mat(float(trans_array->getReference(0)), float(trans_array->getReference(1)), float(trans_array->getReference(2)), float(trans_array->getReference(3)),
                  float(trans_array->getReference(4)), float(trans_array->getReference(5)), float(trans_array->getReference(6)), float(trans_array->getReference(7)),
                  float(trans_array->getReference(8)), float(trans_array->getReference(9)), float(trans_array->getReference(10)), float(trans_array->getReference(11)),
                  float(trans_array->getReference(12)), float(trans_array->getReference(13)), float(trans_array->getReference(14)), float(trans_array->getReference(15)));
        m_impl->trans = mat;
        m_impl->trans_dirty = true;
    }

    //
    // visual items
    //
    if (node_kv.contains(KEY_VISUAL))
    {
        const Array<var>* visual_array = node_kv[KEY_VISUAL].getArray();
        for (int i = 0; i < visual_array->size(); i++)
        {
            VisualItem* item = new VisualItem();
            Result re = item->build(visual_array->getReference(i));
            if (!re)
                return re;
            m_impl->visual_items.add(item);
        }
    }

    //
    // child nodes
    //
    if (node_kv.contains(KEY_CHILD))
    {
        const Array<var>* child_array = node_kv[KEY_CHILD].getArray();
        for (int i = 0; i < child_array->size(); i++)
        {
            Node* child = new Node();
            Result re = child->build(child_array->getReference(i));
            if (!re)
                return re;

            child->m_impl->parent = this;
            child->m_impl->global_dirty = true;
            m_impl->child_nodes.add(child);
        }
    }

    return Result::ok();
}

const Mat4f& Node::get_transform() NOEXCEPT
{
    return m_impl->trans;
}

const Mat4f& Node::get_global_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_global;
}

const Mat4f& Node::get_normal_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_normal;
}

const Mat4f& Node::get_global_normal_transform() NOEXCEPT
{
    if (m_impl->trans_dirty)
        m_impl->update_trans_descendent();
    else if (m_impl->global_dirty)
        m_impl->update_global_descendent();

    return m_impl->trans_normal_global;
}

void Node::set_transform(const Mat4f& value) NOEXCEPT
{
    m_impl->trans_dirty = true;
    m_impl->trans = value;
}

bool Node::add_visual_item(VisualItem* obj)
{
    return m_impl->visual_items.add(obj);
}

bool Node::has_visual_item(VisualItem* obj) const NOEXCEPT
{
    return m_impl->visual_items.contains(obj);
}

bool Node::remove_visual_item(VisualItem* obj)
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

int Node::get_num_visual_items() const NOEXCEPT
{
    return m_impl->visual_items.size();
}

VisualItem* Node::get_visual_item_at(int idx) NOEXCEPT
{
    return m_impl->visual_items.getReference(idx);
}

bool Node::add_child(Node* child)
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

bool Node::has_child(Node* child) const NOEXCEPT
{
    return m_impl->child_nodes.contains(child);
}

bool Node::remove_child(Node* child) NOEXCEPT
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

int Node::get_num_child() const NOEXCEPT
{
    return m_impl->child_nodes.size();
}

Node* Node::get_child_at(int idx) NOEXCEPT
{
    return m_impl->child_nodes.getReference(idx);
}

Node* Node::get_parent() NOEXCEPT
{
    return m_impl->parent;
}

TREEFACE_NAMESPACE_END
