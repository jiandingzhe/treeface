#include "treeface/gl/Errors.h"
#include "treeface/gl/Program.h"
#include "treeface/gl/TypeUtils.h"
#include "treeface/gl/VertexAttrib.h"

#include "treeface/misc/StringCast.h"
#include "treeface/misc/UniversalValue.h"

#include <treecore/Logger.h>
#include <treecore/StringRef.h>

#include <cstdio>

using namespace std;
using namespace treecore;

namespace treeface {

struct Program::Impl
{
    treecore::Array<VertexAttrib> attr_info;
    treecore::HashMap<Identifier, int32> attr_idx_by_name;

    treecore::Array<UniformInfo> uni_store;
    treecore::HashMap<Identifier, int32> uni_idx_by_name; // name => index
    treecore::HashMap<GLint,  int32>     uni_idx_by_loc; // location => index
};

Program::Program( const char* src_vert, const char* src_frag ): m_impl( new Impl() )
{
    m_program = glCreateProgram();
    if (m_program == 0)
        throw std::runtime_error( "failed to assign program ID" );

    m_shader_vert = glCreateShader( GL_VERTEX_SHADER );
    if (!m_shader_vert)
        throw std::runtime_error( "failed to allocate shader ID for vertex shader" );

    m_shader_frag = glCreateShader( GL_FRAGMENT_SHADER );
    if (!m_shader_frag)
        throw std::runtime_error( "failed to allocate shader ID for fragment shader" );

    DBG( "build program" );

    // compile shaders
    {
        glShaderSource( m_shader_vert, 1, &src_vert, nullptr );
        glCompileShader( m_shader_vert );

        treecore::Result re = fetch_shader_error_log( m_shader_vert );
        if (!re)
            throw ProgramCompileError( "failed to compile vertex shader:\n" +
                                       re.getErrorMessage() + "\n" +
                                       "==== vertex shader source ====\n\n" +
                                       String( src_vert ) + "\n" +
                                       "==============================\n" );

        glAttachShader( m_program, m_shader_vert );
    }

    {
        glShaderSource( m_shader_frag, 1, &src_frag, nullptr );
        glCompileShader( m_shader_frag );

        treecore::Result re = fetch_shader_error_log( m_shader_frag );
        if (!re)
            throw ProgramCompileError( "failed to compile fragment shader:\n" +
                                       re.getErrorMessage() + "\n" +
                                       "==== fragment shader source ====\n\n" +
                                       String( src_frag ) + "\n" +
                                       "================================\n" );

        glAttachShader( m_program, m_shader_frag );
    }

    // link program
    glLinkProgram( m_program );

    {
        treecore::Result re = fetch_program_error_log();
        if (!re)
            throw ProgramLinkError( "failed to link shader:\n" +
                                    re.getErrorMessage() + "\n" );
    }

    // extract program attributes
    int n_attr = -1;
    glGetProgramiv( m_program, GL_ACTIVE_ATTRIBUTES, &n_attr );
    if (n_attr == -1)
        die( "failed to get attribute number in program %u", m_program );
    DBG( "vertex attribute amount: " + String( n_attr ) );

    for (int i_attr = 0; i_attr < n_attr; i_attr++)
    {
        char    attr_name[256];
        GLsizei attr_name_len = -1;
        GLenum  attr_type     = 0;
        GLsizei attr_size     = -1;
        glGetActiveAttrib( m_program, i_attr, 256,
                           &attr_name_len, &attr_size, &attr_type, attr_name );

        DBG( "  attribute " + String( i_attr ) + ": name " + String( attr_name ) + ", type " + toString( GLType( attr_type ) ) + ", size " + String( attr_size ) );

        if (attr_name_len == -1)
            die( "failed to get info for attribute %d", i_attr );

        if (attr_name_len >= 255)
            die( "attribute %d name is too long", i_attr );

        m_impl->attr_info.add( { Identifier( attr_name ), attr_size, GLType( attr_type ) } );
        m_impl->attr_idx_by_name.set( Identifier( attr_name ), i_attr );
    }

    // extract program uniforms
    int n_uni = -1;
    glGetProgramiv( m_program, GL_ACTIVE_UNIFORMS, &n_uni );
    if (n_uni == -1)
        die( "failed to get uniform number from program %u", m_program );
    DBG( "uniform amount: " + String( n_uni ) );

    for (int i_uni = 0; i_uni < n_uni; i_uni++)
    {
        // get uniform info
        char    uni_name[256];
        GLsizei uni_name_len = -1;
        GLsizei uni_size     = -1;
        GLenum  uni_type     = 0;
        glGetActiveUniform( m_program, i_uni, 256,
                            &uni_name_len, &uni_size, &uni_type, uni_name );

        if (uni_name_len == -1)
            die( "failed to get info for uniform %d", i_uni );

        if (uni_name_len >= 255)
            die( "uniform %d name is too long", i_uni );

        // get uniform location by name
        GLint uni_loc = glGetUniformLocation( m_program, uni_name );
        if (uni_loc == -1)
            die( "failed to get location for uniform %s", uni_name );

        DBG( "  uniform " + String( i_uni ) + " " + String( uni_name ) + " at " + String( uni_loc ) + ": type " + toString( GLType( uni_type ) ) + ", size " + String( uni_size ) );

        // store uniform info
        m_impl->uni_store.add( { Identifier( uni_name ), uni_size, GLType( uni_type ), uni_loc } );
        m_impl->uni_idx_by_name.set( Identifier( uni_name ), i_uni );
        m_impl->uni_idx_by_loc.set( uni_loc, i_uni );
    }
}

Program::~Program()
{
    if (m_program)
        glDeleteProgram( m_program );

    if (m_shader_vert)
        glDeleteShader( m_shader_vert );

    if (m_shader_frag)
        glDeleteShader( m_shader_frag );

    if (m_impl)
        delete m_impl;
}

treecore::Result Program::fetch_shader_error_log( GLuint shader )
{
    GLint result = -1;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );

