#include "TestFramework.h"
#include "treeface/simd.h"

using namespace treeface;

void TestFramework::content()
{
    SIMDType128 a = simd_set<float, SIMDType128>(1, 2, 3, 4);
    IS(a.values_by_float[0], 1);
    IS(a.values_by_float[1], 2);
    IS(a.values_by_float[2], 3);
    IS(a.values_by_float[3], 4);

    SIMDType128 b = simd_set<float, SIMDType128>(5, 6, 7, 8);
    IS(b.values_by_float[0], 5);
    IS(b.values_by_float[1], 6);
    IS(b.values_by_float[2], 7);
    IS(b.values_by_float[3], 8);

    SIMDType128 c = simd_add<float>(a, b);
    IS(c.values_by_float[0], 6);
    IS(c.values_by_float[1], 8);
    IS(c.values_by_float[2], 10);
    IS(c.values_by_float[3], 12);
}
