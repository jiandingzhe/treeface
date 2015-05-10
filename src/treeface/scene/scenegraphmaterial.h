#ifndef TREEFACE_SCENE_GRAPH_MATERIAL_H
#define TREEFACE_SCENE_GRAPH_MATERIAL_H

#include "treeface/material.h"
#include "treeface/math/mat4.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class SceneGraphMaterial: public Material
{
    friend class MaterialManager;
    friend class SceneRenderer;

public:
    SceneGraphMaterial();
    virtual ~SceneGraphMaterial();

    void instant_set_transform(const Mat4f& trans_position, const Mat4f& trans_normal) const NOEXCEPT;
    void instant_set_projection(const Mat4f& value) const NOEXCEPT;
    void instant_set_light(const Vec4f& light_direct, const Vec4f& light_color) const NOEXCEPT;

    JUCE_DECLARE_NON_COPYABLE(SceneGraphMaterial);
    JUCE_DECLARE_NON_MOVABLE(SceneGraphMaterial);

protected:
    bool m_translucent = false;
    bool m_project_shadow = true;
    bool m_receive_shadow = true;
    GLint m_uni_trans = -1;
    GLint m_uni_norm_trans = -1;
    GLint m_uni_proj_trans = -1;
    GLint m_uni_light_direct = -1;
    GLint m_uni_light_color = -1;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_GRAPH_MATERIAL_H
