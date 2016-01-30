#ifndef TREEFACE_UNIVERSAL_VALUE_H
#define TREEFACE_UNIVERSAL_VALUE_H

#include "treeface/gl/Enums.h"
#include "treeface/gl/TypeUtils.h"

#include "treeface/math/Vec2.h"
#include "treeface/math/Vec3.h"
#include "treeface/math/Vec4.h"
#include "treeface/math/Mat2.h"
#include "treeface/math/Mat3.h"
#include "treeface/math/Mat4.h"

class TestFramework;

namespace treeface
{

class UniversalValue
{
    friend class ::TestFramework;

    union UniversalValueData
    {
        UniversalValueData() {}
        UniversalValueData( const UniversalValueData &peer ) { memcpy( this, &peer, sizeof(UniversalValueData) ); }
        UniversalValueData( GLbyte value ): i8( value ) {}
        UniversalValueData( GLubyte value ): ui8( value ) {}
        UniversalValueData( GLshort value ): i16( value ) {}
        UniversalValueData( GLushort value ): ui16( value ) {}
        UniversalValueData( GLint value ): i32( value ) {}
        UniversalValueData( GLuint value ): ui32( value ) {}
        UniversalValueData( GLfloat value ): f32( value ) {}
        UniversalValueData( GLdouble value ): f64( value ) {}
        UniversalValueData( const Vec2f &value ): vec2( value ) {}
        UniversalValueData( const Vec3f &value ): vec3( value ) {}
        UniversalValueData( const Vec4f &value ): vec4( value ) {}
        UniversalValueData( const Mat2f &value ): mat2( value ) {}
        UniversalValueData( const Mat3f &value ): mat3( value ) {}
        UniversalValueData( const Mat4f &value ): mat4( value ) {}

        GLbyte   i8;
        GLubyte  ui8;
        GLshort  i16;
        GLushort ui16;
        GLint    i32;
        GLuint   ui32;
        GLfloat  f32;
        GLdouble f64;
        Vec2f    vec2;
        Vec3f    vec3;
        Vec4f    vec4;
        Mat2f    mat2;
        Mat3f    mat3;
        Mat4f    mat4;
    };

public:
    UniversalValue() noexcept {}

    UniversalValue( GLType type ) noexcept: m_type( type ) {}

    UniversalValue( bool value ) noexcept
        : m_type( TFGL_TYPE_BOOL )
        , m_data( GLboolean( value ) )
    {}

    UniversalValue( GLbyte value ) noexcept
        : m_type( TFGL_TYPE_BYTE )
        , m_data( value )
    {}

    UniversalValue( GLubyte value ) noexcept
        : m_type( TFGL_TYPE_UNSIGNED_BYTE )
        , m_data( value )
    {}

    UniversalValue( GLshort value ) noexcept
        : m_type( TFGL_TYPE_SHORT )
        , m_data( value )
    {}

    UniversalValue( GLushort value ) noexcept
        : m_type( TFGL_TYPE_UNSIGNED_SHORT )
        , m_data( value )
    {}

    UniversalValue( GLint value ) noexcept
        : m_type( TFGL_TYPE_INT )
        , m_data( value )
    {}

    UniversalValue( GLuint value ) noexcept
        : m_type( TFGL_TYPE_UNSIGNED_INT )
        , m_data( value )
    {}

    UniversalValue( GLfloat value ) noexcept
        : m_type( TFGL_TYPE_FLOAT )
        , m_data( value )
    {}

    UniversalValue( GLdouble value ) noexcept
        : m_type( TFGL_TYPE_DOUBLE )
        , m_data( value )
    {}

    UniversalValue( const Vec2f& value ) noexcept
        : m_type( TFGL_TYPE_VEC2F )
        , m_data( value )
    {}

    UniversalValue( const Vec3f& value ) noexcept
        : m_type( TFGL_TYPE_VEC3F )
        , m_data( value )
    {}

    UniversalValue( const Vec4f& value ) noexcept
        : m_type( TFGL_TYPE_VEC4F )
        , m_data( value )
    {}

