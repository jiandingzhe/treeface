#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>

#include "treeface/common.h"
#include "treeface/math/vec3.h"

#include <treecore/AlignedMalloc.h>
#include <treecore/PlatformDefs.h>
#include <treecore/SimdObject.h>
#include <treecore/FloatUtils.h>
#include <treecore/IntUtils.h>

namespace treeface {

/**
 * 4-dimentional vector class implemented with SIMD operations.
 *
 * The layout of Vec4 is compatiable with OpenGL, so the data can be directly
 * transmitted to GL server side buffers or program attributes.
 */
template<typename T>
struct Vec4
{
    typedef typename treecore::similar_float<T>::type FloatType;
    typedef typename treecore::similar_int<T>::type   IntType;
    typedef treecore::SimdObject<T, 4>                DataType;

    TREECORE_ALIGNED_ALLOCATOR( Vec4 );

    /**
     * @brief default constructor
     *
     * values are initialized as zero point
     */
    Vec4(): data( T( 0 ), T( 0 ), T( 0 ), T( 1 ) ) {}

    /**
     * @brief constructor with values specified
     * @param value: a SIMDType containing x, y, z and w
     */
    Vec4( const DataType& value ): data( value ) {}

    /**
     * @brief copy constructor
     * @param other: another vector object
     */
    Vec4( const Vec4& other ): data( other.data ) {}

    /**
     * @brief constructor with values specified separately
     * @param x
     * @param y
     * @param z
     * @param w
     */
    Vec4( T x, T y, T z, T w )
    {
        data.set_all( x, y, z, w );
    }

    /**
     * @brief constructor with values specified
     * @param values: an array storing reversed w, z, y, x
     */
    Vec4( const T* values )
    {
        data.set_all( values );
    }

    Vec4 operator -() const noexcept
    {
        DataType result = data ^ treecore::SimdObject<IntType, 4>( treecore::float_sign_mask<T>::value );
        return Vec4( result );
    }

    bool is_zero() const noexcept
    {
        return data.template get<0>() != T( 0 ) ||
               data.template get<1>() != T( 0 ) ||
               data.template get<2>() != T( 0 ) ||
               data.template get<3>() != T( 0 );
    }

    ///
    /// \brief assignment operator
    /// \param other: another vector object
    /// \return self
    ///
    Vec4& operator =( const Vec4& other )
    {
        data = other.data;
        return *this;
    }

    /**
     * @brief set vector contents
     * @param x
     * @param y
     * @param z
     * @param w
     */
    void set( T x, T y, T z, T w )
    {
        data.set_all( x, y, z, w );
    }

    /**
     * @brief set vector contents
     * @param values: an array storing x, y, z and w
     */
    void set( const T* values )
    {
        data.set_all( values );
    }

    void set_point( const Vec3<T>& value )
    {
        data.set_all( value.x, value.y, value.z, T( 1 ) );
    }

    void set_direction( const Vec3<T>& value )
    {
        data.set_all( value.x, value.y, value.z, T( 0 ) );
    }

    /**
     * @brief get the value of 1st (x) component
     * @return x
     */
    T get_x() const
    {
        return data.template get<0>();
    }

    /**
     * @brief get the value of 2nd (y) component
     * @return y
     */
    T get_y() const
    {
        return data.template get<1>();
    }

