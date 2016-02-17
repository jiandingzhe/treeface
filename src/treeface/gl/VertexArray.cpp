#include "treeface/gl/GLBuffer.h"
#include "treeface/gl/Program.h"
#include "treeface/gl/TypeUtils.h"
#include "treeface/gl/VertexArray.h"
#include "treeface/gl/VertexTemplate.h"

#include "treeface/misc/StringCast.h"
#include "treeface/misc/TypedTemplateWithOffset.h"

#include <treecore/ArrayRef.h>
#include <treecore/Logger.h>
#include <treecore/StringArray.h>
#include <treecore/StringRef.h>
#include <treecore/Identifier.h>

using namespace treecore;

namespace treeface {

void _build_one_( const TypedTemplateWithOffset& host_attr, GLsizei stride, Program* program )
{
    int prog_attr_idx = program->get_attribute_index( host_attr.name );
    if (prog_attr_idx < 0)
    {
        DBG( "  skipped attibute \"" + host_attr.name.toString() + "\" which do not exist in program" );
        return;
    }
    const TypedTemplateWithLocation& prog_attr = program->get_attribute( prog_attr_idx );

    DBG( "  connect with attr " + host_attr.name.toString() + " at " + String( prog_attr.location ) + ", size " + String( host_attr.n_elem ) + " offset " + String( uint64( host_attr.offset ) ) );
    glEnableVertexAttribArray( prog_attr.location );
    glVertexAttribPointer( prog_attr.location,
                           host_attr.n_elem,
                           host_attr.type,
                           host_attr.normalize,
                           stride,
                           reinterpret_cast<void*>(host_attr.offset) );

}

VertexArray::VertexArray( GLBuffer* buffer_vtx,
                          GLBuffer* buffer_idx,
                          const VertexTemplate& vertex_info,
                          Program* program )
    : m_vtx_info( vertex_info )
    , m_program( program )
    , m_buf_vtx( buffer_vtx )
    , m_buf_idx( buffer_idx )
{
    glGenVertexArrays( 1, &m_array );
    jassert( m_array != 0 );

    bind();
    buffer_vtx->bind();
    buffer_idx->bind();

    // set attribute binding for vertex array
    DBG( "connect buffer with program" );
    GLsizei stride = (GLsizei) vertex_info.vertex_size();
    for (int i = 0; i < vertex_info.n_attribs(); i++)
    {
        _build_one_( vertex_info.get_attrib( i ), stride, program );
    }

    unbind();
    buffer_vtx->unbind();
    buffer_idx->unbind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays( 1, &m_array );
}

GLuint VertexArray::get_current_bound_vertex_array()
{
    GLint result = -1;
    glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &result );
    return GLuint( result );
}

} // namespace treeface
