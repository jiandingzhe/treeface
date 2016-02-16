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

} // namespace treeface
