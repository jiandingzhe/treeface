#ifndef TREEFACE_NODE_PRIVATE_H
#define TREEFACE_NODE_PRIVATE_H

#include "treeface/scene/scenenode.h"
#include "treeface/math/mat4.h"

#include <treecore/AlignedMalloc.h>
#include <treecore/Holder.h>
#include <treecore/SortedSet.h>

namespace treeface {

struct SceneNode::Impl: public treecore::AlignedMalloc<16>
{
    Mat4f trans;
    Mat4f trans_inv;

    Mat4f trans_global;
    Mat4f trans_global_inv;

    bool trans_dirty = true;
    bool global_dirty = true;
    treecore::SortedSet<treecore::Holder<SceneNode> > child_nodes;
    SceneNode* parent = nullptr;

    treecore::SortedSet<treecore::Holder<SceneItem> > items;

    void update_trans_descendent();
    void update_global_descendent();
};

} // namespace treeface

#endif // TREEFACE_NODE_PRIVATE_H
