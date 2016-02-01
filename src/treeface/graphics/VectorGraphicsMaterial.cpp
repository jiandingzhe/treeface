#include "treeface/graphics/VectorGraphicsMaterial.h"

using namespace treecore;

#define UNI_NAME_LINE_WIDTH "line_width"
#define UNI_NAME_SKLT_MIN   "skeleton_min"
#define UNI_NAME_SKLT_MAX   "skeleton_max"

namespace treeface
{

const Identifier VectorGraphicsMaterial::UNIFORM_LINE_WIDTH( UNI_NAME_LINE_WIDTH );
const Identifier VectorGraphicsMaterial::UNIFORM_SKELETON_MIN( UNI_NAME_SKLT_MIN );
const Identifier VectorGraphicsMaterial::UNIFORM_SKELETON_MAX( UNI_NAME_SKLT_MAX );

VectorGraphicsMaterial::VectorGraphicsMaterial() {}

VectorGraphicsMaterial::~VectorGraphicsMaterial() {}

treecore::String VectorGraphicsMaterial::get_shader_source_addition() const noexcept
{
    return SceneGraphMaterial::get_shader_source_addition() + String(
        "uniform highp float " UNI_NAME_LINE_WIDTH ";\n"
        "uniform highp vec2 " UNI_NAME_SKLT_MIN ";\n"
        "uniform highp vec2 " UNI_NAME_SKLT_MAX ";\n"
        "\n" );
}

} // namespace treeface
