#ifndef TREEFACE_SCENE_NODE_MANAGER_H
#define TREEFACE_SCENE_NODE_MANAGER_H

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
class SceneNode;
class VisualObject;

class SceneNodeManager: public treejuce::Object
{
public:
    SceneNodeManager() = delete;
    SceneNodeManager(GeometryManager* geo_mgr, MaterialManager* mat_mgr);

    JUCE_DECLARE_NON_COPYABLE(SceneNodeManager);
    JUCE_DECLARE_NON_MOVABLE(SceneNodeManager);

    virtual ~SceneNodeManager();

    treejuce::Result add_nodes(const treejuce::var& data);

    /**
     * @brief
     * @param data_name
     * @return
     * @see add_nodes(const treejuce::var& data)
     */
    treejuce::Result add_nodes(const treejuce::String& data_name);

    /**
     * @brief get named node object
     * @param name
     * @return If has node object with name, return node object; otherwise
     *         return nullptr.
     */
    SceneNode* get_node(const treejuce::String& name);

    treejuce::Result build_visual_item(const treejuce::var& data, VisualObject* visual_item);
    treejuce::Result build_node(const treejuce::var& data, SceneNode* node);

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_NODE_MANAGER_H
