#include "TestFramework.h"

#include "treeface/nedbase.h"
#include "treeface/math/vec4.h"
#include "nedmalloc.h"

#include <treejuce/Holder.h>

using namespace treeface;
using namespace treejuce;

struct Foo: public NedBase
{
    int a = 0;
    float b = 0;
};

struct Bar
{
    Foo c;
};

void TestFramework::content()
{
    Foo* foo = new Foo();
    OK(foo);
    Bar* bar = new Bar();
    OK(bar);

    Vec4f* baz = (Vec4f*) nedalloc::nedmalloc2(sizeof(Vec4f), sizeof(float)*4);
    OK(baz);
    new (baz) Vec4f(1,2,3,4);
    IS(baz->get_x(), 1);
    IS(baz->get_y(), 2);
    IS(baz->get_z(), 3);
    IS(baz->get_w(), 4);
}
