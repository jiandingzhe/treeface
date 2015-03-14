#ifndef TREEFACE_MAT4_H
#define TREEFACE_MAT4_H

#include "treeface/common.h"

TREEFACE_NAMESPACE_BEGIN

template<typename T>
struct Mat4
{
    Mat4() {}


    float data[4][4] = {{1, 0, 0, 0},
                        {0, 1, 0, 0},
                        {0, 0, 1, 0},
                        {0, 0, 0, 1}};
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MAT4_H
