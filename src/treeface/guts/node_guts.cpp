#include "treeface/guts/node_guts.h"

TREEFACE_NAMESPACE_BEGIN

void SceneNode::Impl::update_trans_descendent()
{
    trans_inv = trans;
    trans_inv.inverse();

    trans_normal = trans.get_normal_matrix();

    trans_dirty = false;

    update_global_descendent();
}

void SceneNode::Impl::update_global_descendent()
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

TREEFACE_NAMESPACE_END
