#ifndef TREEFACE_MAT4_H
#define TREEFACE_MAT4_H

#include <treecore/AlignedMalloc.h>

#include "treeface/math/matutils.h"
#include "treeface/math/vec4.h"
#include "treeface/math/quat.h"

namespace treeface {

template<typename T>
struct Mat4
{
    typedef treecore::SimdObject<T, 4> DataType;

    TREECORE_ALIGNED_ALLOCATOR(Mat4);

    /**
     * @brief create identity matrix
     */
    Mat4()
    {
        data[0].set_all(T(1), T(0), T(0), T(0));
        data[1].set_all(T(0), T(1), T(0), T(0));
        data[2].set_all(T(0), T(0), T(1), T(0));
        data[3].set_all(T(0), T(0), T(0), T(1));
    }

    /**
     * @brief create matrix from existing one
     */
    Mat4(const Mat4& other) = default;

    /**
     * @brief create matrix specifying values in column-major
     */
    Mat4(T col0_0, T col0_1, T col0_2, T col0_3,
         T col1_0, T col1_1, T col1_2, T col1_3,
         T col2_0, T col2_1, T col2_2, T col2_3,
         T col3_0, T col3_1, T col3_2, T col3_3)
    {
        set(col0_0, col0_1, col0_2, col0_3,
            col1_0, col1_1, col1_2, col1_3,
            col2_0, col2_1, col2_2, col2_3,
            col3_0, col3_1, col3_2, col3_3);
    }

    /**
     * @brief create matrix from array of data
     * @param values: an array containin 16 elements in column-major (elements 1 to 4 will be used as column 1)
     */
    explicit Mat4(const T* values)
    {
        data[0].set_all(values + 0);
        data[1].set_all(values + 4);
        data[2].set_all(values + 8);
        data[3].set_all(values + 12);
    }

    /**
     * @brief create matrix with column data specified
     * @param col1: data for column 1
     * @param col2: data for column 2
     * @param col3: data for column 3
     * @param col4: data for column 4
     */
    Mat4(const Vec4<T>& col1, const Vec4<T>& col2, const Vec4<T>& col3, const Vec4<T>& col4)
    {
        data[0] = col1.data;
        data[1] = col2.data;
        data[2] = col3.data;
        data[3] = col4.data;
    }

    /**
     * @brief create matrix with column data specified
     * @param col1: data for column 1
     * @param col2: data for column 2
     * @param col3: data for column 3
     * @param col4: data for column 4
     */
    Mat4(const DataType& col1, const DataType& col2, const DataType& col3, const DataType& col4)
    {
        data[0] = col1;
        data[1] = col2;
        data[2] = col3;
        data[3] = col4;
    }

    /**
     * @brief create matrix with translate and rotate specified
     * @param translate
     * @param rotate
     */
    Mat4(const Vec3<T>& translate, const Quat<T>& rotate)
    {
        set_rotate(rotate);
        data[3].set_all(translate.x, translate.y, translate.z, T(1));
    }

    Mat4(const Vec4<T>& translate, const Quat<T>& rotate)
    {
        set_rotate(rotate);
        data[3] = translate.data;
        data[3].template set<3>(T(1));
    }

    /**
     * @brief get value at specified position
     * @return value on specified row and column
     */
    template<int row, int col>
    T get() const noexcept
    {
        return data[col].template get<row>();
    }

    template<int row, int col>
    void set(T value) noexcept
    {
        data[col].template set<row>(value);
    }

    void set(T col0_0, T col0_1, T col0_2, T col0_3,
             T col1_0, T col1_1, T col1_2, T col1_3,
             T col2_0, T col2_1, T col2_2, T col2_3,
             T col3_0, T col3_1, T col3_2, T col3_3) noexcept
    {
        data[0].set_all(col0_0, col0_1, col0_2, col0_3);
        data[1].set_all(col1_0, col1_1, col1_2, col1_3);
        data[2].set_all(col2_0, col2_1, col2_2, col2_3);
        data[3].set_all(col3_0, col3_1, col3_2, col3_3);
    }

