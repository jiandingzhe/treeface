#ifndef TREEFACE_VISUAL_OBJECT_H
#define TREEFACE_VISUAL_OBJECT_H

#include "treeface/scene/sceneitem.h"

#include <treecore/RefCountObject.h>

namespace treecore {
class Result;
class var;
} // namespace treecore

namespace treeface {

class SceneGraphMaterial;
class Geometry;
struct VertexArray;

class VisualObject: public SceneItem
{
public:
    VisualObject();
    virtual ~VisualObject();

    TREECORE_DECLARE_NON_COPYABLE(VisualObject);
    TREECORE_DECLARE_NON_MOVABLE(VisualObject);

    treecore::Result build(Geometry* geom, SceneGraphMaterial* mat) noexcept;

    SceneGraphMaterial* get_material() noexcept;
    Geometry* get_geometry() noexcept;
    VertexArray* get_vertex_array() noexcept;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_VISUAL_OBJECT_H
