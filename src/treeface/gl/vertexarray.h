#ifndef TREEFACE_GL_VERTEX_ARRAY_H
#define TREEFACE_GL_VERTEX_ARRAY_H

#include "treeface/common.h"

#include "treejuce/Array.h"
#include "treejuce/String.h"
#include "treejuce/Object.h"

#include "GL/glew.h"

TREEFACE_NAMESPACE_BEGIN

class VertexIndexBuffer;
class Program;

class VertexArray: public treejuce::Object
{
public:
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

    void connect_buffer_and_program(VertexIndexBuffer* buffer,
                                    GLsizei stride,
                                    const treejuce::Array<AttrDesc>& attr_desc,
                                    Program* program);

    inline void bind() const
    {
        glBindVertexArray(m_array);
    }

private:
    GLuint m_array = 0;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_VERTEX_ARRAY_H
