#include "treeface/gl/program.h"

#include "treejuce/StringRef.h"

#include <cstdio>

using namespace std;

TREEFACE_NAMESPACE_BEGIN

Program::Program() {}

Program::~Program()
{
    if (m_program)
        glDeleteProgram(m_program);

    if (m_shader_vert)
        glDeleteShader(m_shader_vert);

    if (m_shader_geom)
        glDeleteShader(m_shader_geom);

    if (m_shader_frag)
        glDeleteShader(m_shader_frag);
}

treejuce::Result Program::init(const char* src_vert, const char* src_geom, const char* src_frag)
{
    m_program = glCreateProgram();

    if (src_vert)
    {
        m_shader_vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_shader_vert, 1, &src_vert, nullptr);
        glCompileShader(m_shader_vert);

        treejuce::Result re = fetch_shader_error_log(GL_VERTEX_SHADER);
        if (!re)
            return re;

        glAttachShader(m_program, m_shader_vert);
    }

    if (src_geom)
    {
        m_shader_geom = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(m_shader_geom, 1, &src_geom, nullptr);
        glCompileShader(m_shader_geom);

        treejuce::Result re = fetch_shader_error_log(GL_GEOMETRY_SHADER);
        if (!re)
            return re;

        glAttachShader(m_program, m_shader_geom);
    }

    if (src_frag)
    {
        m_shader_frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_shader_frag, 1, &src_frag, nullptr);
        glCompileShader(m_shader_frag);

        treejuce::Result re = fetch_shader_error_log(GL_FRAGMENT_SHADER);
        if (!re)
            return re;

        glAttachShader(m_program, m_shader_frag);
    }

    glLinkProgram(m_program);

    {
        treejuce::Result re = fetch_program_error_log();
        if (!re)
            return re;
    }

    // extract program attributes
    int n_attr = -1;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &n_attr);

    if (n_attr == -1)
    {
        fprintf(stderr, "failed to get attribute number from program %u\n", m_program);
        abort();
    }

    for (int i_attr = 0; i_attr < n_attr; i_attr++)
    {
        char attr_name[256];
        GLsizei attr_name_len = -1;
        GLenum attr_type = 0;
        GLsizei attr_size = -1;
        glGetActiveAttrib(m_program, i_attr, 256,
                          &attr_name_len, &attr_size, &attr_type, attr_name);

        if (attr_name_len == -1)
        {
            fprintf(stderr, "failed to get attribute %d info\n", i_attr);
            abort();
        }

        if (attr_name_len >= 255)
        {
            fprintf(stderr, "attribute %d name is too long\n", i_attr);
            abort();
        }

        m_attr_info.add({treejuce::String(attr_name), attr_size, attr_type});
        m_attr_idx_by_name.set(treejuce::String(attr_name), i_attr);
    }

    // TODO extract program uniforms
    int n_uni = -1;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &n_uni);

    if (n_uni == -1)
    {
        fprintf(stderr, "failed to get uniform number from program %u\n", m_program);
        abort();
    }

    for (int i_uni = 0; i_uni < n_uni; i_uni++)
    {
        char uni_name[256];
        GLsizei uni_name_len = -1;
        GLsizei uni_size = -1;
        GLenum uni_type = 0;
        glGetActiveUniform(m_program, i_uni, 256,
                           &uni_name_len, &uni_size, &uni_type, uni_name);

        if (uni_name_len == -1)
        {
            fprintf(stderr, "failed to get uniform %d info\n", i_uni);
            abort();
        }

        if (uni_name_len >= 255)
        {
            fprintf(stderr, "uniform %d name is too long\n", i_uni);
            abort();
        }

        m_uni_info.add({uni_name, uni_size, uni_type});
        m_uni_idx_by_name.set(uni_name, i_uni);
    }

    return treejuce::Result::ok();
}

treejuce::Result Program::fetch_shader_error_log(GLuint shader)
{
    GLint result = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

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
            fputs("failed to retrieve shader compile error log\n", stderr);
            abort();
        }
    }
    else
    {
        return treejuce::Result::ok();
    }
}

treejuce::Result Program::fetch_program_error_log()
{
    GLint result = -1;
    glGetProgramiv(m_program, GL_LINK_STATUS, &result);

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
            fputs("failed to retrieve program link error log\n", stderr);
            abort();
        }
    }
    else
    {
        return treejuce::Result::ok();
    }
}

TREEFACE_NAMESPACE_END
