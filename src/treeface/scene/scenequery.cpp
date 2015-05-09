#include "treeface/scene/scenequery.h"

#include "treeface/scene/node.h"

#include <treejuce/Result.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

SceneQuery::SceneQuery()
{
}

SceneQuery::~SceneQuery()
{
}

treejuce::Result SceneQuery::traverse(Node* root) NOEXCEPT
{
    Result begin_re = traverse_begin();
    if (!begin_re)
        return begin_re;

    Result node_re = recur_part(root);
    if (!node_re)
        return node_re;

    Result end_re = traverse_end();
    if (!end_re)
        return end_re;

    return Result::ok();
}

treejuce::Result SceneQuery::recur_part(Node* node) NOEXCEPT
{
    Result one_node_re = traverse_one_node(node);
    if (!one_node_re)
        return one_node_re;

    for (int i = 0; i < node->get_num_children(); i++)
    {
        Node* child = node->get_child_at(i);
        Result child_re = recur_part(child);
        if (!child_re)
            return child_re;
    }

    return Result::ok();
}

TREEFACE_NAMESPACE_END
