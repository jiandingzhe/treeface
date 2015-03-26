#ifndef TREEJUCE_SIMD_FUNC_TEMP_H
#define TREEJUCE_SIMD_FUNC_TEMP_H

#include "treejuce/Common.h"

TREEFACE_JUCE_NAMESPACE_BEGIN
template<int SZ>
union SIMDType;

template<int IDX, typename T, int SZ>
inline T simd_get_one(const SIMDType<SZ>& data);

template<int IDX, typename T, int SZ>
inline void simd_set_one(SIMDType<SZ>&data, T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_broadcast(T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(T a, T b, T c, T d);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(T value);

template<typename T, int SZ>
inline SIMDType<SZ> simd_set(const T* values);

template<typename T, int SZ>
inline SIMDType<SZ> simd_add(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_sub(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_mul(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_div(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_and(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_or(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_xor(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<typename T, int SZ>
inline SIMDType<SZ> simd_cmp(const SIMDType<SZ>&a, const SIMDType<SZ>&b);

template<int IDX1, int IDX2, int IDX3, int IDX4, int SZ>
inline SIMDType<SZ> simd_shuffle(const SIMDType<SZ>&input);

template<typename T, int SZ>
T simd_sum(const SIMDType<SZ>&value);

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_SIMD_FUNC_TEMP_H
