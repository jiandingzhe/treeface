#include "TestFramework.h"
#include "treecore/RefCountObject.h"
#include "treecore/RefCountHolder.h"

using namespace treecore;

class TestType: public RefCountObject
{
public:
    TestType(bool* marker): live_marker(marker)
    {
        *live_marker = true;
    }

    virtual ~TestType()
    {
        *live_marker = false;
    }

    bool* live_marker;
};

typedef RefCountHolder<TestType> TestHolder;

void TestFramework::content()
{
    bool marker1 = false;
    TestType* obj1 = new TestType(&marker1);

    OK(obj1);
    OK(marker1);
    IS(obj1->get_ref_count(), obj1->ms_count);
    IS(obj1->ms_count, 0);

    obj1->ref();
    IS(obj1->get_ref_count(), 1);

    {
        TestHolder holder1(obj1);
        IS(obj1->get_ref_count(), 2);

        TestHolder holder2(holder1);
        IS(obj1->get_ref_count(), 3);

        TestHolder holder3(obj1);
        IS(obj1->get_ref_count(), 4);
    }

    IS(obj1->get_ref_count(), 1);
    OK(marker1);

    obj1->unref();
    OK(!marker1);
}
