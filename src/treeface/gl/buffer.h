#ifndef TREEFACE_GL_BUFFER_H
#define TREEFACE_GL_BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <treecore/ArrayRef.h>
#include <treecore/MemoryBlock.h>
#include <treecore/RefCountObject.h>

namespace treeface
{

class Buffer: public treecore::RefCountObject
{
public:
    Buffer(GLenum type, GLenum policy);

    void use() const
    {
        glBindBuffer(m_type, m_buffer);
    }

    void unuse() const
    {
        glBindBuffer(m_type, 0);
    }

    template<typename T>
    void set_data(const treecore::ArrayRef<T>& data)
    {
        glBufferData(m_type, sizeof(T)*data.size(), data.get_data(), m_policy);
    }

    void set_data(const treecore::MemoryBlock& data)
    {
        glBufferData(m_type, data.getSize(), data.getData(), m_policy);
    }

protected:
    GLenum m_type = 0;
    GLenum m_policy = GL_STATIC_DRAW;
    GLuint m_buffer = 0;
};

} // namespace treeface


#endif // TREEFACE_GL_BUFFER_H
