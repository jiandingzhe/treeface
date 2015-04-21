#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/common.h"
#include "treeface/math.h"
#include "treeface/gl/sampler.h"
#include "treeface/gl/vertexattrib.h"

#include <treejuce/Array.h>
#include <treejuce/HashMap.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class Material;
class VertexArray;

class Program: public treejuce::Object
{
    friend class Material;
    friend class VertexArray;

public:
    /**
     * @brief allocate shader and program IDs
     */
    Program();

    // invalidate copy and move
    JUCE_DECLARE_NON_COPYABLE(Program);
    JUCE_DECLARE_NON_MOVABLE(Program);

    virtual ~Program();

    /**
     * @brief create shader and program object, compile source, link
     * @param src_vert
     * @param src_geom
     * @param src_frag
     * @return
     */
    treejuce::Result build(const char* src_vert, const char* src_frag);

    inline bool usable() const NOEXCEPT
    {
        return compiled_and_linked;
    }

    inline void use() const NOEXCEPT
    {
        glUseProgram(m_program);
    }

    inline void unuse() const NOEXCEPT
    {
        glUseProgram(0);
    }

    inline void instant_set_uniform(GLint uni, GLint value) const NOEXCEPT
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_INT);
        glUniform1i(uni, value);
    }

    inline void instant_set_uniform(GLint uni, GLuint value) const NOEXCEPT
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_UNSIGNED_INT);
        glUniform1ui(uni, value);
    }

    inline void instant_set_uniform(GLint uni, GLfloat value) const NOEXCEPT
    {
        jassert(m_uni_info[uni].size == 1);
        glUniform1f(uni, value);
    }

    inline void instant_set_uniform(GLint uni, const Sampler& sampler) const NOEXCEPT
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

    inline void instant_set_uniform(GLint uni, const Vec4f& value) const NOEXCEPT
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_FLOAT_VEC4);
        glUniform4fv(uni, 1, (const float*)&value);
    }

    inline void instant_set_uniform(GLint uni, const Mat4f& value) const NOEXCEPT
    {
        jassert(m_uni_info[uni].size == 1);
        jassert(m_uni_info[uni].type == GL_FLOAT_MAT4);
        glUniformMatrix4fv(uni, 1, false, (const float*) &value);
    }

    /**
     * get attribute index by name
     *
     * @param name: attribute name
     * @return attribute index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_attribute_index_by_name(const treejuce::String& name) const NOEXCEPT;

    const VertexAttrib& get_attribute_info_by_index(int i_attr) const NOEXCEPT
    {
        return m_attr_info.getReference(i_attr);
    }

    /**
     * get uniform index by name
     *
     * @param name: uniform name
     * @return uniform index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_uniform_index_by_name(const treejuce::String& name) const NOEXCEPT;

    const VertexAttrib& get_uniform_info_by_index(int i_uni)
    {
        return m_uni_info.getReference(i_uni);
    }

protected:
    treejuce::Result fetch_shader_error_log(GLuint shader);
    treejuce::Result fetch_program_error_log();

    unsigned int m_program = 0;
    unsigned int m_shader_vert = 0;
    unsigned int m_shader_frag = 0;

    bool compiled_and_linked = false;

    treejuce::Array<VertexAttrib> m_attr_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_attr_idx_by_name;

    treejuce::Array<VertexAttrib> m_uni_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> m_uni_idx_by_name;
};


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_PROGRAM_H
