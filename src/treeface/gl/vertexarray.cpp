#include "treeface/gl/program.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include "treejuce/StringArray.h"
#include "treejuce/Logger.h"

TREEFACE_NAMESPACE_BEGIN

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_array);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_array);
}

void VertexArray::connect_buffer_and_program(VertexIndexBuffer* buffer,
                                             GLsizei stride,
                                             const treejuce::Array<AttrDesc>& attr_desc,
                                             Program* program)
{
    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_vtx);

    for (const AttrDesc& desc : attr_desc)
    {
        if (program->m_attr_idx_by_name.contains(desc.name))
        {
            treejuce::uint16 attr_idx = program->m_attr_idx_by_name[desc.name];
            glEnableVertexAttribArray(attr_idx);
            glVertexAttribPointer(attr_idx, desc.size, desc.type, desc.normalize, stride, desc.offset);
        }
        else
        {
            treejuce::Logger::writeToLog("attempt to connect to attibute \""+desc.name+"\" which do not exist in program"); // TODO: raise a warn
        }
    }
}

TREEFACE_NAMESPACE_END
