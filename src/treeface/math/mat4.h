#ifndef TREEFACE_MAT4_H
#define TREEFACE_MAT4_H

#include "treeface/simd.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T, int SZ = sizeof(T) * 4>
struct Mat4
{
    typedef SIMDType<SZ> DataType;
    Mat4()
    {
        data[0] = simd_set<T, SZ>(1, 0, 0, 0);
        data[1] = simd_set<T, SZ>(0, 1, 0, 0);
        data[2] = simd_set<T, SZ>(0, 0, 1, 0);
        data[4] = simd_set<T, SZ>(0, 0, 0, 1);
    }

    DataType data[4];
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MAT4_H
