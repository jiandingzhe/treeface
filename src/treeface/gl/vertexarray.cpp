#include "treeface/gl/glbuffer.h"
#include "treeface/gl/program.h"
#include "treeface/gl/type.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertextemplate.h"

#include <treecore/ArrayRef.h>
#include <treecore/Logger.h>
#include <treecore/StringArray.h>
#include <treecore/StringRef.h>
#include <treecore/Identifier.h>

using namespace treecore;

namespace treeface {

void _build_one_( HostVertexAttrib attr, GLsizei stride, Program* program )
{
    int attr_idx = program->get_attribute_index( attr.name );
    if (attr_idx != -1)
    {
        DBG( "  connect with attr " + attr.name.toString() + " at " + String( attr_idx ) + ", size " + String( attr.n_elem ) + " offset " + String( uint64( attr.offset ) ) );
        glEnableVertexAttribArray( attr_idx );
        glVertexAttribPointer( attr_idx,
                               attr.n_elem,
                               attr.type,
                               attr.normalize,
                               stride,
                               reinterpret_cast<void*>(attr.offset) );
    }
    else
    {
        treecore::Logger::writeToLog( "ignore attibute \"" + attr.name.toString() + "\" which do not exist in program" );
    }
}

VertexArray::VertexArray( GLBuffer* buffer_vtx,
                          GLBuffer* buffer_idx,
                          const VertexTemplate& vertex_info,
                          Program* program )
    : m_vtx_info( vertex_info )
    , m_program( program )
    , m_buf_vtx(buffer_vtx)
    , m_buf_idx(buffer_idx)
{
    glGenVertexArrays( 1, &m_array );
    jassert( m_array != 0 );

    bind();
    buffer_vtx->bind();
    buffer_idx->bind();

    // set attribute binding for vertex array
    DBG( "connect buffer with program" );
    GLsizei stride = vertex_info.vertex_size();
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
