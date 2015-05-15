#include "treeface/gl/program.h"
#include "treeface/gl/type.h"
#include "treeface/gl/vertexattrib.h"

#include "treeface/misc/stringcast.h"

#include <treejuce/Logger.h>
#include <treejuce/StringRef.h>

#include <cstdio>

using namespace std;
using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN


struct Program::Impl
{
    treejuce::Array<VertexAttrib>    attr_info;
    treejuce::HashMap<String, int32> attr_idx_by_name;

    treejuce::Array<UniformInfo>     uni_store;
    treejuce::HashMap<String, int32> uni_idx_by_name; // name => index
    treejuce::HashMap<GLint,  int32> uni_idx_by_loc;  // location => index
};

Program::Program(): m_impl(new Impl())
{
    m_program = glCreateProgram();
    if (m_program == 0)
        die("failed to allocate program ID");

    m_shader_vert = glCreateShader(GL_VERTEX_SHADER);
    if (!m_shader_vert)
        die("failed to allocate shader ID for vertex shader");

    m_shader_frag = glCreateShader(GL_FRAGMENT_SHADER);
    if (!m_shader_frag)
        die("failed to allocate shader ID for fragment shader");
}

Program::~Program()
{
    if (m_program)
        glDeleteProgram(m_program);

    if (m_shader_vert)
        glDeleteShader(m_shader_vert);

    if (m_shader_frag)
        glDeleteShader(m_shader_frag);

    if (m_impl)
        delete m_impl;
}

treejuce::Result Program::build(const char* src_vert_raw, const char* src_frag_raw)
{
    DBG("build program");
    if (compiled_and_linked)
        return Result::fail("attempt to build program which is already built");

    // compile shaders
    {
        glShaderSource(m_shader_vert, 1, &src_vert_raw, nullptr);
        glCompileShader(m_shader_vert);

        treejuce::Result re = fetch_shader_error_log(m_shader_vert);
        if (!re)
            return Result::fail("failed to compile vertex shader:\n" +
                                re.getErrorMessage() + "\n" +
                                "==== vertex shader source ====\n\n" +
                                String(src_vert_raw) + "\n" +
                                "==============================\n");

        glAttachShader(m_program, m_shader_vert);
    }

    {
        glShaderSource(m_shader_frag, 1, &src_frag_raw, nullptr);
        glCompileShader(m_shader_frag);

        treejuce::Result re = fetch_shader_error_log(m_shader_frag);
        if (!re)
            return Result::fail("failed to compile fragment shader:\n" +
                                re.getErrorMessage() + "\n" +
                                "==== fragment shader source ====\n\n" +
                                String(src_frag_raw) + "\n" +
                                "================================\n");

        glAttachShader(m_program, m_shader_frag);
    }

    // link program
    glLinkProgram(m_program);

    {
        treejuce::Result re = fetch_program_error_log();
        if (!re)
            return Result::fail("failed to link shader:\n" +
                                re.getErrorMessage() + "\n");
    }

    // extract program attributes
    int n_attr = -1;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &n_attr);
    if (n_attr == -1)
        die("failed to get attribute number in program %u", m_program);
    DBG("vertex attribute amount: "+String(n_attr));

    for (int i_attr = 0; i_attr < n_attr; i_attr++)
    {
        char attr_name[256];
        GLsizei attr_name_len = -1;
        GLenum attr_type = 0;
        GLsizei attr_size = -1;
        glGetActiveAttrib(m_program, i_attr, 256,
                          &attr_name_len, &attr_size, &attr_type, attr_name);

        DBG("  attribute "+String(i_attr)+": name "+String(attr_name)+", type "+gl_type_to_string(attr_type)+", size "+String(attr_size));

        if (attr_name_len == -1)
            die("failed to get info for attribute %d", i_attr);

        if (attr_name_len >= 255)
            die("attribute %d name is too long", i_attr);

        m_impl->attr_info.add({treejuce::String(attr_name), attr_size, attr_type});
        m_impl->attr_idx_by_name.set(treejuce::String(attr_name), i_attr);
    }

    // extract program uniforms
    int n_uni = -1;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &n_uni);
    if (n_uni == -1)
        die("failed to get uniform number from program %u", m_program);
    DBG("uniform amount: "+String(n_uni));

    for (int i_uni = 0; i_uni < n_uni; i_uni++)
    {
        // get uniform info
        char uni_name[256];
        GLsizei uni_name_len = -1;
        GLsizei uni_size = -1;
        GLenum uni_type = 0;
        glGetActiveUniform(m_program, i_uni, 256,
                           &uni_name_len, &uni_size, &uni_type, uni_name);

        if (uni_name_len == -1)
            die("failed to get info for uniform %d", i_uni);

        if (uni_name_len >= 255)
            die("uniform %d name is too long", i_uni);

        // get uniform location by name
        GLint uni_loc = glGetUniformLocation(m_program, uni_name);
        if (uni_loc == -1)
            die("failed to get location for uniform %s", uni_name);

        DBG("  uniform "+String(i_uni)+" "+String(uni_name)+" at " + String(uni_loc) + ": type "+gl_type_to_string(uni_type)+", size "+String(uni_size));

        // store uniform info
        m_impl->uni_store.add({uni_name, uni_size, uni_type, uni_loc});
        m_impl->uni_idx_by_name.set(uni_name, i_uni);
        m_impl->uni_idx_by_loc.set(uni_loc, i_uni);
    }

    compiled_and_linked = true;

    return treejuce::Result::ok();
}

