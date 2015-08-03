#ifndef TREEFACE_COMMON_H
#define TREEFACE_COMMON_H

#include <treecore/Common.h>

#define STRINGIFY(content) #content
#define EXPAND_AND_STRINGIFY(input) STRINGIFY(input)

namespace treeface
{

void die(const char* format, ...);
void warn(const char* format, ...);

} // namespace treeface

#endif // TREEFACE_COMMON_H
