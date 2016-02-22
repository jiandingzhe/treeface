#include "treeface/post/ScreenSpaceMaterial.h"

namespace treeface
{

treecore::String ScreenSpaceMaterial::get_shader_source_addition() const noexcept
{
    return Material::get_shader_source_addition() +
           "float highp width_px;\n"
           "float highp height_px;\n";
}

} // namespace treeface
