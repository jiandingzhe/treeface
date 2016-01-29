#include "treeface/scene/SceneNode.h"
#include "treeface/scene/VisualObject.h"

#include "treeface/scene/guts/SceneNode_guts.h"

using namespace treecore;

namespace treeface {

SceneNode::SceneNode(): m_impl( new Impl() )
{}

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

void SceneNode::set_transform( const Mat4f& value ) noexcept
{
    m_impl->trans_dirty = true;
    m_impl->trans       = value;
}

bool SceneNode::add_item( SceneItem* obj )
{
    if ( m_impl->items.add( obj ) )
    {
        obj->m_node = this;
        return true;
    }
    else
    {
        return false;
    }
}

bool SceneNode::has_item( SceneItem* obj ) const noexcept
{
    return m_impl->items.contains( obj );
}

bool SceneNode::remove_item( SceneItem* obj )
{
    if ( m_impl->items.contains( obj ) )
    {
        m_impl->items.removeValue( obj );
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

SceneItem* SceneNode::get_item_at( int idx ) noexcept
{
    return m_impl->items[idx].get();
}

bool SceneNode::get_self_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept
{
    UniformMap::ConstIterator i( m_impl->self_uniforms );
    if ( m_impl->self_uniforms.select( name, i ) )
    {
        result = i.value();
        return true;
    }
    else
    {
        return false;
    }
}

bool SceneNode::get_uniform_value( const treecore::Identifier& name, UniversalValue& result )
{
    if ( get_self_uniform_value( name, result ) )
    {
        return true;
    }
    else
    {
        if (m_impl->uniform_cache_dirty)
            m_impl->recur_update_uniform_cache_from_parent();

        UniformMap::ConstIterator i_cache( m_impl->cached_descendent_uniforms );
        if ( m_impl->cached_descendent_uniforms.select( name, i_cache ) )
        {
            result = i_cache.value();
            return true;
        }
        else
        {
            return false;
        }
    }
}

void SceneNode::set_uniform_value( const treecore::Identifier& name, const UniversalValue& value )
{
    UniformMap::Iterator i( m_impl->self_uniforms );
    if ( m_impl->self_uniforms.insertOrSelect( name, value, i ) )
    {
        m_impl->recur_invalidate_uniform_cache_to_child();
    }
    else
    {
        if (i.value() != value)
        {
            i.value() = value;
            m_impl->recur_invalidate_uniform_cache_to_child();
        }
    }
}

bool SceneNode::has_self_uniform( const treecore::Identifier& name ) const noexcept
{
    return m_impl->self_uniforms.contains( name );
}

bool SceneNode::has_uniform( const treecore::Identifier& name )
{
    if ( has_self_uniform( name ) )
        return true;

    if (m_impl->uniform_cache_dirty)
        m_impl->recur_update_uniform_cache_from_parent();

    return m_impl->cached_descendent_uniforms.contains( name );
}

bool SceneNode::add_child( SceneNode* child )
{
    if (child->m_impl->parent)
        return false;

    bool add_re = m_impl->child_nodes.add( child );
    if (!add_re)
        return false;

    child->m_impl->parent       = this;
    child->m_impl->global_dirty = true;

    child->m_impl->uniform_cache_dirty = true;
    child->m_impl->recur_invalidate_uniform_cache_to_child();

    return true;
}

bool SceneNode::has_child( SceneNode* child ) const noexcept
{
    return m_impl->child_nodes.contains( child );
}

bool SceneNode::remove_child( SceneNode* child ) noexcept
{
    if ( m_impl->child_nodes.contains( child ) )
    {
        m_impl->child_nodes.removeValue( child );
        child->m_impl->parent = nullptr;
        child->m_impl->global_dirty = true;

        child->m_impl->uniform_cache_dirty = true;
        child->m_impl->recur_invalidate_uniform_cache_to_child();
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

SceneNode* SceneNode::get_child_at( int idx ) noexcept
{
    return m_impl->child_nodes[idx];
}

SceneNode* SceneNode::get_parent() noexcept
{
    return m_impl->parent;
}

} // namespace treeface
