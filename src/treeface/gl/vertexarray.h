#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/common.h"

#include "treejuce/Array.h"
#include "treejuce/String.h"

#include "GL/glew.h"

TREEFACE_NAMESPACE_BEGIN

struct VertexIndexBuffer;
struct Program;

struct VertexArray
{
    struct AttrDesc
    {
        treejuce::String name;
        void* offset;
        GLsizei size;
        GLenum type;
        bool normalize;
    };

    VertexArray();

    virtual ~VertexArray();

    void init(const VertexIndexBuffer& buffer,
              GLsizei stride,
              const treejuce::Array<AttrDesc>& attr_desc,
              const Program& program);

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
