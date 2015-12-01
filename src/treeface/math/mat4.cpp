#include "treeface/math/mat4.h"

#include <stdio.h>

using namespace treecore;

namespace treeface {

static_assert( sizeof(Mat4f) == sizeof(float)*16 , "");
static_assert( sizeof(Mat4f::DataType) == sizeof(float)*4, "");
static_assert( sizeof(Mat4f::DataType::DataType) == sizeof(float)*4, "");

template<>
float Mat4<float>::determinant() const noexcept
{
    // + 00*11*22*33 + 00*21*32*13 + 00*31*12*23 - 00*31*22*13 - 00*21*12*33 - 00*11*32*23
    // + 10*31*22*03 + 10*21*02*33 + 10*01*32*23 - 10*01*22*33 - 10*21*32*03 - 10*31*02*23
    // + 20*01*12*33 + 20*11*32*03 + 20*31*02*13 - 20*31*12*03 - 20*11*02*33 - 20*01*32*13
    // + 30*21*12*03 + 30*11*02*23 + 30*01*22*13 - 30*01*12*23 - 30*11*22*03 - 30*21*02*13

    DataType re(0.0f);
    DataType v0 = data[0];

    // +
    DataType v1 = data[1].template get_shuffle<1, 3, 0, 2>();
    DataType v2 = data[2].template get_shuffle<2, 2, 1, 1>();
    DataType v3 = data[3].template get_shuffle<3, 0, 3, 0>();
    re += v0 * v1 * v2 * v3;

    v1 = data[1].template get_shuffle<2, 2, 1, 1>();
    v2 = data[2].template get_shuffle<3, 0, 3, 0>();
    v3 = data[3].template get_shuffle<1, 3, 0, 2>();
    re += v0 * v1 * v2 * v3;

    v1 = data[1].template get_shuffle<3, 0, 3, 0>();
    v2 = data[2].template get_shuffle<1, 3, 0, 2>();
    v3 = data[3].template get_shuffle<2, 2, 1, 1>();
    re += v0 * v1 * v2 * v3;

    // -
    v1 = data[1].template get_shuffle<3, 0, 3, 0>();
    v2 = data[2].template get_shuffle<2, 2, 1, 1>();
    v3 = data[3].template get_shuffle<1, 3, 0, 2>();
    re -= v0 * v1 * v2 * v3;

    v1 = data[1].template get_shuffle<2, 2, 1, 1>();
    v2 = data[2].template get_shuffle<1, 3, 0, 2>();
    v3 = data[3].template get_shuffle<3, 0, 3, 0>();
    re -= v0 * v1 * v2 * v3;

    v1 = data[1].template get_shuffle<1, 3, 0, 2>();
    v2 = data[2].template get_shuffle<3, 0, 3, 0>();
    v3 = data[3].template get_shuffle<2, 2, 1, 1>();
    re -= v0 * v1 * v2 * v3;

    return re.sum();
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
#define _MAT4_FLOAT_ROTATE_OP_ tmp_unit +  tmp_mul * (tmp_a * tmp_b + (tmp_c * tmp_d) | tmp_sign_mask )
    //        m[0]  = 1 - 2 * (y*y + z*z);
    //        m[1]  = 0 + 2 * (x*y + z*w);
    //        m[2]  = 0 + 2 * (z*x - y*w);
    //        m[3]  = 0;
    Mat4f::DataType tmp_unit(1.0f, 0.0f, 0.0f, 0.0f);
    Mat4f::DataType tmp_mul(-2.0f, 2.0f, 2.0f, 0.0f);
    SimdObject<treecore::int32, 4> tmp_sign_mask(0, 0, 0x80000000, 0);
    Mat4f::DataType tmp_a = value.data.template get_shuffle<1, 0, 2, 0>();
    Mat4f::DataType tmp_b = value.data.template get_shuffle<1, 1, 0, 0>();
    Mat4f::DataType tmp_c = value.data.template get_shuffle<2, 2, 1, 0>();
    Mat4f::DataType tmp_d = value.data.template get_shuffle<2, 3, 3, 0>();
    data[0] = _MAT4_FLOAT_ROTATE_OP_;

    //        m[4]  = 0 + 2 * (x*y - z*w);
    //        m[5]  = 1 - 2 * (z*z + x*x);
    //        m[6]  = 0 + 2 * (y*z + x*w);
    //        m[7]  = 0;
    tmp_unit.set_all(0.0f, 1.0f, 0.0f, 0.0f);
    tmp_mul .set_all(2.0f, -2.0f, 2.0f, 0.0f);
    tmp_sign_mask.set_all(0x80000000, 0, 0, 0);
    tmp_a = value.data.template get_shuffle<0, 2, 1, 0>();
    tmp_b = value.data.template get_shuffle<1, 2, 2, 0>();
    tmp_c = value.data.template get_shuffle<2, 0, 0, 0>();
    tmp_d = value.data.template get_shuffle<3, 0, 3, 0>();
    data[1] = _MAT4_FLOAT_ROTATE_OP_;

    //        m[8]  = 0 + 2 * (z*x + y*w);
    //        m[9]  = 0 + 2 * (y*z - x*w);
    //        m[10] = 1 - 2 * (x*x + y*y);
    //        m[11] = 0;
    tmp_unit.set_all(0.0f, 0.0f, 1.0f, 0.0f);
    tmp_mul .set_all(2.0f, 2.0f, -2.0f, 0.0f);
    tmp_sign_mask.set_all(0, 0x80000000, 0, 0);
    tmp_a = value.data.template get_shuffle<2, 1, 0, 0>();
    tmp_b = value.data.template get_shuffle<0, 2, 0, 0>();
    tmp_c = value.data.template get_shuffle<1, 0, 1, 0>();
    tmp_d = value.data.template get_shuffle<3, 3, 1, 0>();
    data[2] = _MAT4_FLOAT_ROTATE_OP_;

    // translation is kept untouched
}

Mat4<float> operator * (const Mat4<float>& a, const Mat4<float>& b) noexcept
{
    // column to row
    Mat4f a_t = a;
    a_t.transpose();

    // multiply rows in A with columns in B
    Mat4f::DataType re0((a_t.data[0] * b.data[0]).sum(),
                        (a_t.data[1] * b.data[0]).sum(),
                        (a_t.data[2] * b.data[0]).sum(),
                        (a_t.data[3] * b.data[0]).sum());

    Mat4f::DataType re1((a_t.data[0] * b.data[1]).sum(),
                        (a_t.data[1] * b.data[1]).sum(),
                        (a_t.data[2] * b.data[1]).sum(),
                        (a_t.data[3] * b.data[1]).sum());

    Mat4f::DataType re2((a_t.data[0] * b.data[2]).sum(),
                        (a_t.data[1] * b.data[2]).sum(),
                        (a_t.data[2] * b.data[2]).sum(),
                        (a_t.data[3] * b.data[2]).sum());

    Mat4f::DataType re3((a_t.data[0] * b.data[3]).sum(),
                        (a_t.data[1] * b.data[3]).sum(),
                        (a_t.data[2] * b.data[3]).sum(),
                        (a_t.data[3] * b.data[3]).sum());

    return Mat4<float>(re0, re1, re2, re3);
}

Vec4<float> operator * (const Mat4<float>& mat, const Vec4<float>& vec) noexcept
{
    // column to row
    Mat4f mat_t = mat;
    mat_t.transpose();

    return Vec4<float>((mat_t.data[0] * vec.data).sum(),
                       (mat_t.data[1] * vec.data).sum(),
                       (mat_t.data[2] * vec.data).sum(),
                       (mat_t.data[3] * vec.data).sum());
}

} // namespace treeface
