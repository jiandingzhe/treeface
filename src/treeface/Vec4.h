#ifndef TREEFACE_VEC4_H
#define TREEFACE_VEC4_H

namespace treeface
{

template <typename T>
struct Vec4
{
    Vec4() {}

    Vec4(const Vec4& other) : data(other.data) {}

    Vec4(T x, T y, T z, T w): data({x, y, z, w}) {}

    Vec4& operator = (const Vec4& other)
    {
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        data[3] = other.data[3];
        return *this;
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

    T data[4] = {0, 0, 0, 1};
};

typedef Vec4<float> Vec4f;
typedef Vec4<double> vec4d;

} // namespace treeface

#endif // TREEFACE_VEC4_H
