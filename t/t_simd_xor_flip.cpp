#include "TestFramework.h"
#include <treecore/SimdObject.h>
#include <treecore/FloatUtils.h>
#include <treecore/IntUtils.h>

using namespace treecore;

void TestFramework::content()
{
    typedef typename similar_int<float>::type IntT;
    SimdObject<float, 4> result =
            SimdObject<float, 4>(123.456f, -789.123f, -444.222f, 333.111f)
            ^
            SimdObject<IntT, 4>(0, float_sign_mask<float>::value, 0, float_sign_mask<float>::value);

    IS(result.get<0>(), 123.456f);
    IS(result.get<1>(), 789.123f);
    IS(result.get<2>(), -444.222f);
    IS(result.get<3>(), -333.111f);
}
