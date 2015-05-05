#ifndef TREEFACE_SCENE_QUERY_H
#define TREEFACE_SCENE_QUERY_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN
class Node;

class SceneQuery: public treejuce::Object
{
public:
    SceneQuery();

    JUCE_DECLARE_NON_COPYABLE(SceneQuery);

    virtual ~SceneQuery();

    treejuce::Result traverse(Node* root) NOEXCEPT;

protected:
    virtual treejuce::Result traverse_begin() NOEXCEPT = 0;
    virtual treejuce::Result traverse_one_node(Node* node) NOEXCEPT = 0;
    virtual treejuce::Result traverse_end() NOEXCEPT = 0;

private:
    treejuce::Result recur_part(Node* node) NOEXCEPT;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_QUERY_H
