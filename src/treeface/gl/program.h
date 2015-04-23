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

    void use() NOEXCEPT;

    inline void unuse() const NOEXCEPT
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
     * get attribute index by name
     *
     * @param name: attribute name
     * @return attribute index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_attribute_index(const treejuce::String& name) const NOEXCEPT;

    const VertexAttrib& get_attribute_info(int i_attr) const NOEXCEPT;

    /**
     * get uniform index by name
     *
     * @param name: uniform name
     * @return uniform index. -1 will be returned if the name does not exist
     * in program.
     */
    int get_uniform_index(const treejuce::String& name) const NOEXCEPT;

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
