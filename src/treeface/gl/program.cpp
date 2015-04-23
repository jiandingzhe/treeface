#include "treeface/gl/program.h"
#include "treeface/gl/type.h"
#include "treeface/gl/vertexattrib.h"

#include "treeface/stringcast.h"

#include <treejuce/Logger.h>
#include <treejuce/StringRef.h>

#include <cstdio>

using namespace std;
using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

typedef void (*upload_func_t)(GLint uni, const void* data);

void upload_bool(GLint uni, const void* data)
{
    GLboolean value = *((GLboolean*) &data);
    glUniform1i(uni, value);
}

void upload_int(GLint uni, const void* data)
{
    GLint value = *((GLint*) &data);
    glUniform1i(uni, value);
}

void upload_uint(GLint uni, const void* data)
{
    GLuint value = *((GLuint*) &data);
    glUniform1ui(uni, value);
}

void upload_float(GLint uni, const void* data)
{
    float value = *((float*) &data);
    glUniform1f(uni, value);
}

void upload_vec4f(GLint uni, const void* data)
{
    glUniform4fv(uni, 1, (float*)data);
}

void upload_mat4f(GLint uni, const void* data)
{
    glUniformMatrix4fv(uni, 1, false, (float*)data);
}

struct Uniform
{
    Uniform(const treejuce::String& name, GLsizei n_elem, GLenum type);
    ~Uniform();

    Uniform(Uniform&& other):
        info(other.info),
        changed(other.changed),
        size(other.size),
        store(other.store),
        uploader(other.uploader)
    {
        other.store = nullptr;
    }

    Uniform& operator = (Uniform&& other)
    {
        info = other.info;
        changed = other.changed;
        size = other.size;
        store = other.store;
        uploader = other.uploader;

        other.store = nullptr;

        return *this;
    }

    JUCE_DECLARE_NON_COPYABLE(Uniform);

    inline void set(bool value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_BOOL);
        store = reinterpret_cast<void*>(value);
        changed = true;
    }

    inline void set(GLint value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_BOOL ||
                info.type == GL_INT ||
                info.type == GL_SAMPLER_2D ||
                info.type == GL_SAMPLER_3D ||
                info.type == GL_SAMPLER_CUBE ||
                info.type == GL_SAMPLER_2D_SHADOW ||
                info.type == GL_SAMPLER_2D_ARRAY ||
                info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
                info.type == GL_SAMPLER_CUBE_SHADOW);
        store = reinterpret_cast<void*>(value);
        changed = true;
    }

    inline void set(GLuint value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_UNSIGNED_INT);
        store = reinterpret_cast<void*>(value);
        changed = true;
    }

    inline void set(float value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_FLOAT);
        treejuce::int32* value_p = (treejuce::int32*) &value;
        store = reinterpret_cast<void*>(*value_p);
        changed = true;
    }

    inline void set(const Sampler& value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_SAMPLER_2D ||
                info.type == GL_SAMPLER_3D ||
                info.type == GL_SAMPLER_CUBE ||
                info.type == GL_SAMPLER_2D_SHADOW ||
                info.type == GL_SAMPLER_2D_ARRAY ||
                info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
                info.type == GL_SAMPLER_CUBE_SHADOW);
        store = reinterpret_cast<void*>(value.m_sampler);
        changed = true;
    }

    inline void set(const Vec4f& value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_FLOAT_VEC4);
        memcpy(store, &value, sizeof(Vec4f));
        changed = true;
    }

    inline void set(const Mat4f& value) NOEXCEPT
    {
        jassert(info.n_elem == 1);
        jassert(info.type == GL_FLOAT_MAT4);
        memcpy(store, &value, sizeof(Mat4f));
        changed = true;
    }

    inline void upload_data(GLint uni_idx)
    {
        if (changed)
        {
            uploader(uni_idx, store);
            changed = false;
        }
    }

    VertexAttrib info;
    bool changed           = false;
    treejuce::uint32 size  = 0;
    void* store            = nullptr;
    upload_func_t uploader = nullptr;
};


Uniform::Uniform(const treejuce::String& name, GLsizei n_elem, GLenum type)
    : info({name, n_elem, type})
    , size(n_elem * size_of_gl_type(type))
{
    switch(type)
    {
    case GL_BOOL:
        if (n_elem != 1)
            die("uniform bool %d array not supported", n_elem);
        uploader = upload_bool;
        break;
    case GL_INT:
        if (n_elem != 1)
            die("uniform int %d array not supported", n_elem);
        uploader = upload_int;
        break;
    case GL_UNSIGNED_INT:
        if (n_elem != 1)
            die("uniform uint %d array not supported", n_elem);
        uploader = upload_uint;
        break;
    case GL_FLOAT:
        if (n_elem != 1)
            die("uniform float %d array not supported", n_elem);
        uploader = upload_float;
        break;
    case GL_FLOAT_VEC4:
        if (n_elem != 1)
            die("uniform vec4 %d array not supported", n_elem);
        uploader = upload_float;
        break;
    case GL_FLOAT_MAT4:
        if (n_elem != 1)
            die("uniform mat4 %d array not supported", n_elem);
        uploader = upload_mat4f;
        break;
    case GL_SAMPLER_2D:
        if (n_elem != 1)
            die("number of sampler uniform not one");
        uploader = upload_int;
        break;
    default:
        die("unsupported program uniform type: " + gl_type_to_string(type));
    }

    if (size > sizeof(void*))
    {
        store = malloc(size);
    }
}

