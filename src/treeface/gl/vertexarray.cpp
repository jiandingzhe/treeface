#include "treeface/gl/program.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include <treejuce/Logger.h>
#include <treejuce/StringArray.h>
#include <treejuce/StringRef.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_array);
}

void VertexArray::init(const VertexIndexBuffer& buffer,
                       GLsizei stride,
                       const treejuce::Array<AttrDesc>& attr_desc,
                       const Program& program)
{
    glGenVertexArrays(1, &m_array);
    if (!m_array)
        die("failed to create vertex array object");

    glBindVertexArray(m_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.m_buffer_vtx);

    for (const AttrDesc& desc : attr_desc)
    {
        if (program.m_attr_idx_by_name.contains(desc.name))
        {
            treejuce::uint16 attr_idx = program.m_attr_idx_by_name[desc.name];
            Logger::outputDebugString("connect with attr "+desc.name+" at "+String(attr_idx)+", "+" size "+String(desc.size)+" offset "+String(uint64(desc.offset)));
            glEnableVertexAttribArray(attr_idx);
            glVertexAttribPointer(attr_idx, desc.size, desc.type, desc.normalize, stride, desc.offset);
        }
        else
        {
            treejuce::Logger::writeToLog("attempt to connect to attibute \""+desc.name+"\" which do not exist in program");
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TREEFACE_NAMESPACE_END
