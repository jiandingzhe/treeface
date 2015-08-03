#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/program.h"

#include "treeface/guts/material_guts.h"

namespace treeface {

SceneGraphMaterial::SceneGraphMaterial()
{
}

SceneGraphMaterial::~SceneGraphMaterial()
{
}

void SceneGraphMaterial::instant_set_matrix_model_view(const Mat4f& mat) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_model_view, mat);
}

void SceneGraphMaterial::instant_set_matrix_proj(const Mat4f& mat) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_proj, mat);
}

void SceneGraphMaterial::instant_set_matrix_model_view_proj(const Mat4f& mat) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_model_view_proj, mat);
}

void SceneGraphMaterial::instant_set_matrix_norm(const Mat4f& mat) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_norm, mat);
}

void SceneGraphMaterial::instant_set_light(const Vec4f& direction, const Vec4f& color, const Vec4f& ambient) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_light_direct, direction);
    m_program->instant_set_uniform(m_uni_light_color, color);
    m_program->instant_set_uniform(m_uni_light_ambient, ambient);
}

} // namespace treeface
