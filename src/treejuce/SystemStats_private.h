#ifndef TREEJUCE_SYSTEM_STATS_PRIVATE_H
#define TREEJUCE_SYSTEM_STATS_PRIVATE_H

#include "treejuce/Common.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

struct CPUInformation
{
    CPUInformation() NOEXCEPT
        : numCpus (0), hasMMX (false), hasSSE (false),
          hasSSE2 (false), hasSSE3 (false), has3DNow (false)
    {
        initialise();
    }

    void initialise() NOEXCEPT;

    int numCpus;
    bool hasMMX, hasSSE, hasSSE2, hasSSE3, has3DNow;
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_SYSTEM_STATS_PRIVATE_H
