#include "treeface/common.h"

#include "treejuce/String.h"
#include "treejuce/StringRef.h"

#include <cstdarg>
#include <cstdio>

using namespace std;

TREEFACE_NAMESPACE_BEGIN

void die(treejuce::StringRef format, ...)
{
    treejuce::String format_mod(format.text);
    format_mod += '\n';

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format_mod.toRawUTF8(), args);
    va_end(args);
    abort();
}

TREEFACE_NAMESPACE_END
