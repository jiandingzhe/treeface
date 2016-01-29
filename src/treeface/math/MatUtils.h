#ifndef TREEFACE_MAT_UTIL_H
#define TREEFACE_MAT_UTIL_H

#include "treeface/base/Common.h"

#include <treecore/SimdObject.h>

namespace treeface {

///
/// @brief get determinant value of a 3x3 matrix
///
/// For each input SIMD value, only the first 3 components are actually used.
///
template<typename T>
T det3x3(const treecore::SimdObject<T, 4>& v0,
         const treecore::SimdObject<T, 4>& v1,
         const treecore::SimdObject<T, 4>& v2) noexcept;

extern template
float det3x3<float>(const treecore::SimdObject<float, 4>& v0,
                    const treecore::SimdObject<float, 4>& v1,
                    const treecore::SimdObject<float, 4>& v2) noexcept;


template<typename T>
void gen_quat_rotate(const treecore::SimdObject<T, 4>& quat,
                     treecore::SimdObject<T, 4>& col1,
                     treecore::SimdObject<T, 4>& col2,
                     treecore::SimdObject<T, 4>& col3) noexcept;

// instantiate float
extern template
void gen_quat_rotate<float>(const treecore::SimdObject<float, 4>& quat,
                            treecore::SimdObject<float, 4>& col1,
                            treecore::SimdObject<float, 4>& col2,
                            treecore::SimdObject<float, 4>& col3) noexcept;

} // namespace treeface

#endif // TREEFACE_MAT_UTIL_H
