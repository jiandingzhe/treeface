#ifndef TREEFACE_NODE_MANAGER_H
#define TREEFACE_NODE_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Object.h>
#include <treejuce/Result.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class String;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN
class GeometryManager;
class MaterialManager;
class Node;
class VisualItem;

class NodeManager: public treejuce::Object
{
public:
    NodeManager() = delete;
    NodeManager(GeometryManager* geo_mgr, MaterialManager* mat_mgr);

    JUCE_DECLARE_NON_COPYABLE(NodeManager);
    JUCE_DECLARE_NON_MOVABLE(NodeManager);

    virtual ~NodeManager();

    treejuce::Result get_node(const treejuce::String& name, Node** node_pp);
    bool has_node(const treejuce::String& name);

    treejuce::Result build_visual_item(const treejuce::var& data, VisualItem* visual_item);
    treejuce::Result build_node(const treejuce::var& data, Node* node);

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_NODE_MANAGER_H
