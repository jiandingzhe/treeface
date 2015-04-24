#include "treeface/misc/vertextemplate.h"

#include "treeface/gl/type.h"

#include <treejuce/Array.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct VertexTemplate::Impl
{
    treejuce::Array<HostVertexAttrib> attrs;
    treejuce::Array<size_t> elem_offsets;
    treejuce::Array<int> elem_attr_index;
    uint32 size = 0;
};

VertexTemplate::VertexTemplate(): m_impl(new Impl())
{
}

//VertexTemplate::VertexTemplate(treejuce::ArrayRef<const VertexAttrib> attrs)
//    : m_impl(new Impl())
//{
//    m_impl->attrs.addArray(attrs.get_const_data(), attrs.size());

//    uint32 attr_offset = 0;

//    for (int i_attr = 0; i_attr < attrs.size(); i_attr++)
//    {
//        VertexAttrib attr = attrs[i_attr];
//        m_impl->attr_offsets.add(attr_offset);

//        for (int i_elem = 0; i_elem < attr.n_elem; i_elem++)
//        {
//            m_impl->elem_attr_index.add(i_attr);

//            uint32 elem_offset = attr_offset + attr.get_elem_offset(i_elem);
//            m_impl->elem_offsets.add(elem_offset);
//        }

//        attr_offset += attr.size();
//    }

//    m_impl->size = attr_offset;
//}


void VertexTemplate::add_attrib(const VertexAttrib& attr, bool normalize)
{
    size_t attr_offset = m_impl->size;
    int prev_n_attr = m_impl->attrs.size();
    m_impl->attrs.add(HostVertexAttrib(attr, attr_offset, normalize));

    size_t elem_offset = attr_offset;
    int elem_size = size_of_gl_type(attr.type);

    for (int i = 0; i < attr.n_elem; i++)
    {
        m_impl->elem_offsets.add(elem_offset);
        m_impl->elem_attr_index.add(prev_n_attr);
        elem_offset += elem_size;
    }

    m_impl->size += attr.size();
}

size_t VertexTemplate::vertex_size() const NOEXCEPT
{
    return m_impl->size;
}

int VertexTemplate::n_elems() const NOEXCEPT
{
    return m_impl->elem_offsets.size();
}

int VertexTemplate::n_attribs() const NOEXCEPT
{
    return m_impl->attrs.size();
}

size_t VertexTemplate::get_elem_offset(int i_elem) const NOEXCEPT
{
    return m_impl->elem_offsets[i_elem];
}

size_t VertexTemplate::get_elem_offset(int i_attr, int i_elem_in_attr) const NOEXCEPT
{
    const HostVertexAttrib attr = m_impl->attrs.getReference(i_attr);
    return attr.offset + attr.get_elem_offset(i_elem_in_attr);
}

GLenum VertexTemplate::get_elem_type(int i_elem) const NOEXCEPT
{
    int i_attr = m_impl->elem_attr_index[i_elem];
    return m_impl->attrs.getReference(i_attr).type;
}

GLenum VertexTemplate::get_elem_size(int i_elem) const NOEXCEPT
{
    return size_of_gl_type(get_elem_type(i_elem));
}

const HostVertexAttrib& VertexTemplate::get_attrib(int i_attr) const NOEXCEPT
{
    return m_impl->attrs.getReference(i_attr);
}

const HostVertexAttrib& VertexTemplate::get_elem_attrib(int i_elem) const NOEXCEPT
{
    int i_attr = m_impl->elem_attr_index[i_elem];
    return m_impl->attrs.getReference(i_attr);
}

TREEFACE_NAMESPACE_END
