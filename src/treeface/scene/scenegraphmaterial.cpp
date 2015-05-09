#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/program.h"

TREEFACE_NAMESPACE_BEGIN

SceneGraphMaterial::SceneGraphMaterial()
{
}

SceneGraphMaterial::~SceneGraphMaterial()
{
}

void SceneGraphMaterial::instant_set_transform(const Mat4f& trans_position, const Mat4f& trans_normal) const NOEXCEPT
{
    if (m_uni_trans >= 0)
        m_program->instant_set_uniform(m_uni_trans, trans_position);

    if (m_uni_norm_trans >= 0)
        m_program->instant_set_uniform(m_uni_norm_trans, trans_normal);
}

void SceneGraphMaterial::instant_set_projection(const Mat4f& value) const NOEXCEPT
{
    if (m_uni_proj_trans >= 0)
        m_program->instant_set_uniform(m_uni_proj_trans, value);
}

void SceneGraphMaterial::instant_set_light(const Vec4f& light_direct, const Vec4f& light_color) const NOEXCEPT
{
    if (m_uni_light_direct >= 0)
        m_program->instant_set_uniform(m_uni_light_direct, light_direct);
    if (m_uni_light_color >= 0)
        m_program->instant_set_uniform(m_uni_light_color, light_color);
}

TREEFACE_NAMESPACE_END
