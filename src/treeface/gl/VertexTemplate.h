#ifndef TREEFACE_VERTEX_TEMPLATE_H
#define TREEFACE_VERTEX_TEMPLATE_H

#include "treeface/base/Common.h"

#include "treeface/misc/TypedTemplate.h"
#include "treeface/misc/TypedTemplateWithOffset.h"

#include <treecore/IntTypes.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class var;
class Result;
} // namespace treecore

namespace treeface {

/**
 * @brief holds vertex attribute meta data for vertex buffers
 *
 * GPU buffers are bare byte data, so we need meta data to describe how vertices
 * organize.
 */
class VertexTemplate: public treecore::RefCountObject
{
public:
    ///
    /// \brief create an empty VertexTemplate object.
    ///
    VertexTemplate();

    ///
    /// \brief create VertexTemplate from config information
    /// \param list_node
    ///
    VertexTemplate(const treecore::var& list_node);

    VertexTemplate(const VertexTemplate& other);
    VertexTemplate& operator = (const VertexTemplate& other);

    TREECORE_DECLARE_NON_MOVABLE(VertexTemplate);

    virtual ~VertexTemplate();

    ///
    /// \brief add one vertex attribute into vertex template.
    ///
    /// \param attr       the attribute to be added.
    /// \param normalize  whether this attribute should be normalized when sent
    ///                   to GL device side. Only affects integral types.
    /// \param align      attribute alignment
    ///
    void add_attrib(const TypedTemplate& attr, bool normalize, treecore::uint32 align);

    /**
     * @brief get the byte size of a single vertex
     * @return vertex size in bytes.
     */
    size_t vertex_size() const noexcept;

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
    int n_elems() const noexcept;

    /**
     * @brief get the number of vertex attributes for a single vertex.
     *
     * For example, a vertex have an attribute "position" of three floats, and
     * an attribute "tex_coord" of two floats, and yet another attribute "color"
     * of four bytes, then n_attribs() will give you 3.
     *
     * @return the number of vertex attributes.
     */
    int n_attribs() const noexcept;

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
    size_t get_elem_offset(int i_elem) const noexcept;

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
    size_t get_elem_offset(int i_attr, int i_elem_in_attr) const noexcept;

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
    GLType get_elem_type(int i_elem) const noexcept;

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
    treecore::int32 get_elem_size(int i_elem) const noexcept;

    /**
     * @brief get specified vertex attribute.
     *
     * @param i_attr: index of wanted vertex attribute.
     *
     * @return vertex attribute metadata.
     */
    const TypedTemplateWithOffset& get_attrib(int i_attr) const noexcept;

    /**
     * @brief get vertex attribute of the specified element.
     *
     * @param i_elem: index element, whose vertex attribute will be retrieved.
     *
     * @return vertex attribute metadata.
     */
    const TypedTemplateWithOffset& get_elem_attrib(int i_elem) const noexcept;

    /**
     * @brief set value of specified element
     *
     * @param vertex: the address of host-side vertex data. It should be the
     *        beginning of the specified one vertex, not whole vertex buffer.
     * @param i_elem: the element index.
     * @param value: the value to be set.
     */
    void set_value_at(void* vertex, int i_elem, const treecore::var& value) const noexcept;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_VERTEX_TEMPLATE_H
