#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>

#include "treeface/nedbase.h"

#include <treejuce/PlatformDefs.h>
#include <treejuce/SIMD.h>

TREEFACE_NAMESPACE_BEGIN


/**
 * 4-dimentional vector class implemented with SIMD operations.
 *
 * The layout of Vec4 is compatiable with OpenGL, so the data can be directly
 * transmitted to GL server side buffers or program attributes.
 */
template<typename T, int SZ = sizeof(T)*4>
struct Vec4: public NedBase
{
    typedef treejuce::SIMDType<SZ> DataType;

    /**
     * @brief default constructor
     *
     * values are initialized as zero point
     */
    Vec4(): data(treejuce::simd_set<T, SZ>(0, 0, 0, 1)) {}

    /**
     * @brief constructor with values specified
     * @param value: a SIMDType containing x, y, z and w
     */
    Vec4(const treejuce::SIMDType<SZ>& value): data(value) {}

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
    Vec4(T x, T y, T z, T w): data(treejuce::simd_set<T, SZ> (x, y, z, w)) {}

    /**
     * @brief constructor with values specified
     * @param values: an array storing x, y, z and w
     */
    Vec4(const T* values): data(treejuce::simd_set<T, SZ>(values)) {}

    /**
     * @brief assignment operator
     * @param other: another vector object
     * @return self
     */
    Vec4& operator = (const Vec4& other)
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
    void set(float x, float y, float z, float w)
    {
        data = treejuce::simd_set<T, SZ> (x, y, z, w);
    }

    /**
     * @brief set vector contents
     * @param values: an array storing x, y, z and w
     */
    void set(const float* values)
    {
        data = treejuce::simd_set<T, SZ> (values);
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
        return treejuce::simd_get_one<0, float>(data);
    }

    /**
     * @brief get the value of 2nd (y) component
     * @return y
     */
    T get_y() const
    {
        return treejuce::simd_get_one<1, float>(data);
    }

    /**
     * @brief get the value of 3rd (z) component
     * @return z
     */
    T get_z() const
    {
        return treejuce::simd_get_one<2, float>(data);
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const
    {
        return treejuce::simd_get_one<3, float>(data);
    }

    /**
     * @brief set the value of 1st (x) component
     * @param value: the value to be set to x component
     */
    void set_x(T value)
    {
        treejuce::simd_set_one<0>(data, value);
    }

    /**
     * @brief set the value of 2nd (y) component
     * @param value: the value to be set to y component
     */
    void set_y(T value)
    {
        treejuce::simd_set_one<1>(data, value);
    }

    /**
     * @brief set the value of 3rd (z) component
     * @param value: the value to be set to z component
     */
    void set_z(T value)
    {
        treejuce::simd_set_one<2>(data, value);
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w(T value)
    {
        treejuce::simd_set_one<3>(data, value);
    }

    /**
     * @brief vector add by another vector
     * @param other: the addend to be added to self
     * @return self
     */
    Vec4& operator += (const Vec4& other)
    {
        data = treejuce::simd_add<T>(data, other.data);
        return *this;
    }

    /**
     * @brief vector sub by another vector
     * @param other: the subtrahend
     * @return self
     */
    Vec4& operator -= (const Vec4& other)
    {
        data = treejuce::simd_sub<T>(data, other.data);
        return *this;
    }

    /**
     * @brief scale by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator *= (T value)
    {
        treejuce::SIMDType<SZ> tmp = treejuce::simd_set<T, SZ> (value);
        data = treejuce::simd_mul<T>(data, tmp);
        return *this;
    }

    /**
     * @brief shrink by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator /= (float value)
    {
        treejuce::SIMDType<SZ> tmp = treejuce::simd_set<T, SZ>(value);
        data = treejuce::simd_div<T>(data, tmp);
        return *this;
    }

    /**
     * @brief scale vector to make its length to be 1
     * @return length before normalize
     */
    T normalize()
    {
        T len = length();
        data = treejuce::simd_div<T>(data, treejuce::simd_set<T, SZ>(len, len, len, len));
        return len;
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
        return treejuce::simd_sum<T, SZ>(treejuce::simd_mul<T, SZ>(data, data));
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
    return Vec4<T>( treejuce::simd_add<T, SZ>(a.data, b.data) );
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
    return Vec4<T>( treejuce::simd_sub<T, SZ>(a.data, b.data) );
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
    treejuce::SIMDType<SZ> tmp = treejuce::simd_set<T, SZ> (b);
    return Vec4<T>(treejuce::simd_mul<T, SZ>(a.data, tmp));
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
    treejuce::SIMDType<16> tmp = treejuce::simd_set<T, 16> (b);
    return Vec4<T>(treejuce::simd_div<T, SZ>(a.data, tmp));
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
    treejuce::SIMDType<16> tmp = treejuce::simd_mul<T>(a.data, b.data);
    return treejuce::simd_sum<T, SZ>(tmp);
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
// Res.ele[0] = A.ele[1] * B.ele[2] - A.ele[2] * B.ele[1];
// Res.ele[1] = A.ele[2] * B.ele[0] - A.ele[0] * B.ele[2];
// Res.ele[2] = A.ele[0] * B.ele[1] - A.ele[1] * B.ele[0];
// Res.ele[3] = 0.0f;
    treejuce::SIMDType<SZ> v1 = treejuce::simd_shuffle<1, 2, 0, 0>(a.data);
    treejuce::SIMDType<SZ> v2 = treejuce::simd_shuffle<2, 0, 1, 0>(b.data);

    treejuce::SIMDType<SZ> v3 = treejuce::simd_shuffle<2, 0, 1, 0>(a.data);
    treejuce::SIMDType<SZ> v4 = treejuce::simd_shuffle<1, 2, 0, 0>(b.data);

    treejuce::SIMDType<SZ> re = treejuce::simd_sub<T>(treejuce::simd_mul<T>(v1, v2),
                                               treejuce::simd_mul<T>(v3, v4));
    treejuce::simd_set_one<3, T>(re, 0);
    return Vec4<T>(re);
}

/**
 * @brief test if two Vec4 objects are identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator == (const Vec4<T>& a, const Vec4<T>& b)
{
    die("vec4 == not implemented");
    return false;
}

/**
 * @brief test if two Vec4 objects are not identical
 */
template<typename T, int SZ = sizeof(T)*4>
bool operator != (const Vec4<T>& a, const Vec4<T>& b)
{
    die("vec4 != not implemented");
    return false;
}

typedef Vec4<float> Vec4f;
typedef Vec4<std::int32_t> Vec4i;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VEC4_H
