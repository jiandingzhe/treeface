#include "treeface/math/vec4.h"
#include "TestFramework.h"

using namespace treeface;

void TestFramework::content()
{
    Vec4f a;
    OK("init");
    IS(a.data[0], 0);
    IS(a.data[1], 0);
    IS(a.data[2], 0);
    IS(a.data[3], 1);

    Vec4f b(1, 2, 3, 4);
    OK("init with values");
    IS(b.data[0], 1);
    IS(b.data[1], 2);
    IS(b.data[2], 3);
    IS(b.data[3], 4);

    float data[4] = {5, 6, 7, 8};
    Vec4f c(data);
    OK("init with array");
    IS(c.data[0], 5);
    IS(c.data[1], 6);
    IS(c.data[2], 7);
    IS(c.data[3], 8);

    b += c;
    OK("operator +=");
    IS(b.data[0], 6);
    IS(b.data[1], 8);
    IS(b.data[2], 10);
    IS(b.data[3], 12);

    Vec4f d = b + c;
    OK("operator +");
    IS(d.data[0], 11);
    IS(d.data[1], 14);
    IS(d.data[2], 17);
    IS(d.data[3], 20);

    a -= c;
    OK("operator -=");
    IS(a.data[0], -5);
    IS(a.data[1], -6);
    IS(a.data[2], -7);
    IS(a.data[3], -7);

    d = b - c;
    OK("operator -");
    IS(d.data[0], 1);
    IS(d.data[1], 2);
    IS(d.data[2], 3);
    IS(d.data[3], 4);

    b *= 3;
    OK("operator *=");
    IS(b.data[0], 18);
    IS(b.data[1], 24);
    IS(b.data[2], 30);
    IS(b.data[3], 36);

    b /= 2;
    OK("operator /=");
    IS(b.data[0], 9);
    IS(b.data[1], 12);
    IS(b.data[2], 15);
    IS(b.data[3], 18);

    OK("dot");
    IS(a * b, -348);

    OK("unit vector cross");
    Vec4f v1(1, 0, 0, 0);
    Vec4f v2(0, 1, 0, 0);
    Vec4f v3 = v1 ^ v2;
    IS(v3.data[0], 0);
    IS(v3.data[1], 0);
    IS(v3.data[2], 1);
    IS(v3.data[3], 0);

    Vec4f v4 = v2 ^ v3;
    IS(v4.data[0], 1);
    IS(v4.data[1], 0);
    IS(v4.data[2], 0);
    IS(v4.data[3], 0);

    OK("operator ==");
    OK(v4 == v1);
    OK(!(v4 == v2));

    OK("operator !=");
    OK(v4 != v2);
    OK(!(v4 != v1));
}
