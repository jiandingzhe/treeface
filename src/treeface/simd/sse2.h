#ifndef TREEFACE_SIMD_FUNC_SSE2_H
#define TREEFACE_SIMD_FUNC_SSE2_H

#include "treeface/simd/template.h"

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
SIMDType128 simd_set<float, SIMDType128>(float a)
{
    SIMDType128 re;
    re.simd_by_float = _mm_set1_ps(a);
    return re;
}

template<>
SIMDType128 simd_add<float, SIMDType128>(SIMDType128 a, SIMDType128 b)
{
    SIMDType128 re;
    re.simd_by_float = _mm_add_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType128 simd_sub<float, SIMDType128>(SIMDType128 a, SIMDType128 b)
{
    SIMDType128 re;
    re.simd_by_float = _mm_sub_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType128 simd_mul<float, SIMDType128>(SIMDType128 a, SIMDType128 b)
{
    SIMDType128 re;
    re.simd_by_float = _mm_mul_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType128 simd_div<float, SIMDType128>(SIMDType128 a, SIMDType128 b)
{
    SIMDType128 re;
    re.simd_by_float = _mm_div_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
float simd_sum<float, SIMDType128>(const SIMDType128& a)
{
    float temp;
    SIMDType128 b=a;
    SIMDType128 c;
    SIMDType128 d;
    SIMDType128 e;
    c=_mm_srli_epi32(a,32);
    d=_mm_srli_epi32(a,64);
    b=_mm_add_ss(b,c);
    b=_mm_add_ss(b,d);
    e=_mm_srli_epi32(a,96);
    b=_mm_add_ss(b,e);
    _mm_store_ss(&temp,b);
    return temp;
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SIMD_FUNC_SSE2_H
