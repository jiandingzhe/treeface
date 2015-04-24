#ifndef TREEFACE_VERTEX_TEMPLATE_H
#define TREEFACE_VERTEX_TEMPLATE_H

#include "treeface/common.h"
#include "treeface/gl/vertexattrib.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/IntTypes.h>

TREEFACE_NAMESPACE_BEGIN

class VertexTemplate
{
public:
    VertexTemplate();

    VertexTemplate(VertexTemplate&& other): m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    VertexTemplate& operator = (VertexTemplate&& other)
    {
        m_impl = other.m_impl;
        other.m_impl = nullptr;
        return *this;
    }

    JUCE_DECLARE_NON_COPYABLE(VertexTemplate);

    void add_attrib(const VertexAttrib& attr, bool normalize);

    size_t vertex_size() const NOEXCEPT;
    int n_elems() const NOEXCEPT;
    int n_attribs() const NOEXCEPT;

    size_t get_elem_offset(int i_elem) const NOEXCEPT;
    size_t get_elem_offset(int i_attr, int i_elem_in_attr) const NOEXCEPT;

    GLenum get_elem_type(int i_elem) const NOEXCEPT;
    GLenum get_elem_size(int i_elem) const NOEXCEPT;

    const HostVertexAttrib& get_attrib(int i_attr) const NOEXCEPT;
    const HostVertexAttrib& get_elem_attrib(int i_elem) const NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VERTEX_TEMPLATE_H
