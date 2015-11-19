#ifndef TREEFACE_SCENE_GRAPH_MATERIAL_H
#define TREEFACE_SCENE_GRAPH_MATERIAL_H

#include "treeface/material.h"
#include "treeface/math/mat4.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

class SceneGraphMaterial: public Material
{
    friend class MaterialManager;
    friend class SceneRenderer;

public:
    SceneGraphMaterial();
    virtual ~SceneGraphMaterial();

    void instant_set_matrix_model_view(const Mat4f& mat) const noexcept;
    void instant_set_matrix_proj(const Mat4f& mat) const noexcept;
    void instant_set_matrix_model_view_proj(const Mat4f& mat) const noexcept;
    void instant_set_matrix_norm(const Mat4f& mat) const noexcept;
    void instant_set_light(const Vec4f& direction, const Vec4f& color, const Vec4f& ambient) const noexcept;

    bool is_translucent() const noexcept
    {
        return m_translucent;
    }

    bool project_shadow() const noexcept
    {
        return m_project_shadow;
    }

    bool receive_shadow() const noexcept
    {
        return m_receive_shadow;
    }

    TREECORE_DECLARE_NON_COPYABLE(SceneGraphMaterial);
    TREECORE_DECLARE_NON_MOVABLE(SceneGraphMaterial);

protected:
    bool m_translucent = false;
    bool m_project_shadow = true;
    bool m_receive_shadow = true;
    GLint m_uni_model_view = -1;
    GLint m_uni_proj = -1;
    GLint m_uni_model_view_proj = -1;
    GLint m_uni_norm = -1;
    GLint m_uni_light_direct = -1;
    GLint m_uni_light_color = -1;
    GLint m_uni_light_ambient = -1;
};

} // namespace treeface

#endif // TREEFACE_SCENE_GRAPH_MATERIAL_H
