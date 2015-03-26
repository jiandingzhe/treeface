#ifndef TREEFACE_QUAT_H
#define TREEFACE_QUAT_H

#include "treeface/math/vec4.h"

TREEFACE_NAMESPACE_BEGIN

/**
 * Quaternions are frequently used in 3D rotation.
 */
template<typename T, int SZ = sizeof(T) * 4>
struct Quat
{
    typedef treejuce::SIMDType<SZ> DataType;

    /**
     * @brief create quaternion representing zero rotation
     */
    Quat(): data(treejuce::simd_set<T, SZ>(0, 0, 0, 1)) {}

    /**
     * @brief crete quaternion with value specified
     * @param value
     */
    Quat(const treejuce::SIMDType<SZ>& value): data(value) {}

    /**
     * @brief create quaternion by specifying all four components
     * @param x: component 1
     * @param y: component 2
     * @param z: component 3
     * @param w: component 4, represents cos(a/2)
     */
    Quat(T x, T y, T z, T w): data(treejuce::simd_set<T, SZ>(x, y, z, w)) {}

    /**
     * @brief create quaternion with rotation specified by right-hand angle and axis
     * @param angle: rotation around axis
     * @param axis: direction of rotaton axis. The 4rd component will be omitted
     */
    Quat(T angle, Vec4<T> axis)
    {
        set_angle_axis(angle, axis);
    }

    /**
     * @brief get the value of 1st (x) axis component
     * @return x
     */
    T get_x() const NOEXCEPT
    {
        return treejuce::simd_get_one<0, float>(data);
    }

    /**
     * @brief get the value of 2nd (y) axis component
     * @return y
     */
    T get_y() const NOEXCEPT
    {
        return treejuce::simd_get_one<1, float>(data);
    }

    /**
     * @brief get the value of 3rd (z) axis component
     * @return z
     */
    T get_z() const NOEXCEPT
    {
        return treejuce::simd_get_one<2, float>(data);
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const NOEXCEPT
    {
        return treejuce::simd_get_one<3, float>(data);
    }

    /**
     * @brief get rotation represented by this quaternion
     * @param angle: variable for storing angle
     * @param axis: variable for storing axis in (x, y, z, 0)
     */
    void get_angle_axis(T& angle, Vec4<T>& axis) const NOEXCEPT
    {
        T cos_value = treejuce::simd_get_one<3, float>(data);
        angle = std::acos(cos_value) * 2;

        T sin_value = std::sqrt(1 - cos_value * cos_value);
        axis.data = data;
        axis.data = treejuce::simd_div<T>(axis.data, treejuce::simd_set<T, SZ>(sin_value, sin_value, sin_value, sin_value));
        axis.set_w(0);
    }

    /**
     * @brief set the value of 1st (x) axis component
     * @param value: the value to be set to x component
     */
    void set_x(T value) NOEXCEPT
    {
        treejuce::simd_set_one<0>(data, value);
    }

    /**
     * @brief set the value of 2nd (y) axis component
     * @param value: the value to be set to y component
     */
    void set_y(T value) NOEXCEPT
    {
        treejuce::simd_set_one<1>(data, value);
    }

    /**
     * @brief set the value of 3rd (z) axis component
     * @param value: the value to be set to z component
     */
    void set_z(T value) NOEXCEPT
    {
        treejuce::simd_set_one<2>(data, value);
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w(T value) NOEXCEPT
    {
        treejuce::simd_set_one<3>(data, value);
    }

    /**
     * @brief set rotation by specifying right-hand angle and axis
     * @param angle: rotation around axis
     * @param axis: direction of rotation axis. The 4rd component will be omitted.
     */
    void set_angle_axis(T angle, Vec4<T> axis) NOEXCEPT
    {
        axis.set_w(0);
        axis.normalize();

        axis *= std::sin(angle / 2);
        data = axis.data;

        treejuce::simd_set_one<3, T, SZ>(data, std::cos(angle / 2));
    }

    /**
     * @brief set values to (-x, -y, -z, w)
     */
    void inverse() NOEXCEPT;

    /**
     * @brief make quaternion length to be one
     * @return length before normalize
     */
    T normalize() NOEXCEPT
    {
        T len = length();
        data = treejuce::simd_div<T>(data, treejuce::simd_set<T, SZ>(len, len, len, len));
        return len;
    }

    /**
     * @brief get quaternion length
     * @return length value
     */
    T length() const NOEXCEPT
    {
        return std::sqrt(length2());
    }

    /**
     * @brief get length square
     * @return length * length
     */
    T length2() const NOEXCEPT
    {
        return treejuce::simd_sum<T>(treejuce::simd_mul<T>(data, data));
    }

    /**
     * @brief apply rotation represented by this quaternion to a vector
     * @param input: vector to be rotated. To get correct result, the 4rd
     *        component of input vector should be zero.
     * @return rotated vector
     */
    Vec4<T> rotate(Vec4<T> input) const NOEXCEPT
    {
        Quat<T> inv(data);
        inv.inverse();
        Quat<T> re =  *this * Quat<T>(input.data) * inv;
        return Vec4<T>(re.data);
    }

    DataType data;
};

template<>
inline void Quat<float>::inverse() NOEXCEPT
{
    // directly operate on float's sign bit
    data = treejuce::simd_xor<float>(data, treejuce::simd_set<std::int32_t, 16>(0x80000000, 0x80000000, 0x80000000, 0x00000000));
}

/**
 * @brief quaternion multiply
 */
template<typename T, int SZ = sizeof(T) * 4>
inline Quat<T> operator * (const Quat<T>& a, const Quat<T>& b) NOEXCEPT
{
    //res.x = + a.x*b.w + a.y*b.z - a.z*b.y + a.w*b.x;
    //res.y = - a.x*b.z + a.y*b.w + a.z*b.x + a.w*b.y;
    //res.z = + a.x*b.y - a.y*b.x + a.z*b.w + a.w*b.z;
    //res.w = - a.x*b.x - a.y*b.y - a.z*b.z + a.w*b.w;

    Quat<T> result;

    T av = a.get_x();
    treejuce::SIMDType<SZ> tmp1 = treejuce::simd_set<T, SZ>(av, -av, av, -av);
    treejuce::SIMDType<SZ> tmp2 = treejuce::simd_shuffle<3, 2, 1, 0>(b.data);
    result.data = treejuce::simd_mul<T>(tmp1, tmp2);

    av = a.get_y();
    tmp1 = treejuce::simd_set<T, SZ>(av, av, -av, -av);
    tmp2 = treejuce::simd_shuffle<2, 3, 0, 1>(b.data);
    result.data = treejuce::simd_add<T>(result.data, treejuce::simd_mul<T>(tmp1, tmp2));

    av = a.get_z();
    tmp1 = treejuce::simd_set<T, SZ>(-av, av, av, -av);
    tmp2 = treejuce::simd_shuffle<1, 0, 3, 2>(b.data);
    result.data = treejuce::simd_add<T>(result.data, treejuce::simd_mul<T>(tmp1, tmp2));

    av = a.get_w();
    tmp1 = treejuce::simd_set<T, SZ>(av, av, av, av);
    // no shuffle needed, use b data directly
    result.data = treejuce::simd_add<T>(result.data, treejuce::simd_mul<T>(tmp1, b.data));

    return result;
}

typedef Quat<float> Quatf;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_QUAT_H