void Program::use() NOEXCEPT
{
    glUseProgram(m_program);
}

treejuce::Result Program::fetch_shader_error_log(GLuint shader)
{
    GLint result = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == -1)
        die("failed to fetch compile status from shader %u\n", shader);

    if (result == GL_FALSE)
    {
        GLint log_len = -1;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        if (log_len > 0)
        {
            char* log = (char*) malloc(log_len+1);
            GLsizei written = 0;
            glGetShaderInfoLog(shader, log_len, &written, log);

            treejuce::Result re = treejuce::Result::fail(log);
            free(log);
            return re;
        }
        else
        {
            die("failed to retrieve shader compile error log");
        }
    }

    return treejuce::Result::ok();
}

treejuce::Result Program::fetch_program_error_log()
{
    GLint result = -1;
    glGetProgramiv(m_program, GL_LINK_STATUS, &result);

    if (result == -1)
        die("failed to fetch link status from program %u\n", m_program);

    if (result == GL_FALSE)
    {
        GLint log_len = -1;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_len);

        if (log_len > 0)
        {
            char* log = (char*) malloc(log_len + 1);
            GLsizei written = 0;
            glGetProgramInfoLog(m_program, log_len, &written, log);

            treejuce::Result re = treejuce::Result::fail(log);
            free(log);
            return re;
        }
        else
        {
            die("failed to retrieve program link error log");
        }
    }

    return treejuce::Result::ok();
}

void Program::instant_set_uniform(GLint uni_loc, GLint value) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_INT ||
            uni_info.type == GL_BOOL ||
            uni_info.type == GL_SAMPLER_2D ||
            uni_info.type == GL_SAMPLER_3D ||
            uni_info.type == GL_SAMPLER_CUBE ||
            uni_info.type == GL_SAMPLER_2D_SHADOW ||
            uni_info.type == GL_SAMPLER_2D_ARRAY ||
            uni_info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
            uni_info.type == GL_SAMPLER_CUBE_SHADOW);
#endif
    glUniform1i(uni_loc, value);
}

void Program::instant_set_uniform(GLint uni_loc, GLuint value) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_UNSIGNED_INT);
#endif
    glUniform1ui(uni_loc, value);
}

void Program::instant_set_uniform(GLint uni_loc, GLfloat value) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_FLOAT);
#endif
    glUniform1f(uni_loc, value);
}

void Program::instant_set_uniform(GLint uni_loc, const Sampler& sampler) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_SAMPLER_2D ||
            uni_info.type == GL_SAMPLER_3D ||
            uni_info.type == GL_SAMPLER_CUBE ||
            uni_info.type == GL_SAMPLER_2D_SHADOW ||
            uni_info.type == GL_SAMPLER_2D_ARRAY ||
            uni_info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
            uni_info.type == GL_SAMPLER_CUBE_SHADOW);
#endif
    glUniform1i(uni_loc, sampler.m_sampler);
}

void Program::instant_set_uniform(GLint uni_loc, const Vec4f& value) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_FLOAT_VEC4);
#endif
    glUniform4fv(uni_loc, 1, (const float*)&value);
}

void Program::instant_set_uniform(GLint uni_loc, const Mat4f& value) const NOEXCEPT
{
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index(uni_loc);
    jassert(i_uni >= 0);
    const UniformInfo& uni_info = m_impl->uni_store.getReference(i_uni);
    jassert(uni_info.n_elem == 1);
    jassert(uni_info.type == GL_FLOAT_MAT4);
#endif
    glUniformMatrix4fv(uni_loc, 1, false, (const float*) &value);
}

int Program::get_attribute_index(const treejuce::String& name) const NOEXCEPT
{
    if (m_impl->attr_idx_by_name.contains(name))
        return m_impl->attr_idx_by_name[name];
    else
        return -1;
}

const VertexAttrib& Program::get_attribute_info(int i_attr) const NOEXCEPT
{
    return m_impl->attr_info.getReference(i_attr);
}

int Program::get_uniform_index(const treejuce::String& name) const NOEXCEPT
{
    if (m_impl->uni_idx_by_name.contains(name))
        return m_impl->uni_idx_by_name[name];
    else
        return -1;
}

int Program::get_uniform_index(const GLint uni_loc) const NOEXCEPT
{
    if (m_impl->uni_idx_by_loc.contains(uni_loc))
        return m_impl->uni_idx_by_loc[uni_loc];
    else
        return -1;
}

const UniformInfo& Program::get_uniform_info(int i_uni) const NOEXCEPT
{
    return m_impl->uni_store.getReference(i_uni);
}

GLint Program::get_uniform_location(const treejuce::String& name) const NOEXCEPT
{
    if (m_impl->uni_idx_by_name.contains(name))
    {
        int index = m_impl->uni_idx_by_name[name];
        return m_impl->uni_store.getReference(index).location;
    }
    else
    {
        return -1;
    }
}

TREEFACE_NAMESPACE_END