    if (result == -1)
        die( "failed to fetch compile status from shader %u\n", shader );

    if (result == GL_FALSE)
    {
        GLint log_len = -1;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_len );

        if (log_len > 0)
        {
            char*   log     = (char*) malloc( log_len + 1 );
            GLsizei written = 0;
            glGetShaderInfoLog( shader, log_len, &written, log );

            treecore::Result re = treecore::Result::fail( log );
            free( log );
            return re;
        }
        else
        {
            die( "failed to retrieve shader compile error log" );
        }
    }

    return treecore::Result::ok();
}

treecore::Result Program::fetch_program_error_log()
{
    GLint result = -1;
    glGetProgramiv( m_program, GL_LINK_STATUS, &result );

    if (result == -1)
        die( "failed to fetch link status from program %u\n", m_program );

    if (result == GL_FALSE)
    {
        GLint log_len = -1;
        glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &log_len );

        if (log_len > 0)
        {
            char*   log     = (char*) malloc( log_len + 1 );
            GLsizei written = 0;
            glGetProgramInfoLog( m_program, log_len, &written, log );

            treecore::Result re = treecore::Result::fail( log );
            free( log );
            return re;
        }
        else
        {
            die( "failed to retrieve program link error log" );
        }
    }

    return treecore::Result::ok();
}

void Program::set_uniform( GLint uni_loc, GLint value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == GL_INT ||
             uni_info.type == GL_BOOL ||
             uni_info.type == GL_SAMPLER_2D ||
             uni_info.type == GL_SAMPLER_3D ||
             uni_info.type == GL_SAMPLER_CUBE ||
             uni_info.type == GL_SAMPLER_2D_SHADOW ||
             uni_info.type == GL_SAMPLER_2D_ARRAY ||
             uni_info.type == GL_SAMPLER_2D_ARRAY_SHADOW ||
             uni_info.type == GL_SAMPLER_CUBE_SHADOW );
#endif
    glUniform1i( uni_loc, value );
}

void Program::set_uniform( GLint uni_loc, GLuint value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == GL_UNSIGNED_INT ||
             uni_info.type == GL_BOOL );
#endif
    glUniform1ui( uni_loc, value );
}

void Program::set_uniform( GLint uni_loc, GLfloat value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == GL_FLOAT ||
             uni_info.type == GL_BOOL );
#endif
    glUniform1f( uni_loc, value );
}

void Program::set_uniform( GLint uni_loc, const Vec2f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_VEC2F );
#endif
    glUniform2fv( uni_loc, 1, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const Vec3f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_VEC3F );
