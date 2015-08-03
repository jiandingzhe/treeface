#ifndef TREEFACE_VISUAL_OBJECT_H
#define TREEFACE_VISUAL_OBJECT_H

#include "treeface/scene/sceneitem.h"

#include <treecore/Object.h>

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

    JUCE_DECLARE_NON_COPYABLE(VisualObject);
    JUCE_DECLARE_NON_MOVABLE(VisualObject);

    treecore::Result build(Geometry* geom, SceneGraphMaterial* mat) NOEXCEPT;

    SceneGraphMaterial* get_material() NOEXCEPT;
    Geometry* get_geometry() NOEXCEPT;
    VertexArray* get_vertex_array() NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_VISUAL_OBJECT_H
