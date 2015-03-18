#include "TestFramework.h"
#include "treeface/simd.h"

using namespace treeface;

void TestFramework::content()
{
    OK("set");
    SIMDType<16> a = simd_set<float, 16>(1, 2, 3, 4);
    IS(a.values_by_float[0], 1);
    IS(a.values_by_float[1], 2);
    IS(a.values_by_float[2], 3);
    IS(a.values_by_float[3], 4);

    OK("binary layout");
    float v0 = simd_get_one<0, float>(a);
    float v1 = simd_get_one<1, float>(a);
    float v2 = simd_get_one<2, float>(a);
    float v3 = simd_get_one<3, float>(a);
    IS(v0, a.values_by_float[0]);
    IS(v1, a.values_by_float[1]);
    IS(v2, a.values_by_float[2]);
    IS(v3, a.values_by_float[3]);

    OK("sum");
    float sum = simd_sum<float>(a);
    IS(sum, v0 + v1 + v2 + v3);

    OK("add");
    SIMDType<16> b = simd_set<float, 16>(5, 6, 7, 8);
    IS(b.values_by_float[0], 5);
    IS(b.values_by_float[1], 6);
    IS(b.values_by_float[2], 7);
    IS(b.values_by_float[3], 8);

    SIMDType<16> c = simd_add<float>(a, b);
    IS(c.values_by_float[0], 6);
    IS(c.values_by_float[1], 8);
    IS(c.values_by_float[2], 10);
    IS(c.values_by_float[3], 12);

    OK("sub");
    c = simd_sub<float>(a, b);
    IS(c.values_by_float[0], -4);
    IS(c.values_by_float[1], -4);
    IS(c.values_by_float[2], -4);
    IS(c.values_by_float[3], -4);

    OK("mul");
    c = simd_mul<float>(a, b);
    IS(c.values_by_float[0], 5);
    IS(c.values_by_float[1], 12);
    IS(c.values_by_float[2], 21);
    IS(c.values_by_float[3], 32);

    OK("div");
    c = simd_div<float>(a, b);
    IS(c.values_by_float[0], float(1)/5);
    IS(c.values_by_float[1], float(2)/6);
    IS(c.values_by_float[2], float(3)/7);
    IS(c.values_by_float[3], float(4)/8);

    OK("shuffle 3 2 1 0");
    c = simd_shuffle<3, 2, 1, 0>(a);
    IS(c.values_by_float[0], 4);
    IS(c.values_by_float[1], 3);
    IS(c.values_by_float[2], 2);
    IS(c.values_by_float[3], 1);

    OK("shuffle 1 0 3 2");
    c = simd_shuffle<1, 0, 3, 2>(a);
    IS(c.values_by_float[0], 2);
    IS(c.values_by_float[1], 1);
    IS(c.values_by_float[2], 4);
    IS(c.values_by_float[3], 3);
}
