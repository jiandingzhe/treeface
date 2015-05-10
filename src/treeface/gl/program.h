#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/gl/sampler.h"

#include "treeface/common.h"
#include "treeface/math.h"

#include <treejuce/Array.h>
#include <treejuce/HashMap.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

class Material;
class Uniform;
class VertexArray;
class VertexAttrib;

/**
 * @brief The Program class represents a GLSL program with vertex shader and
 *        fragment shader.
 *
 * The program object is initially empty. The build() method will construct the
 * program with vertex and fragment source code. The metadata of all vertex
 * attributes and uniforms will be enumerated, and can be accessed by
 * get_attribute_index(), get_attribute_info(), get_uniform_index(),
 * get_uniform_info() methods.
 *
 * The geometry shader is not included, as OpenGL ES do not support it.
 *
 * The cached set_uniform() methods seems totally useless, and it will probably
 * be deprecated in future.
 */
class Program: public treejuce::Object
{
    friend class Material;
    friend class VertexArray;

public:
    /**
     * @brief create an empty Program object
     *
     * GL shader and program object indices will be allocated
     */
    Program();

    // invalidate copy and move
    JUCE_DECLARE_NON_COPYABLE(Program);
    JUCE_DECLARE_NON_MOVABLE(Program);

    /**
     * @brief the GL shader and program objects will also be released
     */
    virtual ~Program();

    /**
     * @brief compile source, link
     * @param src_vert: source code of vertex shader
     * @param src_frag: source code of fragment shader
     * @return ok if success, fail if any compilation and link error occurred.
     */
    treejuce::Result build(const char* src_vert, const char* src_frag);

    inline bool usable() const NOEXCEPT
    {
        return compiled_and_linked;
    }

    /**
     * @brief bind program, upload outdated cached uniforms
     */
    void use() NOEXCEPT;

    /**
     * @brief bind zero
     */
    inline static void unuse() NOEXCEPT
    {
        glUseProgram(0);
    }

    void instant_set_uniform(GLint uni, GLint value) const NOEXCEPT;
    void instant_set_uniform(GLint uni, GLuint value) const NOEXCEPT;
    void instant_set_uniform(GLint uni, GLfloat value) const NOEXCEPT;
    void instant_set_uniform(GLint uni, const Sampler& sampler) const NOEXCEPT;
    void instant_set_uniform(GLint uni, const Vec4f& value) const NOEXCEPT;
    void instant_set_uniform(GLint uni, const Mat4f& value) const NOEXCEPT;

    void set_uniform(GLint i_uni, GLint value) NOEXCEPT;
    void set_uniform(GLint i_uni, GLuint value) NOEXCEPT;
    void set_uniform(GLint i_uni, float value) NOEXCEPT;
    void set_uniform(GLint i_uni, const Sampler& value) NOEXCEPT;
    void set_uniform(GLint i_uni, const Vec4f& value) NOEXCEPT;
    void set_uniform(GLint i_uni, const Mat4f& value) NOEXCEPT;

    /**
     * @brief get vertex attribute index by name
     * @param name: attribute name
     * @return attribute index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_attribute_index(const treejuce::String& name) const NOEXCEPT;

    /**
     * @brief get vertex attribute info by index
     * @param i_attr: attribute index
     * @return attribute information
     */
    const VertexAttrib& get_attribute_info(int i_attr) const NOEXCEPT;

    /**
     * @brief get uniform index by name
     * @param name: uniform name
     * @return uniform index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_uniform_index(const treejuce::String& name) const NOEXCEPT;

    /**
     * @brief get uniform info by index
     * @param i_attr: uniform index
     * @return uniform information
     */
    const VertexAttrib& get_uniform_info(int i_uni) const NOEXCEPT;

protected:
    struct Impl;

    treejuce::Result fetch_shader_error_log(GLuint shader);
    treejuce::Result fetch_program_error_log();

    unsigned int m_program = 0;
    unsigned int m_shader_vert = 0;
    unsigned int m_shader_frag = 0;

    bool compiled_and_linked = false;

    Impl* m_impl = nullptr;
};


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_PROGRAM_H
