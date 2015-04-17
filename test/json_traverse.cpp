#include <treejuce/JSON.h>
#include <treejuce/Array.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

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

    NamedValueSet& root_kv = root_node.getDynamicObject()->getProperties();
    int root_size = root_kv.size();
    printf("root has %d properties\n", root_size);

    for (int i = 0; i < root_size; i++)
    {
        Identifier key = root_kv.getName(i);
        printf("  %d: %s\n", i, key.toString().toRawUTF8());
        const var& value = root_kv[key];

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
