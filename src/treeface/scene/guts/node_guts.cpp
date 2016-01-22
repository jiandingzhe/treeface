#include "treeface/scene/guts/node_guts.h"

namespace treeface {

void SceneNode::Impl::update_trans_descendent()
{
    trans_inv = trans;
    trans_inv.inverse();

    trans_dirty = false;

    update_global_descendent();
}

void SceneNode::Impl::update_global_descendent()
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

} // namespace treeface
