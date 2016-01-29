#include "treeface/scene/SceneGraphMaterial.h"

#include "treeface/gl/Program.h"

#include "treeface/scene/guts/Material_guts.h"

namespace treeface {

SceneGraphMaterial::SceneGraphMaterial()
{}

SceneGraphMaterial::~SceneGraphMaterial()
{}

void SceneGraphMaterial::set_matrix_model_view( const Mat4f& mat ) const noexcept
{
    m_program->set_uniform( m_uni_model_view, mat );
}

void SceneGraphMaterial::set_matrix_proj( const Mat4f& mat ) const noexcept
{
    m_program->set_uniform( m_uni_proj, mat );
}

void SceneGraphMaterial::set_matrix_model_view_proj( const Mat4f& mat ) const noexcept
{
    m_program->set_uniform( m_uni_model_view_proj, mat );
}

void SceneGraphMaterial::set_matrix_norm( const Mat4f& mat ) const noexcept
{
    m_program->set_uniform( m_uni_norm, mat );
}

void SceneGraphMaterial::set_light( const Vec4f& direction, const Vec4f& color, const Vec4f& ambient ) const noexcept
{
    m_program->set_uniform( m_uni_light_direct,  direction );
    m_program->set_uniform( m_uni_light_color,   color );
    m_program->set_uniform( m_uni_light_ambient, ambient );
}

} // namespace treeface
