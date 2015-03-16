#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>

#include "treeface/common.h"
#include "treeface/simd.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T)*4>
struct Vec4
{
    typedef SIMDType<SZ> DataType;

    Vec4(): data(simd_set<T, SZ>(0, 0, 0, 1)) {}

    Vec4(SIMDType<SZ> value): data(value) {}

    Vec4(const Vec4& other): data(other.data) {}

    Vec4(T x, T y, T z, T w): data(simd_set<T, SZ> (x, y, z, w)) {}

    Vec4(const T* values): data(simd_set<T, SZ>(values)) {}

    Vec4& operator = (const Vec4& other)
    {
        data = other.data;
    }

    void set(float x, float y, float z, float w)
    {
        data = simd_set<T, SZ> (x, y, z, w);
    }

    void set(const float* values)
    {
        data = simd_set<T, SZ> (values);
    }

    void set(const Vec4& other)
    {
        data = other.data;
    }

    T get_x() const
    {
        return simd_get_one<0, float>(data);
    }

    T get_y() const
    {
        return simd_get_one<1, float>(data);
    }

    T get_z() const
    {
        return simd_get_one<2, float>(data);
    }

    T get_w() const
    {
        return simd_get_one<3, float>(data);
    }

    void set_x(T value)
    {
        data = simd_set_one<0>(data, value);
    }

    void set_y(T value)
    {
        data = simd_set_one<1>(data, value);
    }

    void set_z(T value)
    {
        data = simd_set_one<2>(data, value);
    }

    void set_w(T value)
    {
        data = simd_set_one<3>(data, value);
    }

    Vec4& operator += (const Vec4& other)
    {
        data = simd_add<T>(data, other.data);
    }

    Vec4& operator -= (const Vec4& other)
    {
        data = simd_sub<T>(data, other.data);
    }

    Vec4& operator *= (T value)
    {
        SIMDType<SZ> tmp = simd_set<T, SZ> (value);
        data = simd_mul<T>(data, tmp);
    }

    Vec4& operator /= (float value)
    {
        SIMDType<SZ> tmp = simd_set<T, SZ>(value);
        data = simd_div<T>(data, tmp);
    }

    T normalize()
    {
        SIMDType<SZ> tmp = simd_set<T, SZ>(length());
        data = simd_div<T>(data, tmp);
    }

    T length() const
    {
        return std::sqrt(length2());
    }

    T length2() const
    {
        return simd_sum<T, SZ>(simd_mul<T, SZ>(data, data));
    }

    DataType data;
};

template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator + (const Vec4<T>& a, const Vec4<T>& b)
{
    return Vec4<T>( simd_add<T, SZ>(a.data, b.data) );
}

template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator - (const Vec4<T>& a, const Vec4<T>& b)
{
    return Vec4<T>( simd_sub<T, SZ>(a.data, b.data) );
}

template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator * (const Vec4<T>& a, T b)
{
    SIMDType<SZ> tmp = simd_set<T, SZ> (b);
    return Vec4<T>(simd_mul<T, SZ>(a.data, tmp));
}

template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator / (const Vec4<T>& a, T b)
{
    SIMDType<16> tmp = simd_set<T, 16> (b);
    return Vec4<T>(simd_div<T, SZ>(a.data, tmp));
}

template<typename T, int SZ = sizeof(T)*4>
T operator * (const Vec4<T>& a, const Vec4<T>& b)
{
    SIMDType<16> tmp = simd_mul<T>(a.data, b.data);
    return simd_sum<T, SZ>(tmp);
}

template<typename T, int SZ = sizeof(T)*4>
Vec4<T> operator ^ (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

template<typename T, int SZ = sizeof(T)*4>
bool operator == (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

template<typename T, int SZ = sizeof(T)*4>
bool operator != (const Vec4<T>& a, const Vec4<T>& b)
{
    abort();
}

typedef Vec4<float> Vec4f;
typedef Vec4<std::int32_t> Vec4i;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VEC4_H
