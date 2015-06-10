#ifndef TREEFACE_NODE_PRIVATE_H
#define TREEFACE_NODE_PRIVATE_H

#include "treeface/scene/scenenode.h"
#include "treeface/math/mat4.h"
#include "treeface/nedbase.h"

#include <treejuce/Holder.h>
#include <treejuce/SortedSet.h>

TREEFACE_NAMESPACE_BEGIN

struct SceneNode::Impl: public NedBase
{
    Mat4f trans;
    Mat4f trans_inv;

    Mat4f trans_global;
    Mat4f trans_global_inv;

    bool trans_dirty = true;
    bool global_dirty = true;
    treejuce::SortedSet<treejuce::Holder<SceneNode> > child_nodes;
    SceneNode* parent = nullptr;

    treejuce::SortedSet<treejuce::Holder<SceneItem> > items;

    void update_trans_descendent();
    void update_global_descendent();
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_NODE_PRIVATE_H
