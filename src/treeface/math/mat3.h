#ifndef TREEFACE_MAT3_H
#define TREEFACE_MAT3_H

#include <cstring>
#include "treeface/common.h"
#include "treeface/math/matutils.h"

namespace treeface {

template<typename T>
struct Mat3
{
    Mat3()
    {
        data[0] = 1;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 1;
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;
        data[8] = 1;
    }

    Mat3(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
    {
        data[0] = v0;
        data[1] = v1;
        data[2] = v2;
        data[3] = v3;
        data[4] = v4;
        data[5] = v5;
        data[6] = v6;
        data[7] = v7;
        data[8] = v8;
    }

    void transpose() NOEXCEPT
    {
        T tmp[9] = {data[0], data[3], data[6],
                    data[1], data[4], data[7],
                    data[2], data[5], data[8]};
        memcpy(data, tmp, 9 * sizeof(T));
    }

    T inverse() NOEXCEPT
    {
        T det = determinant();
        if (det == 0)
            return det;

        T tmp[9];
        tmp[0] = (data[4] * data[8] - data[5] * data[7]) / det; // 0
        tmp[1] = (data[1] * data[8] - data[2] * data[7]) / det; // 3
        tmp[2] = (data[1] * data[5] - data[2] * data[4]) / det; // 6

        tmp[3] = (data[3] * data[8] - data[5] * data[6]) / det; // 1
        tmp[4] = (data[0] * data[8] - data[2] * data[6]) / det; // 4
        tmp[5] = (data[0] * data[5] - data[2] * data[3]) / det; // 7

        tmp[6] = (data[3] * data[7] - data[4] * data[6]) / det; // 2
        tmp[7] = (data[0] * data[7] - data[1] * data[6]) / det; // 5
        tmp[8] = (data[0] * data[4] - data[1] * data[3]) / det; // 8

        std::memcpy(data, tmp, 9 * sizeof(T));
        return det;
    }

    T determinant() NOEXCEPT
    {
        return det3x3<T>(treecore::simd_set<float,16>(data[0], data[1], data[2], 0),
                         treecore::simd_set<float,16>(data[3], data[4], data[5], 0),
                         treecore::simd_set<float,16>(data[6], data[7], data[8], 0));
    }

    T data[9];
};

typedef Mat3<float> Mat3f;


} // namespace treeface

#endif // TREEFACE_MAT3_H
