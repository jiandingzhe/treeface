#ifndef TREEFACE_VECTOR_GRAPHICS_MATERIAL_H
#define TREEFACE_VECTOR_GRAPHICS_MATERIAL_H

#include "treeface/scene/SceneGraphMaterial.h"

namespace treeface
{

class VectorGraphicsMaterial: public SceneGraphMaterial
{
public:
    static const treecore::Identifier UNIFORM_LINE_WIDTH;
    static const treecore::Identifier UNIFORM_SKELETON_MIN;
    static const treecore::Identifier UNIFORM_SKELETON_MAX;

    VectorGraphicsMaterial();
    virtual ~VectorGraphicsMaterial();

protected:
    treecore::String get_shader_source_addition() const noexcept override;
};

} // namespace treeface

#endif // TREEFACE_VECTOR_GRAPHICS_MATERIAL_H
