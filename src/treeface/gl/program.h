#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/common.h"
#include "treeface/math.h"
#include "treeface/gl/sampler.h"

#include "treejuce/Array.h"
#include "treejuce/HashMap.h"
#include "treejuce/Result.h"
#include "treejuce/String.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

struct Program
{
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

    ~Program();

    /**
     * @brief create shader and program object, compile source, link
     * @param src_vert
     * @param src_geom
     * @param src_frag
     * @return
     */
    treejuce::Result init(const char* src_vert, const char* src_frag);

    inline void use() const NOEXCEPT
    {
        glUseProgram(m_program);
    }

    inline void unuse() const NOEXCEPT
    {
        glUseProgram(0);
    }

    inline void set_uniform(GLint uni, GLint value)
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_INT);
        glUniform1i(uni, value);
    }

    inline void set_uniform(GLint uni, GLuint value)
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_UNSIGNED_INT);
        glUniform1ui(uni, value);
    }

    inline void set_uniform(GLint uni, GLfloat value)
    {
        jassert(m_uni_info[uni].size == 1);
        glUniform1f(uni, value);
    }

    inline void set_uniform(GLint uni, const Sampler& sampler)
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_SAMPLER_2D ||
                m_uni_info[uni].type == GL_SAMPLER_3D ||
                m_uni_info[uni].type == GL_SAMPLER_CUBE ||
                m_uni_info[uni].type == GL_SAMPLER_2D_SHADOW ||
                m_uni_info[uni].type == GL_SAMPLER_2D_ARRAY ||
                m_uni_info[uni].type == GL_SAMPLER_2D_ARRAY_SHADOW ||
                m_uni_info[uni].type == GL_SAMPLER_CUBE_SHADOW);
        glUniform1i(uni, sampler.m_sampler);
    }

    inline void set_uniform(GLint uni, const Vec4f& value)
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_FLOAT_VEC4);
        glUniform4fv(uni, 1, (const float*)&value);
    }

    inline void set_uniform(GLint uni, const Mat4f& value)
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_FLOAT_MAT4);
        glUniformMatrix4fv(uni, 1, false, (const float*) &value);
    }

    treejuce::Result fetch_shader_error_log(GLuint shader);
    treejuce::Result fetch_program_error_log();

    unsigned int m_program = 0;
    unsigned int m_shader_vert = 0;
    unsigned int m_shader_frag = 0;

    treejuce::Array<AttrInfo> m_attr_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_attr_idx_by_name;

    treejuce::Array<AttrInfo> m_uni_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_uni_idx_by_name;
};


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_PROGRAM_H
