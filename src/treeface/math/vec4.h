#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>
#include "treeface/common.h"
#include <treecore/AlignedMalloc.h>
#include <treecore/PlatformDefs.h>
#include <treecore/SimdFunc.h>
#include <treecore/SimilarFloat.h>

namespace treeface {


/**
 * 4-dimentional vector class implemented with SIMD operations.
 *
 * The layout of Vec4 is compatiable with OpenGL, so the data can be directly
 * transmitted to GL server side buffers or program attributes.
 */
TREECORE_ALN_BEGIN(16)
template<typename T>
struct Vec4
{
    typedef typename treecore::similar_float<T>::type FloatType;
    typedef treecore::SIMDType<sizeof(T) * 4> DataType;

    TREECORE_ALIGNED_ALLOCATOR(Vec4);

    /**
     * @brief default constructor
     *
     * values are initialized as zero point
     */
    Vec4()
    {
        treecore::simd_set_all<T>(data, T(0), T(0), T(0), T(1));
    }

    /**
     * @brief constructor with values specified
     * @param value: a SIMDType containing x, y, z and w
     */
    Vec4(const DataType& value): data(value) {}

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
    Vec4(T x, T y, T z, T w)
    {
        treecore::simd_set_all<T>(data, x, y, z, w);
    }

    /**
     * @brief constructor with values specified
     * @param values: an array storing reversed w, z, y, x
     */
    Vec4(const T* values)
    {
        treecore::simd_set_all<T>(data, values);
    }

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
    void set(T x, T y, T z, T w)
    {
        treecore::simd_set_all<T> (data, x, y, z, w);
    }

    /**
     * @brief set vector contents
     * @param values: an array storing x, y, z and w
     */
    void set(const T* values)
    {
        treecore::simd_set_all<T> (data, values);
    }

    /**
     * @brief get the value of 1st (x) component
     * @return x
     */
    T get_x() const
    {
        return treecore::simd_get_one<0, float>(data);
    }

    /**
     * @brief get the value of 2nd (y) component
     * @return y
     */
    T get_y() const
    {
        return treecore::simd_get_one<1, float>(data);
    }

