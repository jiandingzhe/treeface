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

    float data[4] = {5, 6, 7, 8};
    Vec4f c(data);
    OK("init with array");
    IS(c.get_x(), 5);
    IS(c.get_y(), 6);
    IS(c.get_z(), 7);
    IS(c.get_w(), 8);

    b += c;
    OK("operator +=");
    IS(b.get_x(), 6);
    IS(b.get_y(), 8);
    IS(b.get_z(), 10);
    IS(b.get_w(), 12);

    Vec4f d = b + c;
    OK("operator +");
    IS(d.get_x(), 11);
    IS(d.get_y(), 14);
    IS(d.get_z(), 17);
    IS(d.get_w(), 20);

    a -= c;
    OK("operator -=");
    IS(a.get_x(), -5);
    IS(a.get_y(), -6);
    IS(a.get_z(), -7);
    IS(a.get_w(), -7);

    d = b - c;
    OK("operator -");
    IS(d.get_x(), 1);
    IS(d.get_y(), 2);
    IS(d.get_z(), 3);
    IS(d.get_w(), 4);

    b *= 3;
    OK("operator *=");
    IS(b.get_x(), 18);
    IS(b.get_y(), 24);
    IS(b.get_z(), 30);
    IS(b.get_w(), 36);

    b /= 2;
    OK("operator /=");
    IS(b.get_x(), 9);
    IS(b.get_y(), 12);
    IS(b.get_z(), 15);
    IS(b.get_w(), 18);

    OK("dot");
    IS(a * b, -348);

    OK("unit vector cross");
    Vec4f v1(1, 0, 0, 0);
    Vec4f v2(0, 1, 0, 0);
    Vec4f v3 = v1 ^ v2;
    IS(v3.get_x(), 0);
    IS(v3.get_y(), 0);
    IS(v3.get_z(), 1);
    IS(v3.get_w(), 0);

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
