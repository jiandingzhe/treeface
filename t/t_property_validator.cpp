#include "TestFramework.h"
#include "treeface/misc/propertyvalidator.h"

#include <treejuce/Array.h>
#include <treejuce/Holder.h>
#include <treejuce/Identifier.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Object.h>
#include <treejuce/String.h>
#include <treejuce/Variant.h>

using namespace treeface;
using namespace treejuce;

void TestFramework::content()
{
    PropertyValidator validator;
    validator.add_item("prop1", PropertyValidator::ITEM_SCALAR, true);
    validator.add_item("prop2", PropertyValidator::ITEM_SCALAR, false);

    {
        NamedValueSet kv1;
        kv1.set(Identifier("prop1"), var("value"));
        OK(validator.validate(kv1));
    }

    {
        NamedValueSet kv2;
        kv2.set(Identifier("prop1"), var("value"));
        kv2.set(Identifier("prop2"), var("another value"));
        OK(validator.validate(kv2));
    }

    {
        NamedValueSet kv3;
        kv3.set(Identifier("prop1"), var("value"));
        kv3.set(Identifier("prop_not_wanted"), var("foobarbaz"));
        OK(!validator.validate(kv3));
    }

    {
        Array<var> AV;
        NamedValueSet kv4;
        kv4.set(Identifier("prop1"), var("value"));
        kv4.set(Identifier("prop2"), var(AV));
        OK(!validator.validate(kv4));
    }

    {
         Holder<Object> obj = new Object();
         NamedValueSet kv5;
         kv5.set(Identifier("prop1"), var(obj.get()));
         kv5.set(Identifier("prop2"), var("aaaaaaa"));
         OK(!validator.validate(kv5));
    }
}
