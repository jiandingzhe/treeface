#ifndef TREEFACE_VISUAL_OBJECT_GUTS_H
#define TREEFACE_VISUAL_OBJECT_GUTS_H

#include "treeface/scene/VisualObject.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/guts/Utils.h"
#include "treeface/gl/VertexArray.h"

namespace treeface
{

struct VisualObject::Impl
{
    Impl(Geometry* geom, SceneGraphMaterial* mat);
    ~Impl();

    void update_uniform_cache();

    VisualObject::Impl* same_geom_prev = nullptr;
    VisualObject::Impl* same_geom_next = nullptr;
    treecore::RefCountHolder<SceneGraphMaterial> material     = nullptr;
    treecore::RefCountHolder<Geometry>           geometry     = nullptr;
    treecore::RefCountHolder<VertexArray>        vertex_array = nullptr;
    bool uniform_cache_dirty = true;
    treecore::Array<UniformKV> cached_uniforms;
    UniformMap uniforms;
};


} // namespace treeface

#endif // TREEFACE_VISUAL_OBJECT_GUTS_H
