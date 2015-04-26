#include "treeface/gl/type.h"
#include "treeface/gl/vertextemplate.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/stringcast.h"

#include <treejuce/Array.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

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

#define KEY_NAME "name"
#define KEY_N_ELEM "n_elem"
#define KEY_TYPE "type"
#define KEY_NORM "normalized"

Result _validate_attr_kv_(const NamedValueSet& kv)
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item(KEY_NAME, PropertyValidator::ITEM_SCALAR, true);
        validator->add_item(KEY_N_ELEM, PropertyValidator::ITEM_SCALAR, true);
        validator->add_item(KEY_TYPE, PropertyValidator::ITEM_SCALAR, true);
        validator->add_item(KEY_NORM, PropertyValidator::ITEM_SCALAR, false);
    }

    return validator->validate(kv);
}

treejuce::Result VertexTemplate::add_attribs(const treejuce::var& list_node)
{
    if (!list_node.isArray())
        return Result::fail("node is not an array");

    const Array<var>* attr_nodes = list_node.getArray();

    for (int i = 0; i < attr_nodes->size(); i++)
    {
        const var& attr_node = attr_nodes->getReference(i);
        if (!attr_node.isObject())
            return Result::fail("attrib node at "+String(i)+" is not a KV");

        const NamedValueSet& attr_kv = attr_node.getDynamicObject()->getProperties();
        {
            Result re = _validate_attr_kv_(attr_kv);
            if (!re)
                return Result::fail("attrib node at "+String(i)+" is invalid: "+re.getErrorMessage());
        }

        bool do_norm = false;
        if (attr_kv.contains(Identifier(KEY_NORM)))
            do_norm = bool(attr_kv[KEY_NORM]);

        GLenum type = gl_type_from_string(attr_kv[KEY_TYPE].toString());
        add_attrib(VertexAttrib(attr_kv[KEY_NAME].toString(),
                                int(attr_kv[KEY_N_ELEM]),
                                type
                                ),
                   do_norm);
    }

    return Result::ok();
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

void VertexTemplate::set_value_at(void* vertex, int i_elem, const treejuce::var& value) NOEXCEPT
{
    size_t offset = get_elem_offset(i_elem);
    char* value_p = (char*) vertex;
    value_p += offset;

    GLenum type = get_elem_attrib(i_elem).type;
    switch (type)
    {
    case GL_BYTE:
        *((GLbyte*)value_p)   = int(value); break;
    case GL_UNSIGNED_BYTE:
        *((GLubyte*)value_p)  = int(value); break;
    case GL_SHORT:
        *((GLshort*)value_p)  = int(value); break;
    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_SHORT_5_5_5_1: // TODO should we support composite type?
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
        *((GLushort*)value_p) = int(value); break;
    case GL_INT:
        *((GLint*)value_p)    = int(value); break;
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_8_8_8_8: // TODO should we support composite type?
    case GL_UNSIGNED_INT_10_10_10_2:
        *((GLuint*)value_p)   = int64(value); break;
    case GL_FLOAT:
        *((GLfloat*)value_p)  = float(value); break;
    case GL_DOUBLE:
        *((GLdouble*)value_p) = double(value); break;
    default:
        die("vertex template got unsupported GL type enum %x", type);
    }
}

TREEFACE_NAMESPACE_END
