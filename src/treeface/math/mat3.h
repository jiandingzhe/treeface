#ifndef TREEFACE_MAT3_H
#define TREEFACE_MAT3_H

#include <cstring>
#include "treeface/common.h"
#include "treeface/math/vec3.h"
#include "treeface/math/matutils.h"

namespace treeface {

///
///
///
template<typename T>
struct Mat3
{
    Mat3() noexcept
    {
        data[0] = T(1);
        data[1] = T(0);
        data[2] = T(0);

        data[3] = T(0);
        data[4] = T(1);
        data[5] = T(0);

        data[6] = T(0);
        data[7] = T(0);
        data[8] = T(1);
    }

    Mat3(const Mat3& peer) = default;

    Mat3(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8) noexcept
    {
        data[0] = v0;
        data[1] = v1;
        data[2] = v2;
        data[3] = v3;
        data[4] = v4;
        data[5] = v5;
        data[6] = v6;
        data[7] = v7;
        data[8] = v8;
    }

    Mat3(const treecore::SimdObject<T,4>& col1,
         const treecore::SimdObject<T,4>& col2,
         const treecore::SimdObject<T,4>& col3) noexcept
    {
        data[0] = col1.template get<0>();
        data[1] = col1.template get<1>();
        data[2] = col1.template get<2>();

        data[3] = col2.template get<0>();
        data[4] = col2.template get<1>();
        data[5] = col2.template get<2>();

        data[6] = col3.template get<0>();
        data[7] = col3.template get<1>();
        data[8] = col3.template get<2>();
    }

    Mat3(const T* values) noexcept
    {
        memcpy(data, values, sizeof(T) * 9);
    }

    void transpose() noexcept
    {
        T tmp[9] = {data[0], data[3], data[6],
                    data[1], data[4], data[7],
                    data[2], data[5], data[8]};
        memcpy(data, tmp, 9 * sizeof(T));
    }

    T inverse() noexcept;

    T determinant() noexcept
    {
        return det3x3<T>(treecore::SimdObject<T, 4>(data[0], data[1], data[2], T(0)),
                         treecore::SimdObject<T, 4>(data[3], data[4], data[5], T(0)),
                         treecore::SimdObject<T, 4>(data[6], data[7], data[8], T(0)));
    }

    T data[9];
};

typedef Mat3<float> Mat3f;

extern template
float Mat3<float>::inverse() noexcept;

///
/// @brief matrix multiply
///
template<typename T>
Mat3<T> operator * (const Mat3<T>& a, const Mat3<T>& b) noexcept;

extern template
Mat3f operator * (const Mat3f& a, const Mat3f& b) noexcept;

///
/// @brief transform a vector
///
template<typename T>
Vec3<T> operator * (const Mat3<T>& mat, const Vec3<T>& vec) noexcept;

extern template
Vec3f operator * (const Mat3f& mat, const Vec3f& vec) noexcept;


} // namespace treeface

#endif // TREEFACE_MAT3_H
