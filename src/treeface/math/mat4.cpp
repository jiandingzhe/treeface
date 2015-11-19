#include "treeface/math/mat4.h"

#include <stdio.h>

using namespace treecore;

namespace treeface {

static_assert( sizeof(Mat4f) == sizeof(float)*16 , "size of Mat4f is not 16*float");

template<>
float Mat4<float>::determinant() const noexcept
{
    // + 00*11*22*33 + 00*21*32*13 + 00*31*12*23 - 00*31*22*13 - 00*21*12*33 - 00*11*32*23
    // + 10*31*22*03 + 10*21*02*33 + 10*01*32*23 - 10*01*22*33 - 10*21*32*03 - 10*31*02*23
    // + 20*01*12*33 + 20*11*32*03 + 20*31*02*13 - 20*31*12*03 - 20*11*02*33 - 20*01*32*13
    // + 30*21*12*03 + 30*11*02*23 + 30*01*22*13 - 30*01*12*23 - 30*11*22*03 - 30*21*02*13

#define _MAT4_FLOAT_DET_OP_ treecore::simd_mul<float>(treecore::simd_mul<float>(treecore::simd_mul<float>(v0, v1), v2), v3)
    treecore::SIMDType<16> re = treecore::simd_set<float, 16>(0.0f);
    treecore::SIMDType<16> v0 = data[0];

    // +
    treecore::SIMDType<16> v1 = treecore::simd_shuffle<1, 3, 0, 2>(data[1]);
    treecore::SIMDType<16> v2 = treecore::simd_shuffle<2, 2, 1, 1>(data[2]);
    treecore::SIMDType<16> v3 = treecore::simd_shuffle<3, 0, 3, 0>(data[3]);
    re = treecore::simd_add<float>(re, _MAT4_FLOAT_DET_OP_);

    v1 = treecore::simd_shuffle<2, 2, 1, 1>(data[1]);
    v2 = treecore::simd_shuffle<3, 0, 3, 0>(data[2]);
    v3 = treecore::simd_shuffle<1, 3, 0, 2>(data[3]);
    re = treecore::simd_add<float>(re, _MAT4_FLOAT_DET_OP_);

    v1 = treecore::simd_shuffle<3, 0, 3, 0>(data[1]);
    v2 = treecore::simd_shuffle<1, 3, 0, 2>(data[2]);
    v3 = treecore::simd_shuffle<2, 2, 1, 1>(data[3]);
    re = treecore::simd_add<float>(re, _MAT4_FLOAT_DET_OP_);

    // -
    v1 = treecore::simd_shuffle<3, 0, 3, 0>(data[1]);
    v2 = treecore::simd_shuffle<2, 2, 1, 1>(data[2]);
    v3 = treecore::simd_shuffle<1, 3, 0, 2>(data[3]);
    re = treecore::simd_sub<float>(re, _MAT4_FLOAT_DET_OP_);

    v1 = treecore::simd_shuffle<2, 2, 1, 1>(data[1]);
    v2 = treecore::simd_shuffle<1, 3, 0, 2>(data[2]);
    v3 = treecore::simd_shuffle<3, 0, 3, 0>(data[3]);
    re = treecore::simd_sub<float>(re, _MAT4_FLOAT_DET_OP_);

    v1 = treecore::simd_shuffle<1, 3, 0, 2>(data[1]);
    v2 = treecore::simd_shuffle<3, 0, 3, 0>(data[2]);
    v3 = treecore::simd_shuffle<2, 2, 1, 1>(data[3]);
    re = treecore::simd_sub<float>(re, _MAT4_FLOAT_DET_OP_);

    return treecore::simd_sum<float>(re);
}

template<>
void Mat4<float>::set_perspective(float vertical_angle, float ratio, float near, float far) noexcept
{
    if (ratio == 0.0f || near == far)
        return;

    float sine = std::sin(vertical_angle);
    if (sine == 0.0f)
        return;

    float cotan = std::cos(vertical_angle) / sine;
    float depth = far - near;

    float xx = cotan / ratio;
    float yy = cotan;
    float zz = -(near + far) / depth;
    float zw = -2.0f * near * far / depth;

    set(xx, 0, 0,  0,
        0, yy, 0,  0,
        0, 0, zz, -1,
        0, 0, zw,  1);
}

template<>
void Mat4<float>::set_rotate(const Quat<float>& value) noexcept
{
#define _MAT4_FLOAT_ROTATE_OP_ treecore::simd_add<float>(tmp_unit, treecore::simd_mul<float>(tmp_mul, treecore::simd_add<float>(treecore::simd_mul<float>(tmp_a, tmp_b),treecore::simd_xor<float>(treecore::simd_mul<float>(tmp_c, tmp_d), tmp_sign_mask))))
//        m[0]  = 1 - 2 * (y*y + z*z);
//        m[1]  = 0 + 2 * (x*y + z*w);
//        m[2]  = 0 + 2 * (z*x - y*w);
//        m[3]  = 0;
    treecore::SIMDType<16> tmp_unit = treecore::simd_set<float, 16>(1, 0, 0, 0);
    treecore::SIMDType<16> tmp_mul  = treecore::simd_set<float, 16>(-2, 2, 2, 0);
    treecore::SIMDType<16> tmp_a    = treecore::simd_shuffle<1, 0, 2, 0>(value.data);
    treecore::SIMDType<16> tmp_b    = treecore::simd_shuffle<1, 1, 0, 0>(value.data);
    treecore::SIMDType<16> tmp_sign_mask = treecore::simd_set<std::int32_t, 16>(0, 0, 0x80000000, 0);
    treecore::SIMDType<16> tmp_c    = treecore::simd_shuffle<2, 2, 1, 0>(value.data);
    treecore::SIMDType<16> tmp_d    = treecore::simd_shuffle<2, 3, 3, 0>(value.data);
    data[0] = _MAT4_FLOAT_ROTATE_OP_;

//        m[4]  = 0 + 2 * (x*y - z*w);
//        m[5]  = 1 - 2 * (z*z + x*x);
//        m[6]  = 0 + 2 * (y*z + x*w);
//        m[7]  = 0;
    tmp_unit = treecore::simd_set<float, 16>(0, 1, 0, 0);
    tmp_mul  = treecore::simd_set<float, 16>(2, -2, 2, 0);
    tmp_a    = treecore::simd_shuffle<0, 2, 1, 0>(value.data);
    tmp_b    = treecore::simd_shuffle<1, 2, 2, 0>(value.data);
    tmp_sign_mask = treecore::simd_set<std::int32_t, 16>(0x80000000, 0, 0, 0);
    tmp_c    = treecore::simd_shuffle<2, 0, 0, 0>(value.data);
    tmp_d    = treecore::simd_shuffle<3, 0, 3, 0>(value.data);
    data[1] = _MAT4_FLOAT_ROTATE_OP_;

//        m[8]  = 0 + 2 * (z*x + y*w);
//        m[9]  = 0 + 2 * (y*z - x*w);
//        m[10] = 1 - 2 * (x*x + y*y);
//        m[11] = 0;
    tmp_unit = treecore::simd_set<float, 16>(0, 0, 1, 0);
    tmp_mul  = treecore::simd_set<float, 16>(2, 2, -2, 0);
    tmp_a    = treecore::simd_shuffle<2, 1, 0, 0>(value.data);
    tmp_b    = treecore::simd_shuffle<0, 2, 0, 0>(value.data);
    tmp_sign_mask = treecore::simd_set<std::int32_t, 16>(0, 0x80000000, 0, 0);
    tmp_c    = treecore::simd_shuffle<1, 0, 1, 0>(value.data);
    tmp_d    = treecore::simd_shuffle<3, 3, 1, 0>(value.data);
    data[2] = _MAT4_FLOAT_ROTATE_OP_;

    // translation is kept untouched
}

Mat4<float> operator * (const Mat4<float>& a, const Mat4<float>& b) noexcept
{
    // column to row
    treecore::SIMDType<16> row0 = treecore::simd_set<float, 16>(
        treecore::simd_get_one<0, float>(a.data[0]),
        treecore::simd_get_one<0, float>(a.data[1]),
        treecore::simd_get_one<0, float>(a.data[2]),
        treecore::simd_get_one<0, float>(a.data[3]));

    treecore::SIMDType<16> row1 = treecore::simd_set<float, 16>(
        treecore::simd_get_one<1, float>(a.data[0]),
        treecore::simd_get_one<1, float>(a.data[1]),
        treecore::simd_get_one<1, float>(a.data[2]),
        treecore::simd_get_one<1, float>(a.data[3]));

    treecore::SIMDType<16> row2 = treecore::simd_set<float, 16>(
        treecore::simd_get_one<2, float>(a.data[0]),
        treecore::simd_get_one<2, float>(a.data[1]),
        treecore::simd_get_one<2, float>(a.data[2]),
        treecore::simd_get_one<2, float>(a.data[3]));

    treecore::SIMDType<16> row3 = treecore::simd_set<float, 16>(
        treecore::simd_get_one<3, float>(a.data[0]),
        treecore::simd_get_one<3, float>(a.data[1]),
        treecore::simd_get_one<3, float>(a.data[2]),
        treecore::simd_get_one<3, float>(a.data[3]));

    // multiply rows in A with columns in B
    treecore::SIMDType<16> re0 = treecore::simd_set<float, 16>(
        treecore::simd_sum<float>(treecore::simd_mul<float>(row0, b.data[0])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row1, b.data[0])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row2, b.data[0])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row3, b.data[0])));

    treecore::SIMDType<16> re1 = treecore::simd_set<float, 16>(
        treecore::simd_sum<float>(treecore::simd_mul<float>(row0, b.data[1])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row1, b.data[1])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row2, b.data[1])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row3, b.data[1])));

    treecore::SIMDType<16> re2 = treecore::simd_set<float, 16>(
        treecore::simd_sum<float>(treecore::simd_mul<float>(row0, b.data[2])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row1, b.data[2])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row2, b.data[2])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row3, b.data[2])));

    treecore::SIMDType<16> re3 = treecore::simd_set<float, 16>(
        treecore::simd_sum<float>(treecore::simd_mul<float>(row0, b.data[3])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row1, b.data[3])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row2, b.data[3])),
        treecore::simd_sum<float>(treecore::simd_mul<float>(row3, b.data[3])));

    return Mat4<float>(re0, re1, re2, re3);
}

