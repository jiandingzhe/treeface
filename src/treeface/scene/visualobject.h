#ifndef TREEFACE_VISUAL_OBJECT_H
#define TREEFACE_VISUAL_OBJECT_H

#include "treeface/scene/sceneitem.h"

#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

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

    treejuce::Result build(Geometry* geom, SceneGraphMaterial* mat) NOEXCEPT;

    SceneGraphMaterial* get_material() NOEXCEPT;
    Geometry* get_geometry() NOEXCEPT;
    VertexArray* get_vertex_array() NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VISUAL_OBJECT_H