Uniform::~Uniform()
{
    if(size > sizeof(void*) && store != nullptr)
        free(store);
}

struct Program::Impl
{
    treejuce::Array<VertexAttrib> attr_info;
    treejuce::HashMap<treejuce::String, treejuce::uint16> attr_idx_by_name;

    bool uni_changed = false;
    treejuce::Array<Uniform> uni_store;
    treejuce::HashMap<treejuce::String, treejuce::uint16> uni_idx_by_name;
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

treejuce::Result Program::build(const char* src_vert, const char* src_frag)
{
    if (compiled_and_linked)
        return Result::fail("attempt to build program which is already built");

    // compile shaders
    if (src_vert)
    {
        glShaderSource(m_shader_vert, 1, &src_vert, nullptr);
        glCompileShader(m_shader_vert);

        treejuce::Result re = fetch_shader_error_log(m_shader_vert);
        if (!re)
            return re;

        glAttachShader(m_program, m_shader_vert);
    }

    if (src_frag)
    {
        glShaderSource(m_shader_frag, 1, &src_frag, nullptr);
        glCompileShader(m_shader_frag);

        treejuce::Result re = fetch_shader_error_log(m_shader_frag);
        if (!re)
            return re;

        glAttachShader(m_program, m_shader_frag);
    }

    // link program
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
        die("failed to get attribute number in program " + String(m_program));

    for (int i_attr = 0; i_attr < n_attr; i_attr++)
    {
        char attr_name[256];
        GLsizei attr_name_len = -1;
        GLenum attr_type = 0;
        GLsizei attr_size = -1;
        glGetActiveAttrib(m_program, i_attr, 256,
                          &attr_name_len, &attr_size, &attr_type, attr_name);

        if (attr_name_len == -1)
            die("failed to get info for attribute " + String(i_attr));

        if (attr_name_len >= 255)
            die("attribute " + String(i_attr) + " name is too long");

        m_impl->attr_info.add({treejuce::String(attr_name), attr_size, attr_type});
        m_impl->attr_idx_by_name.set(treejuce::String(attr_name), i_attr);
    }

    // extract program uniforms
    int n_uni = -1;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &n_uni);

    if (n_uni == -1)
        die("failed to get uniform number from program %u", m_program);

    for (int i_uni = 0; i_uni < n_uni; i_uni++)
    {
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

        m_impl->uni_store.add({uni_name, uni_size, uni_type});
        m_impl->uni_idx_by_name.set(uni_name, i_uni);
    }

    return treejuce::Result::ok();
}

void Program::use() NOEXCEPT
{
    glUseProgram(m_program);

    if (m_impl->uni_changed)
    {
        for (int i_uni = 0; i_uni < m_impl->uni_store.size(); i_uni++)
        {
            m_impl->uni_store.getReference(i_uni).upload_data(i_uni);
        }
        m_impl->uni_changed = false;
    }
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
    else
    {
        return treejuce::Result::ok();
    }
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
    else
    {
        return treejuce::Result::ok();
    }
}

void Program::instant_set_uniform(GLint uni, GLint value) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_INT ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_3D ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_CUBE ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_SHADOW ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_ARRAY ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_CUBE_SHADOW);
    printf("upload int %d to uniform %d\n", value, uni);
    glUniform1i(uni, value);
}

void Program::instant_set_uniform(GLint uni, GLuint value) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_UNSIGNED_INT);
    glUniform1ui(uni, value);
}

void Program::instant_set_uniform(GLint uni, GLfloat value) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_FLOAT);
    glUniform1f(uni, value);
}

void Program::instant_set_uniform(GLint uni, const Sampler& sampler) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_3D ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_CUBE ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_SHADOW ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_ARRAY ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
            m_impl->uni_store.getReference(uni).info.type == GL_SAMPLER_CUBE_SHADOW);
    glUniform1i(uni, sampler.m_sampler);
}

void Program::instant_set_uniform(GLint uni, const Vec4f& value) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_FLOAT_VEC4);
    glUniform4fv(uni, 1, (const float*)&value);
}

void Program::instant_set_uniform(GLint uni, const Mat4f& value) const NOEXCEPT
{
    jassert(m_impl->uni_store.getReference(uni).size == 1);
    jassert(m_impl->uni_store.getReference(uni).info.type == GL_FLOAT_MAT4);
    glUniformMatrix4fv(uni, 1, false, (const float*) &value);
}

void Program::set_uniform(GLint i_uni, GLint value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
}

void Program::set_uniform(GLint i_uni, GLuint value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
}

void Program::set_uniform(GLint i_uni, float value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
}

void Program::set_uniform(GLint i_uni, const Sampler& value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
}

void Program::set_uniform(GLint i_uni, const Vec4f& value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
}

void Program::set_uniform(GLint i_uni, const Mat4f& value) NOEXCEPT
{
    m_impl->uni_store.getReference(i_uni).set(value);
    m_impl->uni_changed = true;
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

const VertexAttrib& Program::get_uniform_info(int i_uni) const NOEXCEPT
{
    return m_impl->uni_store.getReference(i_uni).info;
}

TREEFACE_NAMESPACE_END
