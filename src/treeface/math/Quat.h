#ifndef TREEFACE_QUAT_H
#define TREEFACE_QUAT_H

#include "treeface/math/Vec4.h"
#include <treecore/FloatUtils.h>

namespace treeface {

/**
 * Quaternions are frequently used in 3D rotation.
 */
template<typename T>
struct Quat
{
    typedef treecore::SimdObject<T, 4> DataType;
    typedef typename treecore::similar_float<T>::type FloatType;

    TREECORE_ALIGNED_ALLOCATOR(Quat);

    /**
     * @brief create quaternion representing zero rotation
     */
    Quat(): data(T(0), T(0), T(0), T(1)) {}

    /**
     * @brief crete quaternion with value specified
     * @param value
     */
    Quat(const DataType& value): data(value) {}
    Quat(const typename DataType::DataType& value): data(value) {}

    /**
     * @brief create quaternion by specifying all four components
     * @param x: component 1
     * @param y: component 2
     * @param z: component 3
     * @param w: component 4, represents cos(a/2)
     */
    Quat(T x, T y, T z, T w): data(x, y, z, w) {}

    /**
     * @brief create quaternion with rotation specified by right-hand angle and axis
     * @param angle: rotation around axis
     * @param axis: direction of rotaton axis. The 4rd component will be omitted
     */
    Quat(T angle, const Vec3<T>& axis)
    {
        set_angle_axis(angle, axis);
    }

    Quat(T angle, const Vec4<T>& axis)
    {
        set_angle_axis(angle, axis);
    }

    /**
     * @brief get the value of 1st (x) axis component
     * @return x
     */
    T get_x() const noexcept
    {
        return data.template get<0>();
    }

    /**
     * @brief get the value of 2nd (y) axis component
     * @return y
     */
    T get_y() const noexcept
    {
        return data.template get<1>();
    }

    /**
     * @brief get the value of 3rd (z) axis component
     * @return z
     */
    T get_z() const noexcept
    {
        return data.template get<2>();
    }

    /**
     * @brief get the value of 4th (w) component
     * @return w
     */
    T get_w() const noexcept
    {
        return data.template get<3>();
    }

    /**
     * @brief get rotation represented by this quaternion
     * @param angle: variable for storing angle
     * @param axis: variable for storing axis in (x, y, z, 0)
     */

    void get_angle_axis(T& angle, Vec3<T>& axis) const noexcept
    {
        T cos_d2 = data.template get<3>();
        T sin_d2 = std::sqrt(1 - cos_d2 * cos_d2);

        angle = std::acos(cos_d2) * 2;

        DataType tmp = data / DataType(sin_d2);
        axis.set(tmp.template get<0>(), tmp.template get<1>(), tmp.template get<2>());
    }

    void get_angle_axis(T& angle, Vec4<T>& axis) const noexcept
    {
        Vec3<T> tmp_axis;
        get_angle_axis(angle, tmp_axis);
        axis.set_direction(tmp_axis);
    }

    /**
     * @brief set the value of 1st (x) axis component
     * @param value: the value to be set to x component
     */
    void set_x(T value) noexcept
    {
        data.template set<0>(value);
    }

    /**
     * @brief set the value of 2nd (y) axis component
     * @param value: the value to be set to y component
     */
    void set_y(T value) noexcept
    {
        data.template set<1>(value);
    }

    /**
     * @brief set the value of 3rd (z) axis component
     * @param value: the value to be set to z component
     */
    void set_z(T value) noexcept
    {
        data.template set<2>(value);
    }

    /**
     * @brief set the value of 4th (w) component
     * @param value: the value to be set to w component
     */
    void set_w(T value) noexcept
    {
        data.template set<3>(value);
    }

    /**
     * @brief set rotation by specifying right-hand angle and axis
     * @param angle: rotation around axis
     * @param axis: direction of rotation axis. The 4rd component will be omitted.
     */

    void set_angle_axis(T angle, const Vec3<T>& axis) noexcept
    {
        T sine = std::sin(angle/2);

        Vec3<T> tmp = axis;
        tmp.normalize();
        tmp *= sine;

        data.set_all( tmp.x, tmp.y, tmp.z, std::sqrt(T(1) - sine*sine) );
    }

    void set_angle_axis(T angle, const Vec4<T>& axis) noexcept
    {
        set_angle_axis(angle, Vec3<T>(axis.get_x(), axis.get_y(), axis.get_z()) );
    }

    /**
     * @brief set values to (-x, -y, -z, w)
     */
    void inverse() noexcept;

    /**
     * @brief make quaternion length to be one
     * @return length before normalize
     */
    FloatType normalize() noexcept
    {
        FloatType len = length();
        data /= DataType(T(len));
        return len;
    }

    /**
     * @brief get quaternion length
     * @return length value
     */
    FloatType length() const noexcept
    {
        return std::sqrt(length2());
    }

    /**
     * @brief get length square
     * @return length * length
     */
    FloatType length2() const noexcept
    {
        return (data * data).sum();
    }

    /**
     * @brief apply rotation represented by this quaternion to a vector
     * @param input: vector to be rotated. To get correct result, the 4rd
     *        component of input vector should be zero.
     * @return rotated vector
     */
    Vec4<T> rotate(Vec4<T> input) const noexcept
    {
        Quat<T> inv(data);
        inv.inverse();

        Quat<T> re =  *this * Quat<T>(input.data) * inv;
        return Vec4<T>(re.data);
    }

    DataType data;
} TREECORE_ALN_END(16);

template<>
inline void Quat<float>::inverse() noexcept
{
    // directly operate on float's sign bit
    data ^= treecore::SimdObject<treecore::int32, 4>(0x80000000, 0x80000000, 0x80000000, 0x00000000);
}

/**
 * @brief quaternion multiply
 */
template<typename T, int SZ = sizeof(T) * 4>
inline Quat<T> operator * (const Quat<T>& a, const Quat<T>& b) noexcept
{
    //res.x = + a.x*b.w + a.y*b.z - a.z*b.y + a.w*b.x;
    //res.y = - a.x*b.z + a.y*b.w + a.z*b.x + a.w*b.y;
    //res.z = + a.x*b.y - a.y*b.x + a.z*b.w + a.w*b.z;
    //res.w = - a.x*b.x - a.y*b.y - a.z*b.z + a.w*b.w;

    Quat<T> result;
    typename Quat<T>::DataType tmp1;
    typename Quat<T>::DataType tmp2;

    T av = a.get_x();
    tmp1.set_all(av, -av, av, -av);
    tmp2 = b.data;
    tmp2.template shuffle<3, 2, 1, 0>();
    result.data = tmp1 * tmp2;

    av = a.get_y();
    tmp1.set_all(av, av, -av, -av);
    tmp2 = b.data;
    tmp2.template shuffle<2, 3, 0, 1>();
    result.data += tmp1 * tmp2;

    av = a.get_z();
    tmp1.set_all(-av, av, av, -av);
    tmp2 = b.data;
    tmp2.template shuffle<1, 0, 3, 2>();
    result.data += tmp1 * tmp2;

    av = a.get_w();
    tmp1.set_all(av, av, av, av);
    tmp2 = b.data;
    result.data += tmp1 * tmp2;

    return result;
}

typedef Quat<float> Quatf;

} // namespace treeface

#endif // TREEFACE_QUAT_H
