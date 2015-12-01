#ifndef TREEFACE_MAT_UTIL_H
#define TREEFACE_MAT_UTIL_H

#include "treeface/common.h"

#include <treecore/IntTypes.h>
#include <treecore/SimdObject.h>
#include <treecore/PlatformDefs.h>

namespace treeface {

template<typename T>
T det3x3(const treecore::SimdObject<T, 4>& v0,
         const treecore::SimdObject<T, 4>& v1,
         const treecore::SimdObject<T, 4>& v2) noexcept;

/**
 * Get determinant value of a 3x3 matrix. For each input SIMD value, only the
 * first 3 components are actually used.
 */
template<>
inline float det3x3<float>(const treecore::SimdObject<float, 4>& v0,
                           const treecore::SimdObject<float, 4>& v1,
                           const treecore::SimdObject<float, 4>& v2) noexcept
{
//        + v00 * v11 * v22 - v02 * v11 * v20
//        + v02 * v10 * v21 - v01 * v10 * v22
//        + v01 * v12 * v20 - v00 * v12 * v21
    treecore::SimdObject<float, 4> tmp1(v0);
    treecore::SimdObject<float, 4> tmp2(v1);
    treecore::SimdObject<float, 4> tmp3(v2);
    tmp1.shuffle<0, 2, 1, 0>();
    tmp2.shuffle<1, 0, 2, 0>();
    tmp3.shuffle<2, 1, 0, 0>();

    treecore::SimdObject<float, 4> re = tmp1 * tmp2 * tmp3;

    tmp1 = v0;
    tmp2 = v1;
    tmp3 = v2;
    tmp1.shuffle<2, 1, 0, 0>();
    tmp2.shuffle<1, 0, 2, 0>();
    tmp3.shuffle<0, 2, 1, 0>();

    re -= tmp1 * tmp2 * tmp3;
    re &= treecore::SimdObject<treecore::int32, 4>(0xffffffff, 0xffffffff, 0xffffffff, 0);
    return re.sum();
}

} // namespace treeface

#endif // TREEFACE_MAT_UTIL_H
