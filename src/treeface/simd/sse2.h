#ifndef TREEFACE_SIMD_FUNC_SSE2_H
#define TREEFACE_SIMD_FUNC_SSE2_H

#include "treeface/simd/template.h"

#include <cstdint>
#include <emmintrin.h>

TREEFACE_NAMESPACE_BEGIN

template<>
union SIMDType<16>
{
    __m128       simd_by_float;
    __m128i      simd_by_int;
    __m128d      simd_by_double;
    float        values_by_float[4];
    std::int32_t values_by_int[4];
    double       values_by_double[2];
};

template<>
inline float simd_get_one<0, float, 16>(SIMDType<16> data)
{
    return data.simd_by_float[0];
}

template<>
inline float simd_get_one<1, float, 16>(SIMDType<16> data)
{
    return data.simd_by_float[1];
}

template<>
inline float simd_get_one<2, float, 16>(SIMDType<16> data)
{
    return data.simd_by_float[2];
}

template<>
inline float simd_get_one<3, float, 16>(SIMDType<16> data)
{
    return data.simd_by_float[3];
}

template<>
inline SIMDType<16> simd_set_one<0, float, 16>(SIMDType<16> data, float value)
{
    data.simd_by_float[0] = value;
    return data;
}

template<>
inline SIMDType<16> simd_set_one<1, float, 16>(SIMDType<16> data, float value)
{
    data.simd_by_float[1] = value;
    return data;
}

template<>
inline SIMDType<16> simd_set_one<2, float, 16>(SIMDType<16> data, float value)
{
    data.simd_by_float[2] = value;
    return data;
}

template<>
inline SIMDType<16> simd_set_one<3, float, 16>(SIMDType<16> data, float value)
{
    data.simd_by_float[3] = value;
    return data;
}

template<>
SIMDType<16> simd_set<float, 16> (float a, float b, float c, float d)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_set_ps(d, c, b, a);
    return re;
}

template<>
SIMDType<16> simd_set<float, 16>(float value)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_set1_ps(value);
    return re;
}

template<>
SIMDType<16> simd_set<float, 16> (const float* values)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_loadu_ps(values);
    return re;
}

template<>
SIMDType<16> simd_add<float, 16> (SIMDType<16> a, SIMDType<16>  b)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_add_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType<16> simd_sub<float, 16> (SIMDType<16> a, SIMDType<16>  b)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_sub_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType<16> simd_mul<float, 16> (SIMDType<16> a, SIMDType<16>  b)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_mul_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
SIMDType<16> simd_div<float, 16> (SIMDType<16> a, SIMDType<16>  b)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_div_ps(a.simd_by_float, b.simd_by_float);
    return re;
}


template<>
SIMDType<16> simd_cmp<float, 16> (SIMDType<16> a, SIMDType<16>  b)
{
    SIMDType<16> re;
    re.simd_by_float = _mm_cmpeq_ps(a.simd_by_float, b.simd_by_float);
    return re;
}

template<>
float simd_sum<float, 16> (SIMDType<16> value)
{
    return value.simd_by_float[0] +
            value.simd_by_float[1] +
            value.simd_by_float[2] +
            value.simd_by_float[3];
}

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SIMD_FUNC_SSE2_H