    /**
     * @brief switch lines and columns
     */
    void transpose() noexcept
    {
        DataType tmp0(get<0, 0>(), get<0, 1>(), get<0, 2>(), get<0, 3>());
        DataType tmp1(get<1, 0>(), get<1, 1>(), get<1, 2>(), get<1, 3>());
        DataType tmp2(get<2, 0>(), get<2, 1>(), get<2, 2>(), get<2, 3>());
        DataType tmp3(get<3, 0>(), get<3, 1>(), get<3, 2>(), get<3, 3>());

        data[0] = tmp0;
        data[1] = tmp1;
        data[2] = tmp2;
        data[3] = tmp3;
    }

    /**
     * @brief get inverse-transpose matrix
     * @return result matrix
     */
    Mat4<T> get_normal_matrix() const noexcept
    {
        Mat4<T> re(*this);
        re.template set<3, 0>(T(0));
        re.template set<3, 1>(T(0));
        re.template set<3, 2>(T(0));
        re.data[3].set_all(T(0), T(0), T(0), T(1));

        re.inverse();
        re.transpose();
        return re;
    }

    /**
     * @brief calculate matrix determinant
     * @return determinant value
     */
    T determinant() const noexcept;

    /**
     * @brief set the first 3x3 dimension to scale matrix. The 4th translation column is untouched.
     * @param x: scale rate along X axis
     * @param y: scale rate along Y axis
     * @param z: scale rate along Z axis
     */
    void set_scale(T sx, T sy, T sz) noexcept
    {
        data[0].set_all(sx,   T(0), T(0), T(0));
        data[1].set_all(T(0), sy,   T(0), T(0));
        data[2].set_all(T(0), T(0), sz,   T(0));
    }

    /**
     * @brief set 4rd column to translation, keep other columns untouched
     * @param x: offset on X axis
     * @param y: offset on Y axis
     * @param z: offset on Z axis
     */
    void set_translate(T x, T y, T z) noexcept
    {
        data[3].set_all(x, y, z, T(1));
    }

    /**
     * @brief set 4rd column to translation, keep other columns untouched
     * @param value: offsets in one vector. The 4rd component will be set to 1 by force.
     */
    void set_translate(Vec4<T> value) noexcept
    {
        value.set_w(1);
        data[3] = value.data;
    }

    /**
     * @brief
     */
    void set_perspective(float vertical_angle, float ratio, float near, float far) noexcept;

