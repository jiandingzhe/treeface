#ifndef TREEFACE_VISUAL_ITEM_H
#define TREEFACE_VISUAL_ITEM_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class SceneGraphMaterial;
class Geometry;
struct VertexArray;

class VisualItem: public treejuce::Object
{
public:
    VisualItem();
    virtual ~VisualItem();

    VisualItem(VisualItem&& other): m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    VisualItem& operator = (VisualItem&& other)
    {
        jassert(m_impl == nullptr);
        m_impl = other.m_impl;
        other.m_impl = nullptr;
        return *this;
    }

    JUCE_DECLARE_NON_COPYABLE(VisualItem);

    treejuce::Result build(Geometry* geom, SceneGraphMaterial* mat) NOEXCEPT;

    SceneGraphMaterial* get_material() NOEXCEPT;
    Geometry* get_geometry() NOEXCEPT;
    VertexArray* get_vertex_array() NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VISUAL_ITEM_H
