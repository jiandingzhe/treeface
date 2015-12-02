#include "treeface/math/mat3.h"
#include "TestFramework.h"


void TestFramework::content()
{
    {
        float det3 = treeface::det3x3(treecore::SimdObject<float, 4>(2.0f, 0.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 2.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 0.0f, 2.0f, 0.0f));
        IS(det3, 8.0f);
    }
    {
        float det3 = treeface::det3x3(treecore::SimdObject<float, 4>(2.0f, 0.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 3.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 0.0f, 5.0f, 0.0f));
        IS(det3, 30.0f);
    }

    {
        treeface::Mat3f mat;
        IS(mat.data[0], 1.0f);
        IS(mat.data[1], 0.0f);
        IS(mat.data[2], 0.0f);

        IS(mat.data[3], 0.0f);
        IS(mat.data[4], 1.0f);
        IS(mat.data[5], 0.0f);

        IS(mat.data[6], 0.0f);
        IS(mat.data[7], 0.0f);
        IS(mat.data[8], 1.0f);


    }

    {

    }
}
