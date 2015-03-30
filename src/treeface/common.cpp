#include "treeface/common.h"

#include "treejuce/String.h"

#include <cstdarg>
#include <cstdio>

using namespace std;

TREEFACE_NAMESPACE_BEGIN

void die(const char* format, ...)
{
    treejuce::String format_mod(format);
    format_mod += '\n';

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format_mod.toRawUTF8(), args);
    va_end(args);
    abort();
}

TREEFACE_NAMESPACE_END
