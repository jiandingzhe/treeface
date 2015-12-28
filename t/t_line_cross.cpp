#include "TestFramework.h"
#include "treeface/graphics/guts/utils.h"

using treeface::Vec2f;

void TestFramework::content()
{
    {
        Vec2f p1(0.5f, 1.5f);
        Vec2f p2(3.5f, 1.5f);

        Vec2f p3(2.0f, 1.0f);
        Vec2f p4(2.0f, 2.0f);

        Vec2f cross;
        OK(cross_test_exc(p1, p2, p2-p1, p3, p4, p4-p3, cross));
        IS(cross.x, 2.0f);
        IS(cross.y, 1.5f);

        cross.set(0.0f, 0.0f);
        OK(cross_test_exc(p3, p4, p4-p3, p1, p2, p2-p1, cross));
        IS(cross.x, 2.0f);
        IS(cross.y, 1.5f);
    }

    {
        Vec2f p1(0.5f, 1.5f);
        Vec2f p2(3.5f, 1.5f);

        Vec2f p3(10.0f, 20.0f);
        Vec2f p4(3.5f, 1.5f);

        Vec2f cross;
        OK(! cross_test_exc(p1, p2, p2-p1, p3, p4, p4-p3, cross));
        IS(cross.x, 3.5f);
        IS(cross.y, 1.5f);

        cross.set(0.0f, 0.0f);
        OK(! cross_test_exc(p1, p2, p2-p1, p4, p3, p3-p4, cross));
        IS(cross.x, 3.5f);
        IS(cross.y, 1.5f);
    }
}
