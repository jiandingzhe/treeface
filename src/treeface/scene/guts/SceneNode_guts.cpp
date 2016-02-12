#include "treeface/scene/guts/SceneNode_guts.h"

using namespace treecore;

namespace treeface {

void SceneNode::Guts::update_trans_descendent()
{
    trans_inv = trans;
    trans_inv.inverse();

    trans_dirty = false;

    update_global_descendent();
}

void SceneNode::Guts::update_global_descendent()
{
    if (parent)
    {
        trans_global = parent->get_global_transform() * trans;
    }
    else
    {
        trans_global = trans;
    }

    trans_global_inv = trans_global;
    trans_global_inv.inverse();

    global_dirty = false;
}

void SceneNode::Guts::update_uniform_cache()
{
    jassert( uniform_cache_dirty );

    if (parent != nullptr)
    {
        // parent self
        cached_inherit_uniforms = parent->m_impl->self_uniforms;

        // parent inherit
        jassert( !parent->m_impl->uniform_cache_dirty );
        UniformMap::ConstIterator it_parent( parent->m_impl->cached_inherit_uniforms );
        while ( it_parent.next() )
        {
            UniformMap::Iterator dull( cached_inherit_uniforms );
            cached_inherit_uniforms.insertOrSelect( it_parent.key(), it_parent.value(), dull );
        }
    }
    else
    {
        cached_inherit_uniforms.clear();
    }

    uniform_cache_dirty = false;
}

void SceneNode::Guts::recur_update_uniform_cache_from_parent()
{
    jassert( uniform_cache_dirty );

    if (parent != nullptr && parent->m_impl->uniform_cache_dirty)
        parent->m_impl->recur_update_uniform_cache_from_parent();

    update_uniform_cache();
}

void SceneNode::Guts::recur_invalidate_uniform_cache_to_child()
{
    uniform_cache_dirty = true;

    for (SceneObjectSlot& slot : objects)
        slot.uniform_cache_dirty = true;

    for (RefCountHolder<SceneNode>& child : child_nodes)
        child->m_impl->recur_invalidate_uniform_cache_to_child();
}

} // namespace treeface
