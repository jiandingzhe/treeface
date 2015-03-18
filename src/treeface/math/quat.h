#ifndef TREEFACE_QUAT_H
#define TREEFACE_QUAT_H

#include "treeface/math/vec4.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T) * 4>
struct Quat
{
    typedef SIMDType<SZ> DataType;

    Quat(): data(simd_set<T, SZ>(0, 0, 0, 1)) {}
    Quat(SIMDType<SZ> value): data(value) {}
    Quat(T x, T y, T z, T w): data(simd_set<T, SZ>(x, y, z, w)) {}

    Quat(T angle, Vec4<T> axis)
    {
        set_angle_axis(angle, axis);
    }

    /**
     * @brief get the value of 1st (x) axis component
     * @return x
     */
    T get_x() const
    {
        return simd_get_one<0, float>(data);
    }

    /**
     * @brief get the value of 2nd (y) axis component
     * @return y
     */
    T get_y() const
    {
        return simd_get_one<1, float>(data);
    }

    /**
     * @brief get the value of 3rd (z) axis component
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

    void get_angle_axis(T& angle, Vec4<T>& axis)
    {
        T cos_value = simd_get_one<3, float>(data);
        angle = std::acos(cos_value) * 2;

        T sin_value = std::sqrt(1 - cos_value * cos_value);
        axis.data = data;
        axis.data = simd_div<T>(axis.data, simd_set<T, SZ>(sin_value, sin_value, sin_value, sin_value));
        axis.set_w(0);
    }

    /**
     * @brief set the value of 1st (x) axis component
     * @param value: the value to be set to x component
     */
    void set_x(T value)
    {
        data = simd_set_one<0>(data, value);
    }

    /**
     * @brief set the value of 2nd (y) axis component
     * @param value: the value to be set to y component
     */
    void set_y(T value)
    {
        data = simd_set_one<1>(data, value);
    }

    /**
     * @brief set the value of 3rd (z) axis component
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

    void set_angle_axis(T angle, Vec4<T> axis)
    {
        axis.set_w(0);
        axis.normalize();

        axis *= std::sin(angle / 2);
        data = axis.data;

        data = simd_set_one<3, T, SZ>(data, std::cos(angle / 2));
    }

    T normalize()
    {
        T len = length();
        data = simd_div<T>(data, simd_set<T, SZ>(len, len, len, len));
        return len;
    }

    T length()
    {
        return std::sqrt(length2());
    }

    T length2()
    {
        return simd_sum<T>(simd_mul<T>(data, data));
    }

    Vec4<T> rotate(Vec4<T> input)
    {
        SIMDType<SZ> tmp = simd_mul<T>(data, simd_set<T, SZ>(-1, -1, -1, 1));
        Quat<T> re =  *this * Quat<T>(input.data) * Quat<T>(tmp);
        return Vec4<T>(re.data);
    }

    DataType data;
};

template<typename T, int SZ = sizeof(T) * 4>
Quat<T> operator * (Quat<T> a, Quat<T> b)
{
    //res.x = + a.x*b.w + a.y*b.z - a.z*b.y + a.w*b.x;
    //res.y = - a.x*b.z + a.y*b.w + a.z*b.x + a.w*b.y;
    //res.z = + a.x*b.y - a.y*b.x + a.z*b.w + a.w*b.z;
    //res.w = - a.x*b.x - a.y*b.y - a.z*b.z + a.w*b.w;

    Quat<T> result;

    T av = a.get_x();
    SIMDType<SZ> tmp1 = simd_set<T, SZ>(av, -av, av, -av);
    SIMDType<SZ> tmp2 = simd_shuffle<3, 2, 1, 0>(b.data);
    result.data = simd_mul<T>(tmp1, tmp2);

    av = a.get_y();
    tmp1 = simd_set<T, SZ>(av, av, -av, -av);
    tmp2 = simd_shuffle<2, 3, 0, 1>(b.data);
    result.data = simd_add<T>(result.data, simd_mul<T>(tmp1, tmp2));

    av = a.get_z();
    tmp1 = simd_set<T, SZ>(-av, av, av, -av);
    tmp2 = simd_shuffle<1, 0, 3, 2>(b.data);
    result.data = simd_add<T>(result.data, simd_mul<T>(tmp1, tmp2));

    av = a.get_w();
    tmp1 = simd_set<T, SZ>(av, av, av, av);
    // no shuffle needed, use b data directly
    result.data = simd_add<T>(result.data, simd_mul<T>(tmp1, b.data));

    return result;
}

typedef Quat<float> Quatf;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_QUAT_H
