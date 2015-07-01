#include "treeface/math/vec4.h"
#include "TestFramework.h"

#include <string.h>

using namespace treeface;

void TestFramework::content()
{
    Vec4f a;
    OK("init");
    IS(a.get_x(), 0);
    IS(a.get_y(), 0);
    IS(a.get_z(), 0);
    IS(a.get_w(), 1);

    Vec4f b(1, 2, 3, 4);
    OK("init with values");
    IS(b.get_x(), 1);
    IS(b.get_y(), 2);
    IS(b.get_z(), 3);
    IS(b.get_w(), 4);

    OK("validate binary layout");
    float expect[4] = {1, 2, 3, 4};
    IS(memcmp(&b, expect, sizeof(float)*4), 0);

    OK("copy constructor");
    {
        Vec4f cpy(b);
        IS(cpy.get_x(), 1);
        IS(cpy.get_y(), 2);
        IS(cpy.get_z(), 3);
        IS(cpy.get_w(), 4);
    }

    OK("operator =");
    {
        Vec4f assign(2, 3, 4, 5);
        assign = b;
        IS(assign.get_x(), 1);
        IS(assign.get_y(), 2);
        IS(assign.get_z(), 3);
        IS(assign.get_w(), 4);
    }

    OK("init with array");
    float data[4] = {5, 6, 7, 8};
    Vec4f c(data);
    IS(c.get_x(), 5);
    IS(c.get_y(), 6);
    IS(c.get_z(), 7);
    IS(c.get_w(), 8);

    {
        OK("operator +=");
        Vec4f re = b;
        re += c;
        IS(re.get_x(), 6);
        IS(re.get_y(), 8);
        IS(re.get_z(), 10);
        IS(re.get_w(), 12);
        IS(b.get_x(), 1);
        IS(b.get_y(), 2);
        IS(b.get_z(), 3);
        IS(b.get_w(), 4);
    }

    {
        OK("operator +");
        Vec4f re = b + c;
        IS(re.get_x(), 6);
        IS(re.get_y(), 8);
        IS(re.get_z(), 10);
        IS(re.get_w(), 12);
    }

    {
        OK("operator -=");
        Vec4f re = b;
        re -= c;
        IS(re.get_x(), -4);
        IS(re.get_y(), -4);
        IS(re.get_z(), -4);
        IS(re.get_w(), -4);
    }

    {
        OK("operator -");
        Vec4f re = b - c;
        IS(re.get_x(), -4);
        IS(re.get_y(), -4);
        IS(re.get_z(), -4);
        IS(re.get_w(), -4);
    }

    {
        OK("operator *=");
        Vec4f re(9, 8, 7, 6);
        re *= 5;
        IS(re.get_x(), 45);
        IS(re.get_y(), 40);
        IS(re.get_z(), 35);
        IS(re.get_w(), 30);
    }

    {
        OK("operator /=");
        Vec4f re(45, 50, 55, 60);
        re /= 5;
        IS(re.get_x(), 9);
        IS(re.get_y(), 10);
        IS(re.get_z(), 11);
        IS(re.get_w(), 12);
    }

    OK("dot");
    IS(b * c, 70);

    OK("unit vector cross");
    Vec4f v1(1, 0, 0, 0);
    Vec4f v2(0, 1, 0, 0);
    Vec4f v3 = v1 ^ v2;
    IS(v3.get_x(), 0);
    IS(v3.get_y(), 0);
    IS(v3.get_z(), 1);
    IS(v3.get_w(), 0);

    {
        OK("normalize");
        Vec4f v(1,2,3,4);
        v.normalize();
        IS_EPSILON(v.length(), 1.0f);
    }

    Vec4f v4 = v2 ^ v3;
    IS(v4.get_x(), 1);
    IS(v4.get_y(), 0);
    IS(v4.get_z(), 0);
    IS(v4.get_w(), 0);

    OK("operator ==");
    OK(v4 == v1);
    OK(!(v4 == v2));

    OK("operator !=");
    OK(v4 != v2);
    OK(!(v4 != v1));
}
