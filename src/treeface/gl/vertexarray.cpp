#include "treeface/gl/program.h"
#include "treeface/gl/type.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/Logger.h>
#include <treejuce/StringArray.h>
#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

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
        Logger::outputDebugString("connect with attr "+attr.name+" at "+String(attr_idx)+", size "+String(attr.n_elem)+" offset "+String(uint64(attr.offset)));
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
        treejuce::Logger::writeToLog("ignore attibute \""+attr.name+"\" which do not exist in program");
    }
}

treejuce::Result VertexArray::build(const VertexIndexBuffer* buffer,
                                    ArrayRef<HostVertexAttrib> attribs,
                                    const Program* program)
{
    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_vtx);

    // calculate stride
    GLsizei stride = 0;
    for (int i = 0; i < attribs.size(); i++)
    {
        const HostVertexAttrib attr = attribs[i];
        if (attr.offset != stride)
            return Result::fail("attribute "+attr.name+" offset is "+String(attr.offset)+", but previously accumulated offset is "+String(stride));

        stride += attr.n_elem * size_of_gl_type(attr.type);
    }

    // set attribute binding for vertex array
    for (int i = 0; i < attribs.size(); i++)
    {
        _build_one_(attribs[i], stride, program);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return Result::ok();
}

treejuce::Result VertexArray::build_one(const VertexIndexBuffer* buffer,
                                        HostVertexAttrib attrib,
                                        GLsizei stride,
                                        const Program* program)
{
    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_vtx);

    _build_one_(attrib, stride, program);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return Result::ok();
}


TREEFACE_NAMESPACE_END
