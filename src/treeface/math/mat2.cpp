#include "treeface/math/mat2.h"

#include <treecore/IntUtils.h>
#include <treecore/FloatUtils.h>

namespace treeface
{

static_assert(sizeof(Mat2f) == sizeof(float)*4, "the size of 2x2 float matrix is 4 floats");

template<typename T>
T Mat2<T>::inverse() noexcept
{
    T det = determinant();
    if (det == T(0))
        return det;

    typedef typename treecore::similar_int<T>::type IntType;
    typedef treecore::SimdObject<IntType, 4> IDataType;

    data.template shuffle<3, 1, 2, 0>();
    data ^= IDataType(0, treecore::float_sign_mask<T>::value, treecore::float_sign_mask<T>::value, 0);
    data /= DataType(det, det, det, det);
    return det;
}

template
float Mat2f::inverse() noexcept;

template<typename T>
Mat2<T> operator * (const Mat2<T>& a, const Mat2<T>& b) noexcept
{
    typename Mat2<T>::DataType tmp_a1 = a.data.template get_shuffle<0, 1, 0, 1>();
    typename Mat2<T>::DataType tmp_a2 = a.data.template get_shuffle<2, 3, 2, 3>();
    typename Mat2<T>::DataType tmp_b1 = b.data.template get_shuffle<0, 0, 2, 2>();
    typename Mat2<T>::DataType tmp_b2 = b.data.template get_shuffle<1, 1, 3, 3>();

    Mat2<T> result(tmp_a1 * tmp_b1 + tmp_a2 * tmp_b2);
    return result;
}

template
Mat2f operator * (const Mat2f& a, const Mat2f& b) noexcept;

template<typename T>
Vec2<T> operator * (const Mat2<T>& mat, const Vec2<T>& vec) noexcept
{
    typename Mat2<T>::DataType tmp = mat.data.template get_shuffle<0, 2, 1, 3>();
    tmp *= typename Mat2<T>::DataType(vec.x, vec.y, vec.x, vec.y);
    return Vec2<T>(tmp.template get<0>() + tmp.template get<1>(),
                   tmp.template get<2>() + tmp.template get<3>());
}

template
Vec2f operator * (const Mat2f& mat, const Vec2f& vec) noexcept;

}