    /**
     * @brief inverse this matrix
     * @return the determinant before inverse
     */
    T inverse() noexcept
    {
        // 00 01 02 03
        // 10 11 12 13
        // 20 21 22 23
        // 30 31 32 33
        T det4 = determinant();
        if (det4 == T(0))
            return det4;

        T v00 = + det3x3<T>(data[1].template get_shuffle<1, 2, 3, 0>(), data[2].template get_shuffle<1, 2, 3, 0>(), data[3].template get_shuffle<1, 2, 3, 0>()) / det4;
        T v10 = - det3x3<T>(data[0].template get_shuffle<1, 2, 3, 0>(), data[2].template get_shuffle<1, 2, 3, 0>(), data[3].template get_shuffle<1, 2, 3, 0>()) / det4;
        T v20 = + det3x3<T>(data[0].template get_shuffle<1, 2, 3, 0>(), data[1].template get_shuffle<1, 2, 3, 0>(), data[3].template get_shuffle<1, 2, 3, 0>()) / det4;
        T v30 = - det3x3<T>(data[0].template get_shuffle<1, 2, 3, 0>(), data[1].template get_shuffle<1, 2, 3, 0>(), data[2].template get_shuffle<1, 2, 3, 0>()) / det4;
        T v01 = - det3x3<T>(data[1].template get_shuffle<0, 2, 3, 0>(), data[2].template get_shuffle<0, 2, 3, 0>(), data[3].template get_shuffle<0, 2, 3, 0>()) / det4;
        T v11 = + det3x3<T>(data[0].template get_shuffle<0, 2, 3, 0>(), data[2].template get_shuffle<0, 2, 3, 0>(), data[3].template get_shuffle<0, 2, 3, 0>()) / det4;
        T v21 = - det3x3<T>(data[0].template get_shuffle<0, 2, 3, 0>(), data[1].template get_shuffle<0, 2, 3, 0>(), data[3].template get_shuffle<0, 2, 3, 0>()) / det4;
        T v31 = + det3x3<T>(data[0].template get_shuffle<0, 2, 3, 0>(), data[1].template get_shuffle<0, 2, 3, 0>(), data[2].template get_shuffle<0, 2, 3, 0>()) / det4;
        T v02 = + det3x3<T>(data[1].template get_shuffle<0, 1, 3, 0>(), data[2].template get_shuffle<0, 1, 3, 0>(), data[3].template get_shuffle<0, 1, 3, 0>()) / det4;
        T v12 = - det3x3<T>(data[0].template get_shuffle<0, 1, 3, 0>(), data[2].template get_shuffle<0, 1, 3, 0>(), data[3].template get_shuffle<0, 1, 3, 0>()) / det4;
        T v22 = + det3x3<T>(data[0].template get_shuffle<0, 1, 3, 0>(), data[1].template get_shuffle<0, 1, 3, 0>(), data[3].template get_shuffle<0, 1, 3, 0>()) / det4;
        T v32 = - det3x3<T>(data[0].template get_shuffle<0, 1, 3, 0>(), data[1].template get_shuffle<0, 1, 3, 0>(), data[2].template get_shuffle<0, 1, 3, 0>()) / det4;
        T v03 = - det3x3<T>(data[1].template get_shuffle<0, 1, 2, 0>(), data[2].template get_shuffle<0, 1, 2, 0>(), data[3].template get_shuffle<0, 1, 2, 0>()) / det4;
        T v13 = + det3x3<T>(data[0].template get_shuffle<0, 1, 2, 0>(), data[2].template get_shuffle<0, 1, 2, 0>(), data[3].template get_shuffle<0, 1, 2, 0>()) / det4;
        T v23 = - det3x3<T>(data[0].template get_shuffle<0, 1, 2, 0>(), data[1].template get_shuffle<0, 1, 2, 0>(), data[3].template get_shuffle<0, 1, 2, 0>()) / det4;
        T v33 = + det3x3<T>(data[0].template get_shuffle<0, 1, 2, 0>(), data[1].template get_shuffle<0, 1, 2, 0>(), data[2].template get_shuffle<0, 1, 2, 0>()) / det4;

        data[0].set_all(v00, v10, v20, v30);
        data[1].set_all(v01, v11, v21, v31);
        data[2].set_all(v02, v12, v22, v32);
        data[3].set_all(v03, v13, v23, v33);

        return det4;
    }

    /**
     * @brief set first 3x3 elements to rotate matrix using specified quaternion. The 4th translation column is untouched.
     * @param value: quaternion representing the rotation
     */
    void set_rotate(const Quat<T>& value) noexcept
    {
        gen_quat_rotate(value.data, data[0], data[1], data[2]);
    }

    DataType data[4];
};

typedef Mat4<float> Mat4f;

///
/// @brief matrix multiply
///
template<typename T>
Mat4<T> operator * (const Mat4<T>& a, const Mat4<T>& b) noexcept;

extern template
Mat4<float> operator * (const Mat4<float>& a, const Mat4<float>& b) noexcept;

///
/// @brief transform a vector
///
template<typename T>
Vec4<T> operator * (const Mat4<T>& mat, const Vec4<T>& vec) noexcept;

extern template
Vec4<float> operator * (const Mat4<float>& mat, const Vec4<float>& vec) noexcept;

} // namespace treeface

#endif // TREEFACE_MAT4_H
