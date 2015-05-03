#ifndef TREEFACE_MAT_UTIL_H
#define TREEFACE_MAT_UTIL_H

#include "treeface/common.h"

#include <treejuce/IntTypes.h>
#include <treejuce/SIMD.h>
#include <treejuce/PlatformDefs.h>

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T) * 4>
T det3x3(const treejuce::SIMDType<SZ>& v0,
         const treejuce::SIMDType<SZ>& v1,
         const treejuce::SIMDType<SZ>& v2) NOEXCEPT;

/**
 * Get determinant value of a 3x3 matrix. For each input SIMD value, only the
 * first 3 components are actually used.
 */
template<>
inline float det3x3<float, 16>(const treejuce::SIMDType<16>& v0,
                               const treejuce::SIMDType<16>& v1,
                               const treejuce::SIMDType<16>& v2) NOEXCEPT
{
//        + v00 * v11 * v22
//        + v02 * v10 * v21
//        + v01 * v12 * v20
//        - v02 * v11 * v20
//        - v01 * v10 * v22
//        - v00 * v12 * v21
    treejuce::SIMDType<16> tmp1 = treejuce::simd_shuffle<0, 2, 1, 0>(v0);
    treejuce::SIMDType<16> tmp2 = treejuce::simd_shuffle<1, 0, 2, 0>(v1);
    treejuce::SIMDType<16> tmp3 = treejuce::simd_shuffle<2, 1, 0, 0>(v2);
    treejuce::SIMDType<16> re = treejuce::simd_mul<float>(tmp1, treejuce::simd_mul<float>(tmp2, tmp3));

    tmp1 = treejuce::simd_shuffle<2, 1, 0, 0>(v0);
    tmp2 = treejuce::simd_shuffle<1, 0, 2, 0>(v1);
    tmp3 = treejuce::simd_shuffle<0, 2, 1, 0>(v2);

    re = treejuce::simd_sub<float>(re, treejuce::simd_mul<float>(tmp1, treejuce::simd_mul<float>(tmp2, tmp3)));
    re = treejuce::simd_and<treejuce::int32>(re, treejuce::simd_set<treejuce::int32, 16>(0xffffffff, 0xffffffff, 0xffffffff, 0));
    return treejuce::simd_sum<float>(re);
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MAT_UTIL_H
