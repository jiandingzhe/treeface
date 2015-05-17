#include "TestFramework.h"

#include "treeface/nedbase.h"

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
}
