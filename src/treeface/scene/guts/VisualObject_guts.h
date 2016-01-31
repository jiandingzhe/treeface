#ifndef TREEFACE_VISUAL_OBJECT_GUTS_H
#define TREEFACE_VISUAL_OBJECT_GUTS_H

#include "treeface/scene/VisualObject.h"
#include "treeface/scene/VisualObject.h"
#include "treeface/scene/guts/Utils.h"

namespace treeface
{

struct VisualObject::Impl
{
    treecore::RefCountHolder<SceneGraphMaterial> material     = nullptr;
    treecore::RefCountHolder<Geometry>           geometry     = nullptr;
    treecore::RefCountHolder<VertexArray>        vertex_array = nullptr;
    UniformMap uniforms;
};


} // namespace treeface

#endif // TREEFACE_VISUAL_OBJECT_GUTS_H
