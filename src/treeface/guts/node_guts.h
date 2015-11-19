#ifndef TREEFACE_NODE_PRIVATE_H
#define TREEFACE_NODE_PRIVATE_H

#include "treeface/scene/scenenode.h"
#include "treeface/math/mat4.h"

#include <treecore/AlignedMalloc.h>
#include <treecore/RefCountHolder.h>
#include <treecore/SortedSet.h>

namespace treeface {

TREECORE_ALN_BEGIN(16)
struct SceneNode::Impl
{
    TREECORE_ALIGNED_ALLOCATOR(SceneNode::Impl);
    Mat4f trans;
    Mat4f trans_inv;

    Mat4f trans_global;
    Mat4f trans_global_inv;

    bool trans_dirty = true;
    bool global_dirty = true;
    treecore::SortedSet<treecore::RefCountHolder<SceneNode> > child_nodes;
    SceneNode* parent = nullptr;

    treecore::SortedSet<treecore::RefCountHolder<SceneItem> > items;

    void update_trans_descendent();
    void update_global_descendent();
} TREECORE_ALN_END(16);

} // namespace treeface

#endif // TREEFACE_NODE_PRIVATE_H
