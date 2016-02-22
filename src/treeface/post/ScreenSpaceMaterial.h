#ifndef TREEFACE_SCREEN_SPACE_MATERIAL_H
#define TREEFACE_SCREEN_SPACE_MATERIAL_H

#include "treeface/scene/Material.h"

namespace treeface
{

class ScreenSpaceMaterial: public Material
{
public:
    ScreenSpaceMaterial() = default;

protected:
    treecore::String get_shader_source_addition() const noexcept override;

};

} // namespace treeface

#endif // TREEFACE_SCREEN_SPACE_MATERIAL_H