    /**
     * @brief get the value of 3rd (z) component
     * @return z
     */
    T get_z() const
    {
        return treecore::simd_get_one<2, float>(data);
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const
    {
        return treecore::simd_get_one<3, float>(data);
    }

    /**
     * @brief set the value of 1st (x) component
     * @param value: the value to be set to x component
     */
    void set_x(T value)
    {
        treecore::simd_set_one<0>(data, value);
    }

    /**
     * @brief set the value of 2nd (y) component
     * @param value: the value to be set to y component
     */
    void set_y(T value)
    {
        treecore::simd_set_one<1>(data, value);
    }

    /**
     * @brief set the value of 3rd (z) component
     * @param value: the value to be set to z component
     */
    void set_z(T value)
    {
        treecore::simd_set_one<2>(data, value);
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w(T value)
    {
        treecore::simd_set_one<3>(data, value);
    }

    /**
     * @brief vector add by another vector
     * @param other: the addend to be added to self
     * @return self
     */
    Vec4& operator += (const Vec4& other)
    {
        DataType result;
        treecore::simd_add<T>(result, data, other.data);
        data = result;
        return *this;
    }

    /**
     * @brief vector sub by another vector
     * @param other: the subtrahend
     * @return self
     */
    Vec4& operator -= (const Vec4& other)
    {
        DataType result;
        treecore::simd_sub<T>(result, data, other.data);
        data = result;
        return *this;
    }

    /**
     * @brief scale by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator *= (T value)
    {
        DataType result;
        DataType tmp;
        treecore::simd_broadcast<T> (tmp, value);
        treecore::simd_mul<T>(result, data, tmp);
        data = result;
        return *this;
    }

    /**
     * @brief shrink by a scalar value
     * @param value: the scale
     * @return self
     */
    Vec4& operator /= (T value)
    {
        DataType result;
        DataType tmp;
        treecore::simd_broadcast<T> (tmp, value);
        treecore::simd_div<T>(result, data, tmp);
        data = result;
        return *this;
    }

    /**
     * @brief scale vector to make its length to be 1
     * @return length before normalize
     */
    FloatType normalize()
    {
        FloatType len = (T) length();
        DataType tmp;
        treecore::simd_broadcast<T>(tmp, T(len));
        DataType result;
        treecore::simd_div<T>(result, data, tmp);
        data = result;
        return len;
    }

    /**
     * @brief get vector length
     * @return length
     */
    FloatType length() const
    {
        return std::sqrt(length2());
    }

    /**
     * @brief get length square
     * @return length square
     */
    FloatType length2() const
    {
        DataType tmp;
        treecore::simd_mul<T>(tmp, data, data);
        return (FloatType) treecore::simd_sum<T>(tmp);
    }

    /**
     * @brief the underlying data storing all vector compnents.
     *
     * The memory layout should be compatible with OpenGL, so that it can be
     * transmitted to GL buffers or program attributes directly.
     */
    DataType data;
} TREECORE_ALN_END(16);

/**
 * @brief vector add
 *
 * @param a: augend
 * @param b: addend
 * @return a new Vec4 object containing result
 */
template<typename T>
Vec4<T> operator + (const Vec4<T>& a, const Vec4<T>& b)
{
    Vec4<T> result;
    treecore::simd_add<T>(result.data, a.data, b.data);
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
Vec4<T> operator - (const Vec4<T>& a, const Vec4<T>& b)
{
    Vec4<T> result;
    treecore::simd_sub<T>(result.data, a.data, b.data);
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
Vec4<T> operator * (const Vec4<T>& a, T b)
{
    typename Vec4<T>::DataType tmp;
    treecore::simd_broadcast<T>(tmp, b);
    typename Vec4<T>::DataType result;
    treecore::simd_mul<T>(result, a.data, tmp);
    return Vec4<T>(result);
}

/**
 * @brief shrink with a scalar
 *
 * @param a: the vector value
 * @param b: scale
 * @return a new Vec4 object containing shrinked values
 */
template<typename T>
Vec4<T> operator / (const Vec4<T>& a, T b)
{
    typename Vec4<T>::DataType tmp;
    treecore::simd_broadcast<T>(tmp, b);
    typename Vec4<T>::DataType result;
    treecore::simd_div<T>(result, a.data, tmp);
    return Vec4<T>(result);
}

/**
 * @brief dot multiply
 *
 * @param a: one vector value
 * @param b: the other vector value
 * @return a scalar value containing dot product
 */
template<typename T>
T operator * (const Vec4<T>& a, const Vec4<T>& b)
{
    typename Vec4<T>::DataType result;
    treecore::simd_mul<T>(result.data, a.data, b.data);
    return result;
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
    typename Vec4<T>::DataType v1 = a.data;
    typename Vec4<T>::DataType v2 = b.data;
    typename Vec4<T>::DataType v3 = a.data;
    typename Vec4<T>::DataType v4 = b.data;

    treecore::simd_shuffle<1, 2, 0, 0>(v1);
    treecore::simd_shuffle<2, 0, 1, 0>(v2);
    treecore::simd_shuffle<2, 0, 1, 0>(v3);
    treecore::simd_shuffle<1, 2, 0, 0>(v4);

    typename Vec4<T>::DataType tmp1;
    typename Vec4<T>::DataType tmp2;
    typename Vec4<T>::DataType re;

    treecore::simd_mul<T>(tmp1, v1, v2);
    treecore::simd_mul<T>(tmp2, v3, v4);
    treecore::simd_sub<T>(re, tmp1, tmp2);
    treecore::simd_set_one<3, T>(re, 0);

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

} // namespace treeface

#endif // TREEFACE_VEC4_H
