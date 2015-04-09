#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/common.h"

#include <treejuce/Array.h>
#include <treejuce/Object.h>
#include <treejuce/String.h>

#include "GL/glew.h"

TREEFACE_NAMESPACE_BEGIN

struct VertexIndexBuffer;
struct Program;

struct VertexArray: public treejuce::Object
{
    struct AttrDesc
    {
        treejuce::String name;
        GLsizei offset;
        GLsizei n_elem;
        GLenum type;
        bool normalize;
    };

    VertexArray();
    virtual ~VertexArray();

    // disable copy and move
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&&) = delete;
    VertexArray& operator = (const VertexArray&) = delete;
    VertexArray& operator = (VertexArray&&) = delete;

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
