#include "treeface/common.h"

#include <treejuce/Logger.h>
#include <treejuce/String.h>
#include <treejuce/StringRef.h>

#include <cstdarg>
#include <cstdio>

using namespace std;
using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

void die(StringRef format, ...)
{
    char buffer[65536];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 65536, (const char*)(format), args);
    va_end(args);

    Logger::writeToLog("DIE: " + String(buffer));

    abort();
}

void warn(StringRef format, ...)
{
    char buffer[65536];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 65536, (const char*)(format), args);
    va_end(args);

    Logger::writeToLog("WARN: " + String(buffer)+"\n");
}

TREEFACE_NAMESPACE_END
