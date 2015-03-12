#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

#include <cmath>

namespace treeface
{

template <typename T>
struct Vec4
{
    Vec4() {}

    Vec4(const Vec4& other)
    {
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        data[3] = other.data[3];
    }

    Vec4(T x, T y, T z, T w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }

    Vec4(const T* values)
    {
        data[0] = values[0];
        data[1] = values[1];
        data[2] = values[2];
        data[3] = values[3];
    }

    Vec4& operator = (const Vec4& other)
    {
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        data[3] = other.data[3];
        return *this;
    }

    void set(T x, T y, T z, T w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }

    void set(const T* values)
    {
        data[0] = values[0];
        data[1] = values[1];
        data[2] = values[2];
        data[3] = values[3];
    }

    void set(const Vec4& other)
    {
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        data[3] = other.data[3];
    }

    Vec4& operator += (const Vec4& other)
    {
        data[0] += other.data[0];
        data[1] += other.data[1];
        data[2] += other.data[2];
        data[3] += other.data[3];
        return *this;
    }

    Vec4& operator -= (const Vec4& other)
    {
        data[0] -= other.data[0];
        data[1] -= other.data[1];
        data[2] -= other.data[2];
        data[3] -= other.data[3];
        return *this;
    }

    Vec4& operator *= (T value)
    {
        data[0] *= value;
        data[1] *= value;
        data[2] *= value;
        data[3] *= value;
        return *this;
    }

    Vec4& operator /= (T value)
    {
        data[0] /= value;
        data[1] /= value;
        data[2] /= value;
        data[3] /= value;
        return *this;
    }

    T normalize()
    {
        T len = length();
        data[0] /= len;
        data[1] /= len;
        data[2] /= len;
        data[3] /= len;
        return len;
    }

    T length()
    {
        return std::sqrt(length2());
    }

    T length2()
    {
        return data[0] * data[0] +
               data[1] * data[1] +
               data[2] * data[2] +
               data[3] * data[3];
    }

    T data[4] = {0, 0, 0, 1};
};

template<typename T>
bool operator == (const Vec4<T>& a, const Vec4<T>& b)
{
    return (a.data[0] == b.data[0] &&
            a.data[1] == b.data[1] &&
            a.data[2] == b.data[2] &&
            a.data[3] == b.data[3]);
}

template<typename T>
bool operator != (const Vec4<T>& a, const Vec4<T>& b)
{
    return (a.data[0] != b.data[0] ||
            a.data[1] != b.data[1] ||
            a.data[2] != b.data[2] ||
            a.data[3] != b.data[3]);
}

template<typename T>
Vec4<T> operator + (const Vec4<T>& a, const Vec4<T>& b)
{
    Vec4<T> re(a);
    re += b;
    return re;
}

template<typename T>
Vec4<T> operator - (const Vec4<T>& a, const Vec4<T>& b)
{
    Vec4<T> re(a);
    re -= b;
    return re;
}

template<typename T>
T operator * (const Vec4<T>& a, const Vec4<T>& b)
{
    return a.data[0] * b.data[0] +
           a.data[1] * b.data[1] +
           a.data[2] * b.data[2] +
           a.data[3] * b.data[3];
}

template<typename T>
Vec4<T> operator ^ (const Vec4<T>& a, const Vec4<T>& b)
{
    return Vec4<T>(a.data[1]*b.data[2] - a.data[2]*b.data[1],
                   a.data[2]*b.data[0] - a.data[0]*b.data[2],
                   a.data[0]*b.data[1] - a.data[1]*b.data[0],
                   0);
}

typedef Vec4<float> Vec4f;
typedef Vec4<double> vec4d;

} // namespace treeface

#endif // TREEFACE_VEC4_H
