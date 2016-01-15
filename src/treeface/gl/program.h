#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/gl/sampler.h"
#include "treeface/gl/vertexattrib.h"

#include "treeface/common.h"
#include "treeface/math.h"

#include <treecore/Array.h>
#include <treecore/HashMap.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>
#include <treecore/String.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <stdexcept>

namespace treeface {

class Material;
struct VertexArray;

struct UniformInfo: public VertexAttrib
{
    UniformInfo( const treecore::String& name, treecore::int32 n_elem, GLType type, GLint location )
        : VertexAttrib( name, n_elem, type )
        , location( location )
    {}

    const GLint location;
};

/**
 * @brief The Program class represents a GLSL program with vertex shader and
 *        fragment shader.
 *
 * The metadata of all vertex attributes and uniforms will be enumerated during
 * build time, and can be accessed by get_attribute_index(),
 * get_attribute_info(), get_uniform_index(), get_uniform_info() methods.
 *
 * The geometry shader is not included, as OpenGL ES do not support it.
 *
 * The cached set_uniform() methods seems totally useless, and it will probably
 * be deprecated in future.
 */
class Program: public treecore::RefCountObject
{
    friend class Material;
    friend struct VertexArray;

public:
    class BindScope
    {
    public:
        BindScope( Program& program ): m_program( program ) { m_program.bind(); }
        ~BindScope()                                        { Program::unbind(); }
    private:
        Program& m_program;
    };

    ///
    /// \brief create Program object
    ///
    /// The OpenGL shader objects will be created using specified vertex and
    /// fragment source code, and the OpenGL program object will be linked.
    ///
    /// In addition, all vertex attribute and uniform information will be
    /// retrieved and stored in some data structure. They can be acquired using
    /// get_uniform_XXX and get_attribute_XXX methods, and don't need program
    /// binding.
    ///
    Program( const char* src_vert, const char* src_frag );

    // invalidate copy and move
    TREECORE_DECLARE_NON_COPYABLE( Program );
    TREECORE_DECLARE_NON_MOVABLE( Program );

    virtual ~Program();

    ///
    /// \brief bind program
    ///
    void bind() noexcept { glUseProgram( m_program ); }

    ///
    /// \brief unbind program
    ///
    static void unbind() { glUseProgram( 0 ); }

    bool is_bound() const noexcept { return get_current_bound_program() == m_program; }

    void set_uniform( GLint uni_loc, GLint value ) const noexcept;
    void set_uniform( GLint uni_loc, GLuint value ) const noexcept;
    void set_uniform( GLint uni_loc, GLfloat value ) const noexcept;
    void set_uniform( GLint uni_loc, const Sampler& sampler ) const noexcept;
    void set_uniform( GLint uni_loc, const Vec4f& value ) const noexcept;
    void set_uniform( GLint uni_loc, const Mat4f& value ) const noexcept;

    ///
    /// \brief get vertex attribute index by name
    ///
    /// \param name  attribute name
    /// \return attribute index. -1 will be returned if the name does not exist
    ///         in program.
    ///
    /// \see get_attribute
    ///
    int get_attribute_index( const treecore::String& name ) const noexcept;

    ///
    /// \brief get vertex attribute info by index
    ///
    /// \param i_attr: attribute index
    /// \return attribute information
    ///
    /// \see get_attribute_index
    ///
    const VertexAttrib& get_attribute( int i_attr ) const noexcept;

    /**
     * \brief get uniform index by name
     *
     * Note that uniform index is different from uniform location!
     *
     * \param name  uniform name
     * \return uniform index. -1 will be returned if name not exist.
     *
     * \see get_uniform
     */
    int get_uniform_index( const treecore::String& name ) const noexcept;

    ///
    /// \brief get uniform index by location
    ///
    /// Note that uniform index is different from uniform location!
    ///
    /// \param uni_loc: uniform location
    /// \return uniform index. -1 will be returned if location not exist.
    ///
    /// \see get_uniform
    ///
    int get_uniform_index( const GLint uni_loc ) const noexcept;

    ///
    /// \brief get uniform info by index
    ///
    /// \param i_attr: uniform index
    /// \return uniform information
    ///
    /// \see get_uniform_index
    ///
    const UniformInfo& get_uniform( int i_uni ) const noexcept;

    ///
    /// \brief get uniform location by name
    ///
    /// Note that uniform location is used by OpenGL uniform accession, which
    /// different from uniform index!
    ///
    /// \param name  uniform name
    /// \return uniform location
    /// \see set_uniform
    ///
    GLint get_uniform_location( const treecore::String& name ) const noexcept;

    GLuint get_gl_handle() const noexcept { return m_program; }

    static GLuint get_current_bound_program() noexcept
    {
        GLint result = -1;
        glGetIntegerv( GL_CURRENT_PROGRAM, &result );
        return GLuint( result );
    }
protected:
    struct Impl;

    treecore::Result fetch_shader_error_log( GLuint shader );
    treecore::Result fetch_program_error_log();

    GLuint m_program     = 0;
    GLuint m_shader_vert = 0;
    GLuint m_shader_frag = 0;

    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_GL_PROGRAM_H
