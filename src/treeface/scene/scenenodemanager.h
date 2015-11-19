#ifndef TREEFACE_SCENE_NODE_MANAGER_H
#define TREEFACE_SCENE_NODE_MANAGER_H

#include "treeface/common.h"

#include <treecore/RefCountHolder.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>

namespace treecore {
class String;
class var;
} // namespace treecore

namespace treeface {
class GeometryManager;
class MaterialManager;
class SceneNode;
class VisualObject;

class SceneNodeManager: public treecore::RefCountObject
{
public:
    SceneNodeManager() = delete;
    SceneNodeManager(GeometryManager* geo_mgr, MaterialManager* mat_mgr);

    TREECORE_DECLARE_NON_COPYABLE(SceneNodeManager);
    TREECORE_DECLARE_NON_MOVABLE(SceneNodeManager);

    virtual ~SceneNodeManager();

    treecore::Result add_nodes(const treecore::var& data, SceneNode** root_node_pp);
    treecore::Result add_nodes(const treecore::var& data, treecore::RefCountHolder<SceneNode>& root_node);

    /**
     * @brief
     * @param data_name
     * @return
     * @see add_nodes(const treecore::var& data)
     */
    treecore::Result add_nodes(const treecore::String& data_name, SceneNode** root_node_pp);
    treecore::Result add_nodes(const treecore::String& data_name, treecore::RefCountHolder<SceneNode>& root_node);

    /**
     * @brief get named node object
     * @param name
     * @return If has node object with name, return node object; otherwise
     *         return nullptr.
     */
    SceneNode* get_node(const treecore::String& name);

protected:
    treecore::Result build_visual_item(const treecore::var& data, VisualObject* visual_item);
    treecore::Result build_node(const treecore::var& data, SceneNode* node);

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_NODE_MANAGER_H
