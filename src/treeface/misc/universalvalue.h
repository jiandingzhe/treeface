#ifndef TREEFACE_UNIVERSAL_VALUE_H
#define TREEFACE_UNIVERSAL_VALUE_H

#include "treeface/gl/enums.h"
#include "treeface/gl/type.h"

#include "treeface/math/vec2.h"
#include "treeface/math/vec3.h"
#include "treeface/math/vec4.h"
#include "treeface/math/mat2.h"
#include "treeface/math/mat3.h"
#include "treeface/math/mat4.h"

class TestFramework;

namespace treeface
{

class UniversalValue
{
    friend ::TestFramework;

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

        GLbyte    i8;
        GLubyte   ui8;
        GLshort   i16;
        GLushort  ui16;
        GLint     i32;
        GLuint    ui32;
        GLfloat   f32;
        GLdouble  f64;
        Vec2f     vec2;
        Vec3f     vec3;
        Vec4f     vec4;
        Mat2f     mat2;
        Mat3f     mat3;
        Mat4f     mat4;
    };

public:
    UniversalValue( GLType type ) noexcept: type( type ) {};

    UniversalValue( const UniversalValue& peer ) noexcept
        : type( peer.type )
        , m_data( peer.m_data )
    {}

    ~UniversalValue() noexcept {}

    UniversalValue& operator =( const UniversalValue& peer ) noexcept
    {
        jassert( type == peer.type );
        memcpy( &m_data, &peer.m_data, sizeof(m_data) );
        return *this;
    }

    UniversalValue& operator =( GLbyte value ) noexcept
    {
        jassert( type == TFGL_TYPE_BYTE );
        m_data.i8 = value;
        return *this;
    }

    UniversalValue& operator =( GLubyte value ) noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_BYTE );
        m_data.ui8 = value;
        return *this;
    }

    UniversalValue& operator =( GLshort value ) noexcept
    {
        jassert( type == TFGL_TYPE_SHORT );
        m_data.i16 = value;
        return *this;
    }

    UniversalValue& operator =( GLushort value ) noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_SHORT );
        m_data.ui16 = value;
        return *this;
    }

    UniversalValue& operator =( GLint value ) noexcept
    {
        jassert( type == TFGL_TYPE_INT );
        m_data.i32 = value;
        return *this;
    }

    UniversalValue& operator =( GLuint value ) noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_INT );
        m_data.ui32 = value;
        return *this;
    }

    UniversalValue& operator =( GLfloat value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT );
        m_data.f32 = value;
        return *this;
    }

    UniversalValue& operator =( GLdouble value ) noexcept
    {
        jassert( type == TFGL_TYPE_DOUBLE );
        m_data.f64 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec2f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_VEC2 );
        m_data.vec2 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec3f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_VEC3 );
        m_data.vec3 = value;
        return *this;
    }

    UniversalValue& operator =( const Vec4f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_VEC4 );
        m_data.vec4 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat2f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_MAT2 );
        m_data.mat2 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat3f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_MAT3 );
        m_data.mat3 = value;
        return *this;
    }

    UniversalValue& operator =( const Mat4f& value ) noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT_MAT4 );
        m_data.mat4 = value;
        return *this;
    }

    operator GLbyte() const noexcept
    {
        jassert( type == TFGL_TYPE_BYTE );
        return m_data.i8;
    }

    operator GLubyte() const noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_BYTE );
        return m_data.ui8;
    }

    operator GLshort() const noexcept
    {
        jassert( type == TFGL_TYPE_SHORT );
        return m_data.i16;
    }

    operator GLushort() const noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_SHORT );
        return m_data.ui16;
    }

    operator GLint() const noexcept
    {
        jassert( type == TFGL_TYPE_INT );
        return m_data.i32;
    }

    operator GLuint() const noexcept
    {
        jassert( type == TFGL_TYPE_UNSIGNED_INT );
        return m_data.ui32;
    }

    operator GLfloat() const noexcept
    {
        jassert( type == TFGL_TYPE_FLOAT );
        return m_data.f32;
    }

    operator GLdouble() const noexcept
    {
        jassert( type == TFGL_TYPE_DOUBLE );
        return m_data.f64;
    }

    operator Vec2f() const noexcept
    {
        jassert( type == TFGL_TYPE_VEC2F );
        return m_data.vec2;
    }

    operator Vec3f() const noexcept
    {
        jassert( type == TFGL_TYPE_VEC3F );
        return m_data.vec3;
    }

    operator Vec4f() const noexcept
    {
        jassert( type == TFGL_TYPE_VEC4F );
        return m_data.vec4;
    }

    operator Mat2f() const noexcept
    {
        jassert( type == TFGL_TYPE_MAT2F );
        return m_data.mat2;
    }

    operator Mat3f() const noexcept
    {
        jassert( type == TFGL_TYPE_MAT2F );
        return m_data.mat3;
    }

    operator Mat4f() const noexcept
    {
        jassert( type == TFGL_TYPE_MAT2F );
        return m_data.mat4;
    }

    const GLType type;

private:
    UniversalValueData m_data;
};

}

#endif // TREEFACE_UNIVERSAL_VALUE_H
