#ifndef TREEFACE_NODE_PRIVATE_H
#define TREEFACE_NODE_PRIVATE_H

#include "treeface/scene/SceneNode.h"
#include "treeface/scene/SceneObject.h"
#include "treeface/scene/guts/SceneObjectSlot.h"
#include "treeface/scene/guts/Utils.h"

#include "treeface/math/Mat4.h"

#include <treecore/AlignedMalloc.h>
#include <treecore/HashSet.h>
#include <treecore/RefCountHolder.h>
#include <treecore/SortedSet.h>

namespace treeface {

TREECORE_ALN_BEGIN( 16 )
struct SceneNode::Guts
{
    TREECORE_ALIGNED_ALLOCATOR( SceneNode::Guts )
    Mat4f trans;
    Mat4f trans_inv;

    Mat4f trans_global;
    Mat4f trans_global_inv;

    bool trans_dirty = true;
    bool global_dirty        = true;
    bool uniform_cache_dirty = true;

    treecore::SortedSet<treecore::RefCountHolder<SceneNode> > child_nodes;
    SceneNode* parent = nullptr;

    UniformMap self_uniforms;
    UniformMap cached_inherit_uniforms;

    SceneObjectSlotArray objects;

    void update_trans_descendent();
    void update_global_descendent();

    void update_uniform_cache();
    void recur_update_uniform_cache_from_parent();
    void recur_invalidate_uniform_cache_to_child();
} TREECORE_ALN_END( 16 );

} // namespace treeface

#endif // TREEFACE_NODE_PRIVATE_H