    /**
     * @brief get the value of 3rd (z) component
     * @return z
     */
    T get_z() const
    {
        return data.template get<2>();
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const
    {
        return data.template get<3>();
    }

    /**
     * @brief set the value of 1st (x) component
     * @param value: the value to be set to x component
     */
    void set_x( T value )
    {
        data.template set<0>( value );
    }

    /**
     * @brief set the value of 2nd (y) component
     * @param value: the value to be set to y component
     */
    void set_y( T value )
    {
        data.template set<1>( value );
    }

    /**
     * @brief set the value of 3rd (z) component
     * @param value: the value to be set to z component
     */
    void set_z( T value )
    {
        data.template set<2>( value );
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w( T value )
    {
        data.template set<3>( value );
    }

    /**
     * @brief vector add by another vector
     * @param other: the addend to be added to self
     * @return self
     */
    Vec4& operator +=( const Vec4& other )
    {
        data += other.data;
        return *this;
    }

    /**
     * @brief vector sub by another vector
     * @param other: the subtrahend
     * @return self
     */
    Vec4& operator -=( const Vec4& other )
    {
        data -= other.data;
        return *this;
    }

    /**
     * @brief scale by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator *=( T value )
    {
        data *= DataType( value );
        return *this;
    }

    /**
     * @brief shrink by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator /=( T value )
    {
        data /= DataType( value );
        return *this;
    }

    /**
     * @brief scale vector to make its length to be 1
     * @return length before normalize
     */
    FloatType normalize()
    {
        FloatType len = length();
        data /= DataType( len );
        return len;
    }

    /**
     * @brief get vector length
     * @return length
     */
    FloatType length() const
    {
        return std::sqrt( length2() );
    }

    /**
     * @brief get length square
     * @return length square
     */
    FloatType length2() const
    {
        return FloatType( (data * data).sum() );
    }

    /**
     * @brief the underlying data storing all vector compnents.
     *
     * The memory layout should be compatible with OpenGL, so that it can be
     * transmitted to GL buffers or program attributes directly.
     */
    DataType data;
};

/**
 * @brief vector add
 *
 * @param a: augend
 * @param b: addend
 * @return a new Vec4 object containing result
 */
template<typename T>
Vec4<T> operator +( const Vec4<T>& a, const Vec4<T>& b )
{
    Vec4<T> result;
    result.data = a.data + b.data;
    return result;
}

/**
 * @brief vector sub
 *
 * @param a: minuend
 * @param b: subtrahend
 * @return a new Vec4 object containing result
 */
template<typename T>
Vec4<T> operator -( const Vec4<T>& a, const Vec4<T>& b )
{
    Vec4<T> result;
    result.data = a.data - b.data;
    return result;
}

/**
 * @brief scale with a scalar
 *
 * @param a: the vector value
 * @param b: scale
 * @return a new Vec4 object containing scaled values
 */
template<typename T>
Vec4<T> operator *( const Vec4<T>& a, T b )
{
    Vec4<T> result;
    result.data = a.data * typename Vec4<T>::DataType( b );
    return result;
}

template<typename T>
Vec4<T> operator *( T a, const Vec4<T>& b )
{
    Vec4<T> result;
    result.data = b.data * typename Vec4<T>::DataType( a );
    return result;
}

/**
 * @brief shrink with a scalar
 *
 * @param a: the vector value
 * @param b: scale
 * @return a new Vec4 object containing shrinked values
 */
template<typename T>
Vec4<T> operator /( const Vec4<T>& a, T b )
{
    Vec4<T> result;
    result.data = a.data / typename Vec4<T>::DataType( b );
    return result;
}

/**
 * @brief dot multiply
 *
 * @param a: one vector value
 * @param b: the other vector value
 * @return a scalar value containing dot product
 */
template<typename T>
T operator *( const Vec4<T>& a, const Vec4<T>& b )
{
    return (a.data * b.data).sum();
}

/**
 * @brief cross multiply
 * @param a: one vector value
 * @param b: the other vector value
 * @return a new Vec4 containing right-hand cross product
 */
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator %( const Vec4<T>& a, const Vec4<T>& b )
{
    // Res.ele[0] = A.ele[1] * B.ele[2] - A.ele[2] * B.ele[1];
    // Res.ele[1] = A.ele[2] * B.ele[0] - A.ele[0] * B.ele[2];
    // Res.ele[2] = A.ele[0] * B.ele[1] - A.ele[1] * B.ele[0];
    // Res.ele[3] = 0.0f;
    typename Vec4<T>::DataType v1 = a.data;
    typename Vec4<T>::DataType v2 = b.data;
    typename Vec4<T>::DataType v3 = a.data;
    typename Vec4<T>::DataType v4 = b.data;

    v1.template shuffle<1, 2, 0, 0>();
    v2.template shuffle<2, 0, 1, 0>();
    v3.template shuffle<2, 0, 1, 0>();
    v4.template shuffle<1, 2, 0, 0>();

    typename Vec4<T>::DataType re = v1 * v2 - v3 * v4;
    re.template set<3>( T( 0 ) );

    return Vec4<T>( re );
}

/**
 * @brief test if two Vec4 objects are identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator ==( const Vec4<T>& a, const Vec4<T>& b )
{
    return a.data == b.data;
}

/**
 * @brief test if two Vec4 objects are not identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator !=( const Vec4<T>& a, const Vec4<T>& b )
{
    return a.data != b.data;
}

typedef Vec4<float>           Vec4f;
typedef Vec4<treecore::int32> Vec4i;

} // namespace treeface

#endif // TREEFACE_VEC4_H
