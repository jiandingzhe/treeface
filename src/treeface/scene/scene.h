#ifndef TREEFACE_SCENE_H
#define TREEFACE_SCENE_H

#include "treeface/common.h"
#include "treeface/math/vec4.h"

#include <treejuce/Object.h>
#include <treejuce/Holder.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class GeometryManager;
class MaterialManager;
class SceneNode;
class SceneNodeManager;
class SceneRenderer;

class Scene: public treejuce::Object
{
    friend class SceneRenderer;
public:
    Scene();
    Scene(GeometryManager* geo_mgr, MaterialManager* mat_mgr);

    JUCE_DECLARE_NON_COPYABLE(Scene)
    JUCE_DECLARE_NON_MOVABLE(Scene)

    virtual ~Scene();

    treejuce::Result build(const treejuce::String& name) NOEXCEPT;
    treejuce::Result build(const treejuce::var& root) NOEXCEPT;

    /**
     * @brief get the root of the scene hierarchy
     *
     * All nodes in the scene are added to this node. Do not modify the
     * transform matrix of this node.
     *
     * @return root node object.
     */
    SceneNode* get_root_node() NOEXCEPT;

    /**
     * @brief get node by name
     * @param name: node name.
     * @return If has node object with name, return node object; otherwise
     *         return nullptr.
     */
    SceneNode* get_node(const treejuce::String& name) NOEXCEPT;

    GeometryManager* get_geometry_manager() NOEXCEPT;
    MaterialManager* get_material_manager() NOEXCEPT;

    const Vec4f& get_global_light_color() const NOEXCEPT;
    void set_global_light_color(float r, float g, float b, float a) NOEXCEPT;
    void set_global_light_color(const Vec4f& value) NOEXCEPT;

    const Vec4f& get_global_light_direction() const NOEXCEPT;
    void set_global_light_direction(float x, float y, float z) NOEXCEPT;
    void set_global_light_direction(const Vec4f& value) NOEXCEPT;

    const Vec4f& get_global_light_ambient() const NOEXCEPT;
    void set_global_light_ambient(float r, float g, float b, float a) NOEXCEPT;
    void set_global_light_ambient(const Vec4f& value) NOEXCEPT;

private:
    struct Guts;
    Guts* m_guts = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_H
