#include "treeface/gl/Errors.h"
#include "treeface/gl/TypeUtils.h"
#include "treeface/gl/VertexTemplate.h"

#include "treeface/misc/Errors.h"
#include "treeface/misc/PropertyValidator.h"
#include "treeface/misc/StringCast.h"

#include <treecore/Array.h>
#include <treecore/DynamicObject.h>
#include <treecore/NamedValueSet.h>
#include <treecore/Result.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

inline GLsizei _expand_to_align_( GLsizei value, GLsizei align )
{
    if (align == 0)
        return value;

    GLsizei rem = value % align;
    if (rem > 0)
        return value - rem + align;
    else
        return value;
}

struct VertexTemplate::Impl
{
    treecore::Array<TypedTemplateWithOffset> attrs;
    treecore::Array<size_t> elem_offsets;
    treecore::Array<int>    elem_attr_index;
    uint32 size = 0;
};

VertexTemplate::VertexTemplate(): m_impl( new Impl() )
{}

#define KEY_NAME   "name"
#define KEY_N_ELEM "n_elem"
#define KEY_TYPE   "type"
#define KEY_NORM   "normalize"
#define KEY_ALN    "align"

Result _validate_attr_kv_( const NamedValueSet& kv )
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item( KEY_NAME,   PropertyValidator::ITEM_SCALAR, true );
        validator->add_item( KEY_N_ELEM, PropertyValidator::ITEM_SCALAR, true );
        validator->add_item( KEY_TYPE,   PropertyValidator::ITEM_SCALAR, true );
        validator->add_item( KEY_NORM,   PropertyValidator::ITEM_SCALAR, false );
        validator->add_item( KEY_ALN,    PropertyValidator::ITEM_SCALAR, false );
    }

    return validator->validate( kv );
}

VertexTemplate::VertexTemplate( const treecore::var& list_node ): m_impl( new Impl() )
{
    if ( !list_node.isArray() )
        throw ConfigParseError( "node is not an array" );

    const Array<var>* attr_nodes = list_node.getArray();

    for (int i = 0; i < attr_nodes->size(); i++)
    {
        const var& attr_node = (*attr_nodes)[i];
        if ( !attr_node.isObject() )
            throw ConfigParseError( "attrib node at " + String( i ) + " is not a KV" );

        const NamedValueSet& attr_kv = attr_node.getDynamicObject()->getProperties();
        {
            Result re = _validate_attr_kv_( attr_kv );
            if (!re)
                throw ConfigParseError( "attrib node at " + String( i ) + " is invalid: " + re.getErrorMessage() );
        }

        bool do_norm = false;
        if ( attr_kv.contains( Identifier( KEY_NORM ) ) )
            do_norm = bool(attr_kv[KEY_NORM]);

        GLType type;
        if ( !fromString( attr_kv[KEY_TYPE], type ) )
            throw ConfigParseError( "failed to parse OpenGL type from: " + attr_kv[KEY_TYPE].toString() );

        int32 n_elem;
        if ( !fromString( attr_kv[KEY_N_ELEM], n_elem ) )
            throw ConfigParseError( "failed to parse number of elements from: " + attr_kv[KEY_N_ELEM].toString() );

        TypedTemplate attr_kern( attr_kv[KEY_NAME].toString(), n_elem, type );

        uint32 align = size_of_gl_type( type );
        if ( attr_kv.contains( KEY_ALN ) && !fromString( attr_kv[KEY_ALN], align ) )
            throw ConfigParseError( "failed to parse attribute alignment from: " + attr_kv[KEY_N_ELEM].toString() );

        add_attrib( attr_kern, do_norm, align );
    }

}

VertexTemplate::VertexTemplate( const VertexTemplate& other ): m_impl( new Impl() )
{
    *m_impl = *other.m_impl;
}

VertexTemplate& VertexTemplate::operator =( const VertexTemplate& other )
{
    *m_impl = *other.m_impl;
    return *this;
}

VertexTemplate::~VertexTemplate()
{
    if (m_impl)
        delete m_impl;
}

void VertexTemplate::add_attrib( const TypedTemplate& attr, bool normalize, uint32 align )
{
    treecore_assert( align <= attr.size() );

    size_t attr_offset = _expand_to_align_( m_impl->size, align );
    int    prev_n_attr = m_impl->attrs.size();
    m_impl->attrs.add( TypedTemplateWithOffset( attr, attr_offset, normalize ) );

    size_t elem_offset = attr_offset;
    int    elem_size   = size_of_gl_type( attr.type );

    for (int i = 0; i < attr.n_elem; i++)
    {
        m_impl->elem_offsets.add( elem_offset );
        m_impl->elem_attr_index.add( prev_n_attr );
        elem_offset += elem_size;
    }

    // update vertex size
    m_impl->size = attr_offset + _expand_to_align_( attr.size(), align );
}

size_t VertexTemplate::vertex_size() const noexcept
{
    return m_impl->size;
}

int VertexTemplate::n_elems() const noexcept
{
    return m_impl->elem_offsets.size();
}

int VertexTemplate::n_attribs() const noexcept
{
    return m_impl->attrs.size();
}

size_t VertexTemplate::get_elem_offset( int i_elem ) const noexcept
{
    return m_impl->elem_offsets[i_elem];
}

size_t VertexTemplate::get_elem_offset( int i_attr, int i_elem_in_attr ) const noexcept
{
    const TypedTemplateWithOffset& attr = m_impl->attrs[i_attr];
    return attr.offset + attr.get_elem_offset( i_elem_in_attr );
}

GLType VertexTemplate::get_elem_type( int i_elem ) const noexcept
{
    int i_attr = m_impl->elem_attr_index[i_elem];
    return m_impl->attrs[i_attr].type;
}

int32 VertexTemplate::get_elem_size( int i_elem ) const noexcept
{
    return size_of_gl_type( get_elem_type( i_elem ) );
}

const TypedTemplateWithOffset& VertexTemplate::get_attrib( int i_attr ) const noexcept
{
    return m_impl->attrs[i_attr];
}

const TypedTemplateWithOffset& VertexTemplate::get_elem_attrib( int i_elem ) const noexcept
{
    int i_attr = m_impl->elem_attr_index[i_elem];
    return m_impl->attrs[i_attr];
}

void VertexTemplate::set_value_at( void* vertex, int i_elem, const treecore::var& value ) const noexcept
{
    size_t offset  = get_elem_offset( i_elem );
    char*  value_p = (char*) vertex;
    value_p += offset;

    GLenum type = get_elem_attrib( i_elem ).type;
    switch (type)
    {
    case GL_BYTE:
        *( (GLbyte*) value_p ) = GLbyte( int(value) ); break;
    case GL_UNSIGNED_BYTE:
        *( (GLubyte*) value_p ) = GLubyte( int(value) ); break;
    case GL_SHORT:
        *( (GLshort*) value_p ) = GLshort( int(value) ); break;
    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_SHORT_5_5_5_1: // TODO should we support composite type?
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
        *( (GLushort*) value_p ) = GLushort( int(value) ); break;
    case GL_INT:
        *( (GLint*) value_p ) = GLint( int(value) ); break;
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_8_8_8_8: // TODO should we support composite type?
    case GL_UNSIGNED_INT_10_10_10_2:
        *( (GLuint*) value_p ) = GLuint( int(value) ); break;
    case GL_FLOAT:
        *( (GLfloat*) value_p ) = float(value); break;
    case GL_DOUBLE:
        *( (GLdouble*) value_p ) = double(value); break;
    default:
        die( "vertex template got unsupported GL type enum %x", type );
    }
}

} // namespace treeface
