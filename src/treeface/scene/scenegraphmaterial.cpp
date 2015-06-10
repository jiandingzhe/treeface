#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/program.h"

#include "treeface/guts/material_guts.h"

TREEFACE_NAMESPACE_BEGIN

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

void SceneGraphMaterial::instant_set_light(const Vec4f& direction, const Vec4f& color, float amb, float dfs, float spc) const NOEXCEPT
{
    m_program->instant_set_uniform(m_uni_light_direct, direction);
    m_program->instant_set_uniform(m_uni_light_color, color);
    m_program->instant_set_uniform(m_uni_light_amb, amb);
    m_program->instant_set_uniform(m_uni_light_dfs, dfs);
    m_program->instant_set_uniform(m_uni_light_spc, spc);
}

TREEFACE_NAMESPACE_END
