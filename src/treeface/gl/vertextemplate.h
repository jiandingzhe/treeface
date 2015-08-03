#ifndef TREEFACE_VERTEX_TEMPLATE_H
#define TREEFACE_VERTEX_TEMPLATE_H

#include "treeface/common.h"
#include "treeface/gl/vertexattrib.h"

#include <treecore/IntTypes.h>

namespace treecore {
class var;
class Result;
} // namespace treecore

namespace treeface {

/**
 * @brief The VertexTemplate class holds vertex attribute meta data for vertex
 * buffers.
 */
class VertexTemplate
{
public:
    /**
     * @brief create an empty VertexTemplate object.
     */
    VertexTemplate();

    /**
     * @brief move constructor.
     * @param other
     */
    VertexTemplate(VertexTemplate&& other): m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    /**
     * @brief move assignment operator.
     * @param other
     * @return
     */
    VertexTemplate& operator = (VertexTemplate&& other)
    {
        jassert(m_impl == nullptr);

        m_impl = other.m_impl;
        other.m_impl = nullptr;

        return *this;
    }

    JUCE_DECLARE_NON_COPYABLE(VertexTemplate);

    /**
     * @brief add one vertex attribute into vertex template.
     *
     * @param attr: the attribute to be added.
     *
     * @param normalize: whether this attribute should be normalized when
     *        sending to GL device side.
     */
    void add_attrib(const VertexAttrib& attr, bool normalize);

    /**
     * @brief add several attributes from parsed JSON content.
     *
     * @param list_node: a JSON node which should be an array of attributes.
     *
     * @return ok if success, fail if any error occurred.
     */
    treecore::Result add_attribs(const treecore::var& list_node);

    /**
     * @brief get the byte size of a single vertex
     * @return vertex size in bytes.
     */
    size_t vertex_size() const NOEXCEPT;

    /**
     * @brief get the number of elements of all vertex attributes for a single
     *        vertex.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes, then n_elems() will give you 3+2+4 = 9.
     *
     * @return the total number of elements.
     */
    int n_elems() const NOEXCEPT;

    /**
     * @brief get the number of vertex attributes for a single vertex.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes, then n_attribs() will give you 3.
     *
     * @return the number of vertex attributes.
     */
    int n_attribs() const NOEXCEPT;

    /**
     * @brief get the offset in bytes for element at specified index.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes, then the offset of 3th element will be 3*sizeof(float),
     * the offset of 8th element will be 3*sizeof(float) + 2*sizeof(float) +
     * 3*sizeof(char) = 23.
     *
     * @param i_elem: the element index.
     *
     * @return the byte offset of specified element.
     */
    size_t get_elem_offset(int i_elem) const NOEXCEPT;

    /**
     * @brief get the offset in bytes for element at specified index.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes. Then the offset of element 0 of attribute 1 will be
     *  3*sizeof(float), the offset of element 3 of attribute 2 will be
     * 3*sizeof(float) + 2*sizeof(float) + 3*sizeof(char) = 23.
     *
     * @param i_attr: the vertex attribute index.
     * @param i_elem_in_attr: the element index inside specified vertex
     *        attribute.
     *
     * @return the byte offset of specified element.
     */
    size_t get_elem_offset(int i_attr, int i_elem_in_attr) const NOEXCEPT;

    /**
     * @brief get type of the element at specified index.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes. Then the type of element 0 is GL_FLOAT, and the type of
     * element 8 is GL_BYTE.
     *
     * @param i_elem: the element index.
     *
     * @return the OpenGL type enum of specified element.
     */
    GLenum get_elem_type(int i_elem) const NOEXCEPT;

    /**
     * @brief get byte size of the element at specified index.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes. Then the size of element 0 is sizeof(float), and the size
     * of element 8 is sizeof(char).
     *
     * @param i_elem: the element index.
     *
     * @return the byte size of specified element.
     */
    GLenum get_elem_size(int i_elem) const NOEXCEPT;

    /**
     * @brief get specified vertex attribute.
     *
     * @param i_attr: index of wanted vertex attribute.
     *
     * @return vertex attribute metadata.
     */
    const HostVertexAttrib& get_attrib(int i_attr) const NOEXCEPT;

    /**
     * @brief get vertex attribute of the specified element.
     *
     * @param i_elem: index element, whose vertex attribute will be retrieved.
     *
     * @return vertex attribute metadata.
     */
    const HostVertexAttrib& get_elem_attrib(int i_elem) const NOEXCEPT;

    /**
     * @brief set value of specified element
     *
     * @param vertex: the address of host-side vertex data. It should be the
     *        beginning of the specified one vertex, not whole vertex buffer.
     * @param i_elem: the element index.
     * @param value: the value to be set.
     */
    void set_value_at(void* vertex, int i_elem, const treecore::var& value) NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_VERTEX_TEMPLATE_H
