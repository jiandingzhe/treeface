#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/common.h"

#include "treeface/gl/vertexattrib.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>

#include "GL/glew.h"

TREEFACE_NAMESPACE_BEGIN

struct VertexIndexBuffer;
struct Program;

struct VertexArray: public treejuce::Object
{
    VertexArray();
    virtual ~VertexArray();

    // disable copy and move
    JUCE_DECLARE_NON_COPYABLE(VertexArray);
    JUCE_DECLARE_NON_MOVABLE(VertexArray);

    /**
     * @brief build vertex array
     * @param buffer
     * @param attribs
     * @param program
     * @return
     */
    treejuce::Result build(const VertexIndexBuffer* buffer,
                           treejuce::ArrayRef<HostVertexAttrib> attribs,
                           const Program* program);

    
    treejuce::Result build_one(const VertexIndexBuffer* buffer,
                               HostVertexAttrib attrib,
                               GLsizei stride,
                               const Program* program);

    inline void use() const NOEXCEPT
    {
        glBindVertexArray(m_array);
    }

    inline void unuse() const NOEXCEPT
    {
        glBindVertexArray(0);
    }

    GLuint m_array = 0;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_VERTEX_ARRAY_H
