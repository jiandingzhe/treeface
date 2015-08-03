#include "treeface/common.h"

#include <treecore/Logger.h>
#include <treecore/String.h>
#include <treecore/StringRef.h>

#include <cstdarg>
#include <cstdio>

using namespace std;
using namespace treecore;

namespace treeface {

void die(const char* format, ...)
{
    char buffer[65536];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 65536, format, args);
    va_end(args);

    Logger::writeToLog("DIE: " + String(buffer));

    abort();
}

void warn(const char* format, ...)
{
    char buffer[65536];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 65536, format, args);
    va_end(args);

    Logger::writeToLog("WARN: " + String(buffer)+"\n");
}

} // namespace treeface
