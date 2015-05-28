#ifndef TREEFACE_SCENE_GUTS_H
#define TREEFACE_SCENE_GUTS_H

#include "treeface/scene/scene.h"
#include "treeface/scene/scenenode.h"
#include "treeface/math.h"

TREEFACE_NAMESPACE_BEGIN

struct Scene::Guts: public NedBase
{
    treejuce::Holder<SceneNode>        root_node = new SceneNode();
    treejuce::Holder<SceneNodeManager> node_mgr;
    treejuce::Holder<MaterialManager>  mat_mgr;
    treejuce::Holder<GeometryManager>  geo_mgr;

    Vec4f global_light_direction{0.577350269, 0.577350269, 0.577350269, 0};
    Vec4f global_light_color{1, 1, 1, 1};
    float global_light_ambient = 1;
    float global_light_diffuse = 1;
    float global_light_specular = 1;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_GUTS_H
