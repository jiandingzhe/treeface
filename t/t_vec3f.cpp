#include "treeface/math/vec3.h"
#include "TestFramework.h"

using namespace treeface;

void TestFramework::content()
{
    {
        Vec3f vec;
        IS(vec.x, 0.0f);
        IS(vec.y, 0.0f);
        IS(vec.z, 0.0f);

        vec.set(100.4f, 432.1f, 555.123f);
        IS(vec.x, 100.4f);
        IS(vec.y, 432.1f);
        IS(vec.z, 555.123f);
    }

    {
        Vec3f vec(432.234f, 653.111f, 907.116f);
        IS(vec.x, 432.234f);
        IS(vec.y, 653.111f);
        IS(vec.z, 907.116f);
    }


    {
        Vec3f vec(432.234f, 653.111f, 907.116f);

        vec += Vec3f(666.342f, 1231.433f, 111.34f);
        IS_EPSILON(vec.x, 1098.576f);
        IS_EPSILON(vec.y, 1884.544f);
        IS_EPSILON(vec.z, 1018.456f);

        vec -= Vec3f(1035.20f, 210.511f, 897.11f);
        IS_EPSILON(vec.x, 63.376f);
        IS_EPSILON(vec.y, 1674.033f);
        IS_EPSILON(vec.z, 121.346f);

        vec *= 12.1f;
        IS_EPSILON(vec.x, 766.8496f);
        IS_EPSILON(vec.y, 20255.8f);
        IS_EPSILON(vec.z, 1468.2866f);

        vec /= 55.5f;
        IS_EPSILON(vec.x, 13.81711f);
        IS_EPSILON(vec.y, 364.9694f);
        IS_EPSILON(vec.z, 26.45561f);
    }

    {
        Vec3f vec = Vec3f(111.222f, 333.444f, 555.666f) + Vec3f(123.456f, 789.333f, 987.444f);
        IS_EPSILON(vec.x, 234.678f);
        IS_EPSILON(vec.y, 1122.777f);
        IS_EPSILON(vec.z, 1543.11f);
    }

    {
        Vec3f vec = Vec3f(111.222f, 333.444f, 555.666f) - Vec3f(123.456f, 789.333f, 987.444f);
        IS_EPSILON(vec.x, -12.234f);
        IS_EPSILON(vec.y, -455.889f);
        IS_EPSILON(vec.z, -431.778f);
    }

    {
        Vec3f vec = Vec3f(111.222f, 333.444f, 555.666f) * 12.34;
        IS_EPSILON(vec.x, 1372.479f);
        IS_EPSILON(vec.y, 4114.699f);
        IS_EPSILON(vec.z, 6856.918f);
    }

    {
        Vec3f vec = Vec3f(111.222f, 333.444f, 555.666f) / 12.34;
        IS_EPSILON(vec.x, 9.013128f);
        IS_EPSILON(vec.y, 27.02139f);
        IS_EPSILON(vec.z, 45.02966f);
    }

    {
        float dot = Vec3f(1.0f, 2.0f, 3.0f) * Vec3f(1.0f, 2.0f, 3.0f);
        IS_EPSILON(dot, 14.0f);

        dot = Vec3f(1.0f, 3.0f, 5.0f) * Vec3f(7.0f, 11.0f, 13.0f);
        IS_EPSILON(dot, 105.0f);
    }

    {
        Vec3f cross = Vec3f(1.0f, 0.0f, 0.0f) ^ Vec3f(0.0f, 1.0f, 0.0f);
        IS(cross.x, 0.0f);
        IS(cross.y, 0.0f);
        IS(cross.z, 1.0f);

        cross = Vec3f(0.0f, 0.0f, 1.0f) ^ Vec3f(1.0f, 0.0f, 0.0f);
        IS(cross.x, 0.0f);
        IS(cross.y, 1.0f);
        IS(cross.z, 0.0f);

        cross = Vec3f(0.0f, 1.0f, 0.0f) ^ Vec3f(0.0f, 0.0f, 1.0f);
        IS(cross.x, 1.0f);
        IS(cross.y, 0.0f);
        IS(cross.z, 0.0f);

        cross = Vec3f(1.0f, 1.0f, 0.0f) ^ Vec3f(-1.0f, 1.0f, 0.0f);
        IS(cross.x, 0.0f);
        IS(cross.y, 0.0f);
        IS(cross.z, 2.0f);
    }
}