#endif
    glUniform3fv( uni_loc, 1, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const Vec4f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_VEC4F );
#endif
    glUniform4fv( uni_loc, 1, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const Mat2f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_MAT2F );
#endif
    glUniformMatrix2fv( uni_loc, 1, false, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const Mat3f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_MAT3F );
#endif
    glUniformMatrix3fv( uni_loc, 1, false, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const Mat4f& value ) const noexcept
{
    jassert( is_bound() );
    if (uni_loc == -1) return;
#ifdef JUCE_DEBUG
    int i_uni = get_uniform_index( uni_loc );
    jassert( i_uni >= 0 );
    const UniformInfo& uni_info = m_impl->uni_store[i_uni];
    jassert( uni_info.n_elem == 1 );
    jassert( uni_info.type == TFGL_TYPE_MAT4F );
#endif
    glUniformMatrix4fv( uni_loc, 1, false, (const float*) &value );
}

void Program::set_uniform( GLint uni_loc, const UniversalValue& value ) const noexcept
{
    if (uni_loc == -1) return;

    switch ( value.get_type() )
    {
    case TFGL_TYPE_BYTE:          set_uniform( uni_loc, GLbyte( value ) );  break;
    case TFGL_TYPE_BOOL:
    case TFGL_TYPE_UNSIGNED_BYTE: set_uniform( uni_loc, GLubyte( value ) ); break;
    case TFGL_TYPE_SAMPLER_2D:
    case TFGL_TYPE_SAMPLER_2D_ARRAY:
    case TFGL_TYPE_SAMPLER_2D_SHADOW:
    case TFGL_TYPE_SAMPLER_2D_ARRAY_SHADOW:
    case TFGL_TYPE_SAMPLER_CUBE:
    case TFGL_TYPE_SAMPLER_CUBE_SHADOW:
    case TFGL_TYPE_SAMPLER_3D:
    case TFGL_TYPE_INT:           set_uniform( uni_loc, GLint( value ) );   break;
    case TFGL_TYPE_UNSIGNED_INT:  set_uniform( uni_loc, GLuint( value ) );  break;
    case TFGL_TYPE_FLOAT:         set_uniform( uni_loc, GLfloat( value ) ); break;
    case TFGL_TYPE_VEC2F:         set_uniform( uni_loc, Vec2f( value ) );   break;
    case TFGL_TYPE_VEC3F:         set_uniform( uni_loc, Vec3f( value ) );   break;
    case TFGL_TYPE_VEC4F:         set_uniform( uni_loc, Vec4f( value ) );   break;
    case TFGL_TYPE_MAT2F:         set_uniform( uni_loc, Mat2f( value ) );   break;
    case TFGL_TYPE_MAT3F:         set_uniform( uni_loc, Mat3f( value ) );   break;
    case TFGL_TYPE_MAT4F:         set_uniform( uni_loc, Mat4f( value ) );   break;
    default:
        throw std::logic_error( "UniversalValue type is not supported to be set as uniform value" );
    }
}

int Program::get_attribute_index( const treecore::Identifier& name ) const noexcept
{
    if ( m_impl->attr_idx_by_name.contains( name ) )
        return m_impl->attr_idx_by_name[name];
    else
        return -1;
}

const VertexAttrib& Program::get_attribute( int i_attr ) const noexcept
{
    return m_impl->attr_info[i_attr];
}

int Program::get_uniform_index( const treecore::Identifier& name ) const noexcept
{
    if ( m_impl->uni_idx_by_name.contains( name ) )
        return m_impl->uni_idx_by_name[name];
    else
        return -1;
}

int Program::get_uniform_index( const GLint uni_loc ) const noexcept
{
    if ( m_impl->uni_idx_by_loc.contains( uni_loc ) )
        return m_impl->uni_idx_by_loc[uni_loc];
    else
        return -1;
}

const UniformInfo& Program::get_uniform( int i_uni ) const noexcept
{
    return m_impl->uni_store[i_uni];
}

GLint Program::get_uniform_location( const treecore::Identifier& name ) const noexcept
{
    if ( m_impl->uni_idx_by_name.contains( name ) )
    {
        int index = m_impl->uni_idx_by_name[name];
        return m_impl->uni_store[index].location;
    }
    else
    {
        return -1;
    }
}

} // namespace treeface
