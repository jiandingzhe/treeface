#ifndef TREEFACE_VISUAL_ITEM_H
#define TREEFACE_VISUAL_ITEM_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class Material;
class Geometry;
class VertexArray;

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
    }

    JUCE_DECLARE_NON_COPYABLE(VisualItem);

    treejuce::Result build(const treejuce::String& geom_name, const treejuce::String& mat_name) NOEXCEPT;
    treejuce::Result build(const treejuce::var& node) NOEXCEPT;
    treejuce::Result build(Geometry* geom, Material* mat) NOEXCEPT;

    Material* get_material() NOEXCEPT;
    Geometry* get_geometry() NOEXCEPT;
    VertexArray* get_vertex_array() NOEXCEPT;

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VISUAL_ITEM_H
