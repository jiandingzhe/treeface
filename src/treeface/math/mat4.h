#ifndef TREEFACE_MAT4_H
#define TREEFACE_MAT4_H

#include "treeface/simd.h"
#include "treeface/math/vec4.h"
#include "treeface/math/quat.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T) * 4>
T det3x3(SIMDType<SZ> v0, SIMDType<SZ> v1, SIMDType<SZ> v2);

template<>
float det3x3<float, 16>(SIMDType<16> v0, SIMDType<16> v1, SIMDType<16> v2)
{
//        + v00 * v11 * v22
//        + v02 * v10 * v21
//        + v01 * v12 * v20
//        - v02 * v11 * v20
//        - v01 * v10 * v22
//        - v00 * v12 * v21
    SIMDType<16> tmp1 = simd_shuffle<0, 2, 1, 0>(v0);
    SIMDType<16> tmp2 = simd_shuffle<1, 0, 2, 0>(v1);
    SIMDType<16> tmp3 = simd_shuffle<2, 1, 0, 0>(v2);
    SIMDType<16> re = simd_mul<float>(tmp1, simd_mul<float>(tmp2, tmp3));

    tmp1 = simd_shuffle<2, 1, 0, 0>(v0);
    tmp2 = simd_shuffle<1, 0, 2, 0>(v1);
    tmp3 = simd_shuffle<0, 2, 1, 0>(v2);

    re = simd_sub<float>(re, simd_mul<float>(tmp1, simd_mul<float>(tmp2, tmp3)));
    re = simd_and<std::int32_t>(re, simd_set<std::int32_t, 16>(0xffffffff, 0xffffffff, 0xffffffff, 0));
    return simd_sum<float>(re);
}

template<typename T, int SZ = sizeof(T) * 4>
struct Mat4
{
    typedef SIMDType<SZ> DataType;

    /**
     * @brief create identity matrix
     */
    Mat4()
    {
        data[0] = simd_set<T, SZ>(1, 0, 0, 0);
        data[1] = simd_set<T, SZ>(0, 1, 0, 0);
        data[2] = simd_set<T, SZ>(0, 0, 1, 0);
        data[3] = simd_set<T, SZ>(0, 0, 0, 1);
    }

    /**
     * @brief create matrix from existing one
     */
    Mat4(const Mat4& other) = default;

    /**
     * @brief create matrix from array of data
     * @param values: an array containin 16 elements in column-major (elements 1 to 4 will be used as column 1)
     */
    explicit Mat4(const T* values)
    {
        data[0] = simd_set<T, SZ>(values);
        data[1] = simd_set<T, SZ>(values+4);
        data[2] = simd_set<T, SZ>(values+8);
        data[3] = simd_set<T, SZ>(values+12);
    }

    /**
     * @brief create matrix with column data specified
     * @param col1: data for column 1
     * @param col2: data for column 2
     * @param col3: data for column 3
     * @param col4: data for column 4
     */
    Mat4(Vec4<T> col1, Vec4<T> col2, Vec4<T> col3, Vec4<T> col4)
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
    Mat4(DataType col1, DataType col2, DataType col3, DataType col4)
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
    Mat4(Vec4<T> translate, Quat<T> rotate)
    {
        set_rotate(rotate);
        data[3] = translate.data;
        data[3] = simd_set_one<3, T>(data[3], 1);
    }

    /**
     * @brief get value at specified position
     * @return value on specified row and column
     */
    template<int row, int col>
    T get()
    {
        return simd_get_one<row, T>(data[col]);
    }

    /**
     * @brief store invert lines and columns in a new matrix
     * @return transposed matrix
     */
    Mat4 transpose() const noexcept
    {
        SIMDType<SZ> tmp0 = simd_set<T, SZ>(
                simd_get_one<0, T>(data[0]),
                simd_get_one<0, T>(data[1]),
                simd_get_one<0, T>(data[2]),
                simd_get_one<0, T>(data[3]));

        SIMDType<SZ> tmp1 = simd_set<T, SZ>(
                simd_get_one<1, T>(data[0]),
                simd_get_one<1, T>(data[1]),
                simd_get_one<1, T>(data[2]),
                simd_get_one<1, T>(data[3]));

        SIMDType<SZ> tmp2 = simd_set<T, SZ>(
                simd_get_one<2, T>(data[0]),
                simd_get_one<2, T>(data[1]),
                simd_get_one<2, T>(data[2]),
                simd_get_one<2, T>(data[3]));

        SIMDType<SZ> tmp3 = simd_set<T, SZ>(
                simd_get_one<3, T>(data[0]),
                simd_get_one<3, T>(data[1]),
                simd_get_one<3, T>(data[2]),
                simd_get_one<3, T>(data[3]));

        return Mat4(tmp0, tmp1, tmp2, tmp3);
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
    void set_scale(T x, T y, T z) noexcept
    {
        data[0] = simd_set<T, SZ>(x, 0, 0, 0);
        data[1] = simd_set<T, SZ>(0, y, 0, 0);
        data[2] = simd_set<T, SZ>(0, 0, z, 0);
    }

    /**
     * @brief set 4rd column to translation, keep other columns untouched
     * @param x: offset on X axis
     * @param y: offset on Y axis
     * @param z: offset on Z axis
     */
    void set_translate(T x, T y, T z) noexcept
    {
        data[3] = simd_set<T, SZ>(x, y, z, 1);
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

    void inverse() noexcept
    {

    }

    /**
     * @brief set first 3x3 dimention to rotate matrix using specified quaternion. The 4th translation column is untouched.
     * @param value: quaternion representing the rotation
     */
    void set_rotate(Quat<T> value) noexcept;

    DataType data[4];
};

typedef Mat4<float> Mat4f;

template<>
float Mat4<float>::determinant() const noexcept;

template<>
void Mat4<float>::set_rotate(Quat<float> value) noexcept;

Mat4<float> operator * (const Mat4<float>& a, const Mat4<float>& b) noexcept;

Vec4<float> operator * (const Mat4<float>& mat, Vec4<float> vec) noexcept;

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MAT4_H
