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

template<typename T>
Mat4<T> operator * (const Mat4<T>& a, const Mat4<T>& b) noexcept
{
    // column to row
    Mat4<T> a_t = a;
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

    return Mat4<T>(re0, re1, re2, re3);
}

template
Mat4<float> operator * (const Mat4<float>& a, const Mat4<float>& b) noexcept;

template<typename T>
Vec4<T> operator * (const Mat4<T>& mat, const Vec4<T>& vec) noexcept
{
    // column to row
    Mat4<T> mat_t = mat;
    mat_t.transpose();

    return Vec4<T>((mat_t.data[0] * vec.data).sum(),
                   (mat_t.data[1] * vec.data).sum(),
                   (mat_t.data[2] * vec.data).sum(),
                   (mat_t.data[3] * vec.data).sum());
}

template
Vec4<float> operator * (const Mat4<float>& mat, const Vec4<float>& vec) noexcept;

} // namespace treeface
