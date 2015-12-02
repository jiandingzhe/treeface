#include "treeface/math/matutils.h"

#include "treeface/math/quat.h"

#include <treecore/FloatUtils.h>
#include <treecore/IntUtils.h>

namespace treeface
{

template<typename T>
T det3x3(const treecore::SimdObject<T, 4>& v0,
         const treecore::SimdObject<T, 4>& v1,
         const treecore::SimdObject<T, 4>& v2) noexcept
{
    typedef typename treecore::similar_int<T>::type IntT;
    //        + v00 * v11 * v22 - v02 * v11 * v20
    //        + v02 * v10 * v21 - v01 * v10 * v22
    //        + v01 * v12 * v20 - v00 * v12 * v21
    treecore::SimdObject<T, 4> tmp1(v0);
    treecore::SimdObject<T, 4> tmp2(v1);
    treecore::SimdObject<T, 4> tmp3(v2);
    tmp1.template shuffle<0, 2, 1, 0>();
    tmp2.template shuffle<1, 0, 2, 0>();
    tmp3.template shuffle<2, 1, 0, 0>();

    treecore::SimdObject<T, 4> re = tmp1 * tmp2 * tmp3;

    tmp1 = v0;
    tmp2 = v1;
    tmp3 = v2;
    tmp1.template shuffle<2, 1, 0, 0>();
    tmp2.template shuffle<1, 0, 2, 0>();
    tmp3.template shuffle<0, 2, 1, 0>();

    re -= tmp1 * tmp2 * tmp3;
    re &= treecore::SimdObject<IntT, 4>(treecore::sized_ones_mask<sizeof(IntT)>::value,
                                        treecore::sized_ones_mask<sizeof(IntT)>::value,
                                        treecore::sized_ones_mask<sizeof(IntT)>::value,
                                        0);
    return re.sum();
}

template
float det3x3<float>(const treecore::SimdObject<float, 4>& v0,
                    const treecore::SimdObject<float, 4>& v1,
                    const treecore::SimdObject<float, 4>& v2) noexcept;


template<typename T>
void gen_quat_rotate(const treecore::SimdObject<T, 4>& quat,
                     treecore::SimdObject<T, 4>& col1,
                     treecore::SimdObject<T, 4>& col2,
                     treecore::SimdObject<T, 4>& col3) noexcept
{
    typedef typename treecore::similar_int<T>::type IntT;

#define _MAT4_FLOAT_ROTATE_OP_ tmp_unit +  tmp_mul * (tmp_a * tmp_b + (tmp_c * tmp_d) | tmp_sign_mask )
    //        m[0]  = 1 - 2 * (y*y + z*z);
    //        m[1]  = 0 + 2 * (x*y + z*w);
    //        m[2]  = 0 + 2 * (z*x - y*w);
    //        m[3]  = 0;
    treecore::SimdObject<T, 4> tmp_unit(T(1),  T(0), T(0), T(0));
    treecore::SimdObject<T, 4> tmp_mul (T(-2), T(2), T(2), T(0));
    treecore::SimdObject<IntT, 4> tmp_sign_mask(0, 0, treecore::float_sign_mask<T>::value, 0);
    treecore::SimdObject<T, 4> tmp_a = quat.template get_shuffle<1, 0, 2, 0>();
    treecore::SimdObject<T, 4> tmp_b = quat.template get_shuffle<1, 1, 0, 0>();
    treecore::SimdObject<T, 4> tmp_c = quat.template get_shuffle<2, 2, 1, 0>();
    treecore::SimdObject<T, 4> tmp_d = quat.template get_shuffle<2, 3, 3, 0>();
    col1 = _MAT4_FLOAT_ROTATE_OP_;

    //        m[4]  = 0 + 2 * (x*y - z*w);
    //        m[5]  = 1 - 2 * (z*z + x*x);
    //        m[6]  = 0 + 2 * (y*z + x*w);
    //        m[7]  = 0;
    tmp_unit.set_all(T(0), T(1),  T(0), T(0));
    tmp_mul .set_all(T(2), T(-2), T(2), T(0));
    tmp_sign_mask.set_all(treecore::float_sign_mask<T>::value, 0, 0, 0);
    tmp_a = quat.template get_shuffle<0, 2, 1, 0>();
    tmp_b = quat.template get_shuffle<1, 2, 2, 0>();
    tmp_c = quat.template get_shuffle<2, 0, 0, 0>();
    tmp_d = quat.template get_shuffle<3, 0, 3, 0>();
    col2 = _MAT4_FLOAT_ROTATE_OP_;

    //        m[8]  = 0 + 2 * (z*x + y*w);
    //        m[9]  = 0 + 2 * (y*z - x*w);
    //        m[10] = 1 - 2 * (x*x + y*y);
    //        m[11] = 0;
    tmp_unit.set_all(T(0), T(0), T(1),  T(0));
    tmp_mul .set_all(T(2), T(2), T(-2), T(0));
    tmp_sign_mask.set_all(0, treecore::float_sign_mask<T>::value, 0, 0);
    tmp_a = quat.template get_shuffle<2, 1, 0, 0>();
    tmp_b = quat.template get_shuffle<0, 2, 0, 0>();
    tmp_c = quat.template get_shuffle<1, 0, 1, 0>();
    tmp_d = quat.template get_shuffle<3, 3, 1, 0>();
    col3 = _MAT4_FLOAT_ROTATE_OP_;
#undef _MAT4_FLOAT_ROTATE_OP_
}

template
void gen_quat_rotate<float>(const treecore::SimdObject<float, 4>& quat,
                            treecore::SimdObject<float, 4>& col1,
                            treecore::SimdObject<float, 4>& col2,
                            treecore::SimdObject<float, 4>& col3) noexcept;

//template
//void gen_quat_rotate<double>(const treecore::SimdObject<double, 4>& quat,
//                             treecore::SimdObject<double, 4>& col1,
//                             treecore::SimdObject<double, 4>& col2,
//                             treecore::SimdObject<double, 4>& col3) noexcept;

} // namespace treeface
