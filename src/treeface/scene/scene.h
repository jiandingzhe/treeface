#ifndef TREEFACE_SCENE_H
#define TREEFACE_SCENE_H

#include "treeface/common.h"
#include "treeface/math/vec4.h"

#include <treecore/RefCountObject.h>
#include <treecore/RefCountHolder.h>

namespace treecore {
class Result;
class var;
} // namespace treecore

namespace treeface {

class GeometryManager;
class MaterialManager;
class SceneNode;
class SceneNodeManager;
class SceneRenderer;

class Scene: public treecore::RefCountObject
{
    friend class SceneRenderer;
public:
    Scene();
    Scene(GeometryManager* geo_mgr, MaterialManager* mat_mgr);

    TREECORE_DECLARE_NON_COPYABLE(Scene)
    TREECORE_DECLARE_NON_MOVABLE(Scene)

    virtual ~Scene();

    treecore::Result build(const treecore::String& name) noexcept;
    treecore::Result build(const treecore::var& root) noexcept;

    /**
     * @brief get the root of the scene hierarchy
     *
     * All nodes in the scene are added to this node. Do not modify the
     * transform matrix of this node.
     *
     * @return root node object.
     */
    SceneNode* get_root_node() noexcept;

    /**
     * @brief get node by name
     * @param name: node name.
     * @return If has node object with name, return node object; otherwise
     *         return nullptr.
     */
    SceneNode* get_node(const treecore::String& name) noexcept;

    GeometryManager* get_geometry_manager() noexcept;
    MaterialManager* get_material_manager() noexcept;

    const Vec4f& get_global_light_color() const noexcept;
    void set_global_light_color(float r, float g, float b, float a) noexcept;
    void set_global_light_color(const Vec4f& value) noexcept;

    const Vec4f& get_global_light_direction() const noexcept;
    void set_global_light_direction(float x, float y, float z) noexcept;
    void set_global_light_direction(const Vec4f& value) noexcept;

    const Vec4f& get_global_light_ambient() const noexcept;
    void set_global_light_ambient(float r, float g, float b, float a) noexcept;
    void set_global_light_ambient(const Vec4f& value) noexcept;

private:
    struct Guts;
    Guts* m_guts = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_H
