#ifndef TREEFACE_SCENE_GUTS_H
#define TREEFACE_SCENE_GUTS_H

#include "treeface/scene/scene.h"
#include "treeface/scene/scenenode.h"
#include "treeface/math.h"

namespace treeface {

TREECORE_ALN_BEGIN(16)
struct Scene::Guts
{
    TREECORE_ALIGNED_ALLOCATOR(Scene::Guts);
    treecore::RefCountHolder<SceneNode>        root_node = new SceneNode();
    treecore::RefCountHolder<SceneNodeManager> node_mgr;
    treecore::RefCountHolder<MaterialManager>  mat_mgr;
    treecore::RefCountHolder<GeometryManager>  geo_mgr;

    Vec4f global_light_direction{0.577350269, 0.577350269, 0.577350269, 0};
    Vec4f global_light_color{1, 1, 1, 1};
    Vec4f global_light_ambient{0, 0, 0, 1};
} TREECORE_ALN_END(16);

} // namespace treeface

#endif // TREEFACE_SCENE_GUTS_H
