#ifndef TREEFACE_SCENE_GUTS_H
#define TREEFACE_SCENE_GUTS_H

#include "treeface/scene/scene.h"
#include "treeface/scene/scenenode.h"
#include "treeface/math.h"

namespace treeface {

struct Scene::Guts: public treecore::AlignedMalloc<16>
{
    treecore::Holder<SceneNode>        root_node = new SceneNode();
    treecore::Holder<SceneNodeManager> node_mgr;
    treecore::Holder<MaterialManager>  mat_mgr;
    treecore::Holder<GeometryManager>  geo_mgr;

    Vec4f global_light_direction{0.577350269, 0.577350269, 0.577350269, 0};
    Vec4f global_light_color{1, 1, 1, 1};
    Vec4f global_light_ambient{0, 0, 0, 1};
};

} // namespace treeface

#endif // TREEFACE_SCENE_GUTS_H