    UniversalValue( const Mat2f& value ) noexcept
        : m_type( TFGL_TYPE_MAT2F )
        , m_data( value )
    {}

    UniversalValue( const Mat3f& value ) noexcept
        : m_type( TFGL_TYPE_MAT3F )
        , m_data( value )
    {}

    UniversalValue( const Mat4f& value ) noexcept
        : m_type( TFGL_TYPE_MAT4F )
        , m_data( value )
    {}

    ///
    /// \brief copy constructor
    ///
    UniversalValue( const UniversalValue& peer ) noexcept
        : m_type( peer.m_type )
        , m_data( peer.m_data )
    {}

    ~UniversalValue() noexcept {}

    GLType get_type() const noexcept { return m_type; }

    UniversalValue& operator =( const UniversalValue& peer ) noexcept
    {
        m_type = peer.m_type;
        memcpy( &m_data, &peer.m_data, sizeof(m_data) );
        return *this;
    }

    UniversalValue& operator =( GLbyte value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_BYTE );
        m_data.i8 = value;
        return *this;
    }

    UniversalValue& operator =( GLubyte value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_BYTE );
        m_data.ui8 = value;
        return *this;
    }

    UniversalValue& operator =( GLshort value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_SHORT );
        m_data.i16 = value;
        return *this;
    }

    UniversalValue& operator =( GLushort value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_SHORT );
        m_data.ui16 = value;
        return *this;
    }

    UniversalValue& operator =( GLint value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_INT );
        m_data.i32 = value;
        return *this;
    }

    UniversalValue& operator =( GLuint value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_INT );
        m_data.ui32 = value;
        return *this;
    }

    UniversalValue& operator =( GLfloat value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT );
        m_data.f32 = value;
        return *this;
    }

    UniversalValue& operator =( GLdouble value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_DOUBLE );
        m_data.f64 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec2f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_VEC2 );
        m_data.vec2 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec3f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_VEC3 );
        m_data.vec3 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec4f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_VEC4 );
        m_data.vec4 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat2f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_MAT2 );
        m_data.mat2 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat3f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_MAT3 );
        m_data.mat3 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat4f& value ) noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT_MAT4 );
        m_data.mat4 = value;
        return *this;
    }

    operator GLbyte() const noexcept
    {
        jassert( m_type == TFGL_TYPE_BYTE );
        return m_data.i8;
    }

    operator GLubyte() const noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_BYTE );
        return m_data.ui8;
    }

    operator GLshort() const noexcept
    {
        jassert( m_type == TFGL_TYPE_SHORT );
        return m_data.i16;
    }

    operator GLushort() const noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_SHORT );
        return m_data.ui16;
    }

    operator GLint() const noexcept
    {
        jassert( m_type == TFGL_TYPE_INT );
        return m_data.i32;
    }

    operator GLuint() const noexcept
    {
        jassert( m_type == TFGL_TYPE_UNSIGNED_INT );
        return m_data.ui32;
    }

    operator GLfloat() const noexcept
    {
        jassert( m_type == TFGL_TYPE_FLOAT );
        return m_data.f32;
    }

    operator GLdouble() const noexcept
    {
        jassert( m_type == TFGL_TYPE_DOUBLE );
        return m_data.f64;
    }

    operator Vec2f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_VEC2F );
        return m_data.vec2;
    }

    operator Vec3f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_VEC3F );
        return m_data.vec3;
    }

    operator Vec4f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_VEC4F );
        return m_data.vec4;
    }

    operator Mat2f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_MAT2F );
        return m_data.mat2;
    }

    operator Mat3f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_MAT2F );
        return m_data.mat3;
    }

    operator Mat4f() const noexcept
    {
        jassert( m_type == TFGL_TYPE_MAT2F );
        return m_data.mat4;
    }

    bool operator ==( const UniversalValue& peer ) const;
    bool operator !=( const UniversalValue& peer ) const
    {
        return !operator ==( peer );
    }

private:
    GLType m_type;
    UniversalValueData m_data;
};

} // namespace treeface

#endif // TREEFACE_UNIVERSAL_VALUE_H
