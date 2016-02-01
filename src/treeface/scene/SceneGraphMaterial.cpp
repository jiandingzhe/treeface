#include "treeface/scene/SceneGraphMaterial.h"

#include "treeface/gl/Program.h"

#include "treeface/scene/guts/Material_guts.h"

#define NAME_MAT_MV       "matrix_model_view"
#define NAME_MAT_PROJ     "matrix_project"
#define NAME_MAT_MVP      "matrix_model_view_project"
#define NAME_MAT_NORM     "matrix_normal"
#define NAME_GLB_L_DIRECT "global_light_direction"
#define NAME_GLB_L_COLOR  "global_light_color"
#define NAME_GLB_L_AMB    "global_light_ambient"

using namespace treecore;

namespace treeface {

const treecore::Identifier SceneGraphMaterial::UNIFORM_MATRIX_MODEL_VIEW( NAME_MAT_MV );
const treecore::Identifier SceneGraphMaterial::UNIFORM_MATRIX_PROJECT( NAME_MAT_PROJ );
const treecore::Identifier SceneGraphMaterial::UNIFORM_MATRIX_MODEL_VIEW_PROJECT( NAME_MAT_MVP );
const treecore::Identifier SceneGraphMaterial::UNIFORM_MATRIX_NORMAL( NAME_MAT_NORM );
const treecore::Identifier SceneGraphMaterial::UNIFORM_GLOBAL_LIGHT_DIRECTION( NAME_GLB_L_DIRECT );
const treecore::Identifier SceneGraphMaterial::UNIFORM_GLOBAL_LIGHT_COLOR( NAME_GLB_L_COLOR );
const treecore::Identifier SceneGraphMaterial::UNIFORM_GLOBAL_LIGHT_AMBIENT( NAME_GLB_L_AMB );

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

treecore::String SceneGraphMaterial::get_shader_source_addition() const noexcept
{
    return Material::get_shader_source_addition() + String(
        "uniform highp mat4 " NAME_MAT_MV ";\n"
        "uniform highp mat4 " NAME_MAT_PROJ ";\n"
        "uniform highp mat4 " NAME_MAT_MVP ";\n"
        "uniform highp mat4 " NAME_MAT_NORM ";\n"
        "uniform mediump vec4 " NAME_GLB_L_DIRECT ";\n"
        "uniform mediump vec4 " NAME_GLB_L_COLOR ";\n"
        "uniform mediump vec4 " NAME_GLB_L_AMB ";\n"
        "\n" );
}

} // namespace treeface
