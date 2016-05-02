#ifndef TREEFACE_MAT2_H
#define TREEFACE_MAT2_H

#include "treeface/math/Vec2.h"

#include <treecore/SimdObject.h>

#include <cmath>

namespace treeface
{

template<typename T>
struct Mat2
{
    typedef treecore::SimdObject<T, 4> DataType;

    Mat2() noexcept: data(T(1), T(0), T(0), T(1))
    {
    }

    Mat2(const DataType& data) noexcept: data(data)
    {
    }

    Mat2(const Mat2& peer) = default;

    Mat2(T col0_0, T col0_1, T col1_0, T col1_1) noexcept
        : data(col0_0, col0_1, col1_0, col1_1)
    {
    }

    Mat2& operator = (const Mat2& peer) = default;

    void set_rotate(T angle_rad) noexcept
    {
        T sine = std::sin(angle_rad);
        T cosine = std::cos(angle_rad);
        data.set_all(cosine, sine, -sine, cosine);
    }

    T determinant() const noexcept
    {
        return data.template get<0>() * data.template get<3>() - data.template get<1>() * data.template get<2>();
    }

    void transpose() noexcept
    {
        data.template shuffle<0, 2, 1, 3>();
    }

    T inverse() noexcept;

    DataType data;
};

typedef Mat2<float> Mat2f;

extern template
float Mat2f::inverse() noexcept;

template<typename T>
Mat2<T> operator * (const Mat2<T>& a, const Mat2<T>& b) noexcept;

extern template
Mat2f operator * (const Mat2f& a, const Mat2f& b) noexcept;

template<typename T>
Vec2<T> operator * (const Mat2<T>& mat, const Vec2<T>& vec) noexcept;

extern template
Vec2f operator * (const Mat2f& mat, const Vec2f& vec) noexcept;

} // namespace treeface

#endif // TREEFACE_MAT2_H
