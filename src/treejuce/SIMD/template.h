#ifndef TREEJUCE_SIMD_FUNC_TEMP_H
#define TREEJUCE_SIMD_FUNC_TEMP_H

#include "treejuce/Common.h"

TREEFACE_JUCE_NAMESPACE_BEGIN
template<int SZ>
union SIMDType;

template<int IDX, typename T, int SZ>
inline T simd_get_one(SIMDType<SZ> data);

template<int IDX, typename T, int SZ>
inline SIMDType<SZ> simd_set_one(SIMDType<SZ> data, T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_broadcast(T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(T a, T b, T c, T d);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(const T* values);

template<typename T, int SZ>
inline SIMDType<SZ> simd_add(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_sub(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_mul(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_div(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_and(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_or(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_xor(SIMDType<SZ> a, SIMDType<SZ> b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_cmp(SIMDType<SZ> a, SIMDType<SZ> b);

template<int IDX1, int IDX2, int IDX3, int IDX4, int SZ>
inline SIMDType<SZ> simd_shuffle(SIMDType<SZ> input);

template<typename T, int SZ>
T simd_sum(SIMDType<SZ> value);

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_SIMD_FUNC_TEMP_H