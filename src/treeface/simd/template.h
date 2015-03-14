#ifndef TREEFACE_SIMD_FUNC_TEMP_H
#define TREEFACE_SIMD_FUNC_TEMP_H

#include "treeface/Common.h"

TREEFACE_NAMESPACE_BEGIN

union SIMDType128;

template<typename T, typename DATA_T>
inline DATA_T simd_set(T a, T b, T c, T d);

template<typename T, typename DATA_T>
inline DATA_T simd_add(const DATA_T a, const DATA_T b);

template<typename T, typename DATA_T>
inline DATA_T simd_sub(const DATA_T a, const DATA_T b);

template<typename T, typename DATA_T>
inline DATA_T simd_mul(const DATA_T a, const DATA_T b);

template<typename T, typename DATA_T>
inline DATA_T simd_div(const DATA_T a, const DATA_T b);

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SIMD_FUNC_TEMP_H
