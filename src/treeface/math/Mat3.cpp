#include "treeface/math/Mat3.h"

using namespace treecore;

namespace treeface
{

template<typename T>
T Mat3<T>::inverse() noexcept
{
    typedef SimdObject<T, 4> TmpT;
    T det = determinant();
    if (det == T(0))
        return det;

    TmpT tmp522(data[5], data[2], data[2], T(0));
    TmpT tmp411(data[4], data[1], data[1], T(0));
    TmpT tmp300(data[3], data[0], data[0], T(0));
    TmpT tmp885(data[8], data[8], data[5], T(0));
    TmpT tmp774(data[7], data[7], data[4], T(0));
    TmpT tmp663(data[6], data[6], data[3], T(0));

    // tmp[0] =   (data[4] * data[8] - data[5] * data[7]) / det;
    // tmp[1] = - (data[1] * data[8] - data[2] * data[7]) / det;
    // tmp[2] =   (data[1] * data[5] - data[2] * data[4]) / det;
    TmpT result_col1 = (tmp411 * tmp885 - tmp522 * tmp774) / TmpT(det, -det, det, T(0));

    // tmp[3] = - (data[3] * data[8] - data[5] * data[6]) / det;
    // tmp[4] =   (data[0] * data[8] - data[2] * data[6]) / det;
    // tmp[5] = - (data[0] * data[5] - data[2] * data[3]) / det;
    TmpT result_col2 = (tmp300 * tmp885 - tmp522 * tmp663) / TmpT(-det, det, -det, T(0));

    // tmp[6] =   (data[3] * data[7] - data[4] * data[6]) / det;
    // tmp[7] = - (data[0] * data[7] - data[1] * data[6]) / det;
    // tmp[8] =   (data[0] * data[4] - data[1] * data[3]) / det;
    TmpT result_col3 = (tmp300 * tmp774 - tmp411 * tmp663) / TmpT(det, -det, det, T(0));

    data[0] = result_col1.template get<0>();
    data[1] = result_col1.template get<1>();
    data[2] = result_col1.template get<2>();

    data[3] = result_col2.template get<0>();
    data[4] = result_col2.template get<1>();
    data[5] = result_col2.template get<2>();

    data[6] = result_col3.template get<0>();
    data[7] = result_col3.template get<1>();
    data[8] = result_col3.template get<2>();

    return det;
}

template
float Mat3<float>::inverse() noexcept;

template<typename T>
Mat3<T> operator * (const Mat3<T>& a, const Mat3<T>& b) noexcept
{
    typedef SimdObject<T, 4> TmpT;

    TmpT a_row0(a.data[0], a.data[3], a.data[6], T(0));
    TmpT a_row1(a.data[1], a.data[4], a.data[7], T(0));
    TmpT a_row2(a.data[2], a.data[5], a.data[8], T(0));

    TmpT b_col0(b.data[0], b.data[1], b.data[2], T(0));
    TmpT b_col1(b.data[3], b.data[4], b.data[5], T(0));
    TmpT b_col2(b.data[6], b.data[7], b.data[8], T(0));

    Mat3<T> result;

    return Mat3<T>((a_row0 * b_col0).sum(),
                   (a_row1 * b_col0).sum(),
                   (a_row2 * b_col0).sum(),
                   (a_row0 * b_col1).sum(),
                   (a_row1 * b_col1).sum(),
                   (a_row2 * b_col1).sum(),
                   (a_row0 * b_col2).sum(),
                   (a_row1 * b_col2).sum(),
                   (a_row2 * b_col2).sum());
}

template
Mat3f operator * (const Mat3f& a, const Mat3f& b) noexcept;


template<typename T>
Vec3<T> operator * (const Mat3<T>& mat, const Vec3<T>& vec) noexcept
{
    typedef SimdObject<T, 4> TmpT;

    TmpT vec_data(vec.x, vec.y, vec.z, T(0));

    return Vec3<T>((TmpT(mat.data[0], mat.data[3], mat.data[6], T(0)) * vec_data).sum(),
                   (TmpT(mat.data[1], mat.data[4], mat.data[7], T(0)) * vec_data).sum(),
                   (TmpT(mat.data[2], mat.data[5], mat.data[8], T(0)) * vec_data).sum());
}

template
Vec3f operator * (const Mat3f& mat, const Vec3f& vec) noexcept;


} // namespace treeface
