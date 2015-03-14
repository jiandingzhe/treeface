#ifndef TREEFACE_SIMD_FUNC_SSE2_H
#define TREEFACE_SIMD_FUNC_SSE2_H

#include "treeface/SIMDFunc_temp.h"

#include <cstdint>
#include <emmintrin.h>

TREEFACE_NAMESPACE_BEGIN

union SIMDType128
{
    __m128       simd_by_float;
    __m128i      simd_by_int;
    __m128d      simd_by_double;
    float        values_by_float[4];
    std::int32_t values_by_int[4];
    double       values_by_double[2];
};

template<>
SIMDType128 simd_set<float, SIMDType128>(float a, float b, float c, float d)
{
    SIMDType128 re;
    re.simd_by_float = _mm_set_ps(d, c, b, a);
    return re;
}

template<>
SIMDType128 simd_add<float, SIMDType128>(SIMDType128 a, SIMDType128 b)
{
    SIMDType128 re;
    re.simd_by_float = _mm_add_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SIMD_FUNC_SSE2_H