Vec4<float> operator * (const Mat4<float>& mat, const Vec4<float>& vec) noexcept
{
    treecore::SIMDType<16> tmp;
    tmp = treecore::simd_set<float, 16>(
        treecore::simd_get_one<0, float>(mat.data[0]),
        treecore::simd_get_one<0, float>(mat.data[1]),
        treecore::simd_get_one<0, float>(mat.data[2]),
        treecore::simd_get_one<0, float>(mat.data[3]));

    float re0 = treecore::simd_sum<float>(treecore::simd_mul<float>(tmp, vec.data));

    tmp = treecore::simd_set<float, 16>(
        treecore::simd_get_one<1, float>(mat.data[0]),
        treecore::simd_get_one<1, float>(mat.data[1]),
        treecore::simd_get_one<1, float>(mat.data[2]),
        treecore::simd_get_one<1, float>(mat.data[3]));

    float re1 = treecore::simd_sum<float>(treecore::simd_mul<float>(tmp, vec.data));

    tmp = treecore::simd_set<float, 16>(
        treecore::simd_get_one<2, float>(mat.data[0]),
        treecore::simd_get_one<2, float>(mat.data[1]),
        treecore::simd_get_one<2, float>(mat.data[2]),
        treecore::simd_get_one<2, float>(mat.data[3]));

    float re2 = treecore::simd_sum<float>(treecore::simd_mul<float>(tmp, vec.data));

    tmp = treecore::simd_set<float, 16>(
        treecore::simd_get_one<3, float>(mat.data[0]),
        treecore::simd_get_one<3, float>(mat.data[1]),
        treecore::simd_get_one<3, float>(mat.data[2]),
        treecore::simd_get_one<3, float>(mat.data[3]));

    float re3 = treecore::simd_sum<float>(treecore::simd_mul<float>(tmp, vec.data));

    return Vec4<float>(re0, re1, re2, re3);
}

} // namespace treeface
