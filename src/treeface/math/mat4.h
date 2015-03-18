#ifndef TREEFACE_MAT4_H
#define TREEFACE_MAT4_H

#include "treeface/simd.h"
#include "treeface/math/vec4.h"
#include "treeface/math/quat.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T) * 4>
struct Mat4
{
    typedef SIMDType<SZ> DataType;
    Mat4()
    {
        data[0] = simd_set<T, SZ>(1, 0, 0, 0);
        data[1] = simd_set<T, SZ>(0, 1, 0, 0);
        data[2] = simd_set<T, SZ>(0, 0, 1, 0);
        data[3] = simd_set<T, SZ>(0, 0, 0, 1);
    }

    Mat4(const Mat4& other) = default;

    Mat4(const T* values)
    {
        data[0] = simd_set<T, SZ>(values);
        data[1] = simd_set<T, SZ>(values+4);
        data[2] = simd_set<T, SZ>(values+8);
        data[3] = simd_set<T, SZ>(values+12);
    }

    Mat4(Vec4<T> axis1, Vec4<T> axis2, Vec4<T> axis3, Vec4<T> trans)
    {
        data[0] = axis1.data;
        data[1] = axis2.data;
        data[2] = axis3.data;
        data[3] = trans.data;
    }

    Mat4(DataType axis1, DataType axis2, DataType axis3, DataType trans)
    {
        data[0] = axis1;
        data[1] = axis2;
        data[2] = axis3;
        data[3] = trans;
    }

    Mat4 transpose()
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

    void set_scale(T x, T y, T z)
    {
        data[0] = simd_set<T, SZ>(x, 0, 0, 0);
        data[1] = simd_set<T, SZ>(0, y, 0, 0);
        data[2] = simd_set<T, SZ>(0, 0, z, 0);
    }

    void set_translate(T x, T y, T z)
    {
        data[3] = simd_set<T, SZ>(x, y, z, 1);
    }

    void set_translate(Vec4<T> value)
    {
        data[3] = value.data;
    }

    void set_rotate(Quat<T> value)
    {
//        m[0]  = T(1) - T(2) * (y*y + z*z);
//        m[1]  =        T(2) * (x*y + z*w);
//        m[2]  =        T(2) * (z*x - y*w);
//        m[3]  =        T(0);

//        m[4]  =        T(2) * (x*y - z*w);
//        m[5]  = T(1) - T(2) * (z*z + x*x);
//        m[6]  =        T(2) * (y*z + x*w);
//        m[7]  = T(0);

//        m[8]  =        T(2) * (z*x + y*w);
//        m[9]  =        T(2) * (y*z - x*w);
//        m[10] = T(1) - T(2) * (x*x + y*y);
//        m[11] = T(0);

//        m[12] = T(0);
//        m[13] = T(0);
//        m[14] = T(0);
//        m[15] = T(1);
    }

    DataType data[4];
};

template<typename T, int SZ = sizeof(T) * 4>
Mat4<T> operator * (const Mat4<T>& a, const Mat4<T>& b)
{
    // column to row
    SIMDType<SZ> row0 = simd_set<T, SZ>(
        simd_get_one<0, T>(a.data[0]),
        simd_get_one<0, T>(a.data[1]),
        simd_get_one<0, T>(a.data[2]),
        simd_get_one<0, T>(a.data[3]));

    SIMDType<SZ> row1 = simd_set<T, SZ>(
        simd_get_one<1, T>(a.data[0]),
        simd_get_one<1, T>(a.data[1]),
        simd_get_one<1, T>(a.data[2]),
        simd_get_one<1, T>(a.data[3]));

    SIMDType<SZ> row2 = simd_set<T, SZ>(
        simd_get_one<2, T>(a.data[0]),
        simd_get_one<2, T>(a.data[1]),
        simd_get_one<2, T>(a.data[2]),
        simd_get_one<2, T>(a.data[3]));

    SIMDType<SZ> row3 = simd_set<T, SZ>(
        simd_get_one<3, T>(a.data[0]),
        simd_get_one<3, T>(a.data[1]),
        simd_get_one<3, T>(a.data[2]),
        simd_get_one<3, T>(a.data[3]));

    // multiply rows in A with columns in B
    SIMDType<SZ> re0 = simd_set<T, SZ>(
        simd_sum<T>(simd_mul<T>(row0, b.data[0])),
        simd_sum<T>(simd_mul<T>(row1, b.data[0])),
        simd_sum<T>(simd_mul<T>(row2, b.data[0])),
        simd_sum<T>(simd_mul<T>(row3, b.data[0])));

    SIMDType<SZ> re1 = simd_set<T, SZ>(
        simd_sum<T>(simd_mul<T>(row0, b.data[1])),
        simd_sum<T>(simd_mul<T>(row1, b.data[1])),
        simd_sum<T>(simd_mul<T>(row2, b.data[1])),
        simd_sum<T>(simd_mul<T>(row3, b.data[1])));

    SIMDType<SZ> re2 = simd_set<T, SZ>(
        simd_sum<T>(simd_mul<T>(row0, b.data[2])),
        simd_sum<T>(simd_mul<T>(row1, b.data[2])),
        simd_sum<T>(simd_mul<T>(row2, b.data[2])),
        simd_sum<T>(simd_mul<T>(row3, b.data[2])));

    SIMDType<SZ> re3 = simd_set<T, SZ>(
        simd_sum<T>(simd_mul<T>(row0, b.data[3])),
        simd_sum<T>(simd_mul<T>(row1, b.data[3])),
        simd_sum<T>(simd_mul<T>(row2, b.data[3])),
        simd_sum<T>(simd_mul<T>(row3, b.data[3])));

    return Mat4<T>(re0, re1, re2, re3);
}

template<typename T, int SZ = sizeof(T) * 4>
Vec4<T> operator * (const Mat4<T>& mat, Vec4<T> vec)
{
    SIMDType<SZ> tmp;
    tmp = simd_set<T, SZ>(
        simd_get_one<0, T>(mat.data[0]),
        simd_get_one<0, T>(mat.data[1]),
        simd_get_one<0, T>(mat.data[2]),
        simd_get_one<0, T>(mat.data[3]));

    T re0 = simd_sum<T>(simd_mul<T>(tmp, vec.data));

    tmp = simd_set<T, SZ>(
        simd_get_one<1, T>(mat.data[0]),
        simd_get_one<1, T>(mat.data[1]),
        simd_get_one<1, T>(mat.data[2]),
        simd_get_one<1, T>(mat.data[3]));

    T re1 = simd_sum<T>(simd_mul<T>(tmp, vec.data));

    tmp = simd_set<T, SZ>(
        simd_get_one<2, T>(mat.data[0]),
        simd_get_one<2, T>(mat.data[1]),
        simd_get_one<2, T>(mat.data[2]),
        simd_get_one<2, T>(mat.data[3]));

    T re2 = simd_sum<T>(simd_mul<T>(tmp, vec.data));

    tmp = simd_set<T, SZ>(
        simd_get_one<3, T>(mat.data[0]),
        simd_get_one<3, T>(mat.data[1]),
        simd_get_one<3, T>(mat.data[2]),
        simd_get_one<3, T>(mat.data[3]));

    T re3 = simd_sum<T>(simd_mul<T>(tmp, vec.data));

    return Vec4<T>(re0, re1, re2, re3);
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MAT4_H
