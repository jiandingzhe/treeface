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

    SceneNode* add_nodes(const treecore::var& data);

    /**
     * @brief
     * @param data_name
     * @return
     * @see add_nodes(const treecore::var& data)
     */
    SceneNode* add_nodes(const treecore::String& data_name);

    /**
     * @brief get named node object
     * @param name
     * @return If has node object with name, return node object; otherwise
     *         return nullptr.
     */
    SceneNode* get_node(const treecore::String& name);

protected:
    VisualObject* create_visual_object(const treecore::var& data);
    void build_node(const treecore::var& data, SceneNode* node);

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_NODE_MANAGER_H
