#include <treecore/JSON.h>
#include <treecore/Array.h>
#include <treecore/DynamicObject.h>
#include <treecore/Result.h>
#include <treecore/Variant.h>

using namespace treecore;

const char* src =
        "{"
        "   \"a\": \"b\","
        "   \"c\": 123"
        "}"
        ;

int main(int argc, char** argv)
{
    var root_node;
    Result parser_re = JSON::parse(src, root_node);

    if (!parser_re)
    {
        fprintf(stderr, "failed to parse JSON:\n"
                        "%s\n",
                parser_re.getErrorMessage().toRawUTF8());
        exit(1);
    }

    if (!root_node.isObject())
    {
        fprintf(stderr, "root node does not hold an object\n");
        exit(1);
    }

    NamedValueSet::MapType& root_kv = root_node.getDynamicObject()->getProperties().getValues();
    NamedValueSet::MapType::ConstIterator i_root_kv(root_kv);
    int root_size = root_kv.size();
    printf("root has %d properties\n", root_size);

    while (i_root_kv.next())
    {
        Identifier key = i_root_kv.key();
        printf("  %s\n", key.toString().toRawUTF8());
        const var& value = i_root_kv.value();

        if (value.isString())
        {
            printf("  string value %s\n", String(value.toString()).toRawUTF8());
        }
        else if (value.isInt())
        {
            printf("  int value %s\n", String(value.toString()).toRawUTF8());
        }
    }
}
