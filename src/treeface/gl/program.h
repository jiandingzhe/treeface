#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/common.h"

#include "treejuce/Array.h"
#include "treejuce/HashMap.h"
#include "treejuce/Object.h"
#include "treejuce/Result.h"
#include "treejuce/String.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class VertexArray;

class Program: public treejuce::Object
{
    friend class VertexArray;

public:
    struct AttrInfo
    {
        treejuce::String name;
        GLsizei size;
        GLenum type;
    };

    Program();

    // invalidate copy and move
    Program(const Program& other) = delete;
    Program(Program&& other) = delete;
    Program& operator = (const Program& other) = delete;
    Program& operator = (Program&& other) = delete;

    virtual ~Program();

    /**
     * @brief create shader and program object, compile source, link
     * @param src_vert
     * @param src_geom
     * @param src_frag
     * @return
     */
    treejuce::Result init(const char* src_vert, const char* src_geom, const char* src_frag);

    inline void use()
    {
        glUseProgram(m_program);
    }

protected:
    treejuce::Result fetch_shader_error_log(GLuint shader);
    treejuce::Result fetch_program_error_log();

protected:
    unsigned int m_program = 0;
    unsigned int m_shader_vert = 0;
    unsigned int m_shader_geom = 0;
    unsigned int m_shader_frag = 0;

    treejuce::Array<AttrInfo> m_attr_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_attr_idx_by_name;

    treejuce::Array<AttrInfo> m_uni_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_uni_idx_by_name;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_PROGRAM_H
