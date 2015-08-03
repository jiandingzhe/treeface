#include "treeface/gl/program.h"
#include "treeface/gl/type.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertextemplate.h"

#include <treecore/ArrayRef.h>
#include <treecore/Logger.h>
#include <treecore/StringArray.h>
#include <treecore/StringRef.h>

using namespace treecore;

namespace treeface {

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_array);
    if (!m_array)
        die("failed to create vertex array object");
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_array);
}

void _build_one_(HostVertexAttrib attr,
                 GLsizei stride,
                 const Program* program)
{
    int attr_idx = program->get_attribute_index(attr.name);
    if (attr_idx != -1)
    {
        DBG("  connect with attr "+attr.name+" at "+String(attr_idx)+", size "+String(attr.n_elem)+" offset "+String(uint64(attr.offset)));
        glEnableVertexAttribArray(attr_idx);
        glVertexAttribPointer(attr_idx,
                              attr.n_elem,
                              attr.type,
                              attr.normalize,
                              stride,
                              reinterpret_cast<void*>(attr.offset));
    }
    else
    {
        treecore::Logger::writeToLog("ignore attibute \""+attr.name+"\" which do not exist in program");
    }
}

treecore::Result VertexArray::build(const VertexIndexBuffer* buffer,
                                    const VertexTemplate& vertex_info,
                                    const Program* program)
{
    if (!program->usable())
        return Result::fail("VertexArray: attempt to connect with unbuilt program object");

    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_vtx);

    // set attribute binding for vertex array
    DBG("connect buffer with program");
    GLsizei stride = vertex_info.vertex_size();
    for (int i = 0; i < vertex_info.n_attribs(); i++)
    {
        _build_one_(vertex_info.get_attrib(i), stride, program);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return Result::ok();
}

treecore::Result VertexArray::build_one(const VertexIndexBuffer* buffer,
                                        HostVertexAttrib attrib,
                                        GLsizei stride,
                                        const Program* program)
{
    if (!program->usable())
        return Result::fail("VertexArray: attempt to connect with unbuilt program object");

    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_vtx);

    _build_one_(attrib, stride, program);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return Result::ok();
}


} // namespace treeface
