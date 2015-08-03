#ifndef TREEFACE_MAT_UTIL_H
#define TREEFACE_MAT_UTIL_H

#include "treeface/common.h"

#include <treecore/IntTypes.h>
#include <treecore/SIMD.h>
#include <treecore/PlatformDefs.h>

namespace treeface {

template<typename T, int SZ = sizeof(T) * 4>
T det3x3(const treecore::SIMDType<SZ>& v0,
         const treecore::SIMDType<SZ>& v1,
         const treecore::SIMDType<SZ>& v2) NOEXCEPT;

/**
 * Get determinant value of a 3x3 matrix. For each input SIMD value, only the
 * first 3 components are actually used.
 */
template<>
inline float det3x3<float, 16>(const treecore::SIMDType<16>& v0,
                               const treecore::SIMDType<16>& v1,
                               const treecore::SIMDType<16>& v2) NOEXCEPT
{
//        + v00 * v11 * v22
//        + v02 * v10 * v21
//        + v01 * v12 * v20
//        - v02 * v11 * v20
//        - v01 * v10 * v22
//        - v00 * v12 * v21
    treecore::SIMDType<16> tmp1 = treecore::simd_shuffle<0, 2, 1, 0>(v0);
    treecore::SIMDType<16> tmp2 = treecore::simd_shuffle<1, 0, 2, 0>(v1);
    treecore::SIMDType<16> tmp3 = treecore::simd_shuffle<2, 1, 0, 0>(v2);
    treecore::SIMDType<16> re = treecore::simd_mul<float>(tmp1, treecore::simd_mul<float>(tmp2, tmp3));

    tmp1 = treecore::simd_shuffle<2, 1, 0, 0>(v0);
    tmp2 = treecore::simd_shuffle<1, 0, 2, 0>(v1);
    tmp3 = treecore::simd_shuffle<0, 2, 1, 0>(v2);

    re = treecore::simd_sub<float>(re, treecore::simd_mul<float>(tmp1, treecore::simd_mul<float>(tmp2, tmp3)));
    re = treecore::simd_and<treecore::int32>(re, treecore::simd_set<treecore::int32, 16>(0xffffffff, 0xffffffff, 0xffffffff, 0));
    return treecore::simd_sum<float>(re);
}

} // namespace treeface

#endif // TREEFACE_MAT_UTIL_H
