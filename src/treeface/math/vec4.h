#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>

#include "treeface/common.h"
#include "treeface/simd.h"

TREEFACE_NAMESPACE_BEGIN


/**
 * 4-dimentional vector class implemented with SIMD operations.
 *
 * The layout of Vec4 is compatiable with OpenGL, so the data can be directly
 * transmitted to GL server side buffers or program attributes.
 */
template<typename T, int SZ = sizeof(T)*4>
struct Vec4
{
    typedef SIMDType<SZ> DataType;

    /**
     * @brief default constructor
     *
     * values are initialized as zero point
     */
    Vec4(): data(simd_set<T, SZ>(0, 0, 0, 1)) {}

    /**
     * @brief constructor with values specified
     * @param value: a SIMDType containing x, y, z and w
     */
    Vec4(SIMDType<SZ> value): data(value) {}

    /**
     * @brief copy constructor
     * @param other: another vector object
     */
    Vec4(const Vec4& other): data(other.data) {}

    /**
     * @brief constructor with values specified separately
     * @param x
     * @param y
     * @param z
     * @param w
     */
    Vec4(T x, T y, T z, T w): data(simd_set<T, SZ> (x, y, z, w)) {}

    /**
     * @brief constructor with values specified
     * @param values: an array storing x, y, z and w
     */
    Vec4(const T* values): data(simd_set<T, SZ>(values)) {}

    /**
     * @brief assignment operator
     * @param other: another vector object
     * @return self
     */
    Vec4& operator = (const Vec4& other)
    {
        data = other.data;
    }

    /**
     * @brief set vector contents
     * @param x
     * @param y
     * @param z
     * @param w
     */
    void set(float x, float y, float z, float w)
    {
        data = simd_set<T, SZ> (x, y, z, w);
    }

    /**
     * @brief set vector contents
     * @param values: an array storing x, y, z and w
     */
    void set(const float* values)
    {
        data = simd_set<T, SZ> (values);
    }

    /**
     * @brief set vector contents
     * @param other: another vector whose data will be copied to this vector
     */
    void set(const Vec4& other)
    {
        data = other.data;
    }

    /**
     * @brief get the value of 1st (x) component
     * @return x
     */
    T get_x() const
    {
        return simd_get_one<0, float>(data);
    }

    /**
     * @brief get the value of 2nd (y) component
     * @return y
     */
    T get_y() const
    {
        return simd_get_one<1, float>(data);
    }

    /**
     * @brief get the value of 3rd (z) component
     * @return z
     */
    T get_z() const
    {
        return simd_get_one<2, float>(data);
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const
    {
        return simd_get_one<3, float>(data);
    }

    /**
     * @brief set the value of 1st (x) component
     * @param value: the value to be set to x component
     */
    void set_x(T value)
    {
        data = simd_set_one<0>(data, value);
    }

    /**
     * @brief set the value of 2nd (y) component
     * @param value: the value to be set to y component
     */
    void set_y(T value)
    {
        data = simd_set_one<1>(data, value);
    }

    /**
     * @brief set the value of 3rd (z) component
     * @param value: the value to be set to z component
     */
    void set_z(T value)
    {
        data = simd_set_one<2>(data, value);
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w(T value)
    {
        data = simd_set_one<3>(data, value);
    }

    /**
     * @brief vector add by another vector
     * @param other: the addend to be added to self
     * @return self
     */
    Vec4& operator += (const Vec4& other)
    {
        data = simd_add<T>(data, other.data);
    }

    /**
     * @brief vector sub by another vector
     * @param other: the subtrahend
     * @return self
     */
    Vec4& operator -= (const Vec4& other)
    {
        data = simd_sub<T>(data, other.data);
    }

    /**
     * @brief scale by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator *= (T value)
    {
        SIMDType<SZ> tmp = simd_set<T, SZ> (value);
        data = simd_mul<T>(data, tmp);
    }

    /**
     * @brief shrink by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator /= (float value)
    {
        SIMDType<SZ> tmp = simd_set<T, SZ>(value);
        data = simd_div<T>(data, tmp);
    }

    /**
     * @brief scale vector to make its length to be 1
     * @return length before normalize
     */
    T normalize()
    {
        SIMDType<SZ> tmp = simd_set<T, SZ>(length());
        data = simd_div<T>(data, tmp);
    }

    /**
     * @brief get vector length
     * @return length
     */
    T length() const
    {
        return std::sqrt(length2());
    }

    /**
     * @brief get length square
     * @return length square
     */
    T length2() const
    {
        return simd_sum<T, SZ>(simd_mul<T, SZ>(data, data));
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
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator + (const Vec4<T>& a, const Vec4<T>& b)
{
    return Vec4<T>( simd_add<T, SZ>(a.data, b.data) );
}

/**
 * @brief vector sub
 *
 * @param a: minuend
 * @param b: subtrahend
 * @return a new Vec4 object containing result
 */
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator - (const Vec4<T>& a, const Vec4<T>& b)
{
    return Vec4<T>( simd_sub<T, SZ>(a.data, b.data) );
}

/**
 * @brief scale with a scalar
 *
 * @param a: the vector value
 * @param b: scale
 * @return a new Vec4 object containing scaled values
 */
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator * (const Vec4<T>& a, T b)
{
    SIMDType<SZ> tmp = simd_set<T, SZ> (b);
    return Vec4<T>(simd_mul<T, SZ>(a.data, tmp));
}

/**
 * @brief shrink with a scalar
 *
 * @param a: the vector value
 * @param b: scale
 * @return a new Vec4 object containing shrinked values
 */
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator / (const Vec4<T>& a, T b)
{
    SIMDType<16> tmp = simd_set<T, 16> (b);
    return Vec4<T>(simd_div<T, SZ>(a.data, tmp));
}

/**
 * @brief dot multiply
 *
 * @param a: one vector value
 * @param b: the other vector value
 * @return a scalar value containing dot product
 */
template<typename T, int SZ = sizeof(T)*4>
T operator * (const Vec4<T>& a, const Vec4<T>& b)
{
    SIMDType<16> tmp = simd_mul<T>(a.data, b.data);
    return simd_sum<T, SZ>(tmp);
}

/**
 * @brief cross multiply
 * @param a: one vector value
 * @param b: the other vector value
 * @return a new Vec4 containing right-hand cross product
 */
template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator ^ (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

/**
 * @brief test if two Vec4 objects are identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator == (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

/**
 * @brief test if two Vec4 objects are not identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator != (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

typedef Vec4<float> Vec4f;
typedef Vec4<std::int32_t> Vec4i;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VEC4_H
