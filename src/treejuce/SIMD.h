#ifndef TREEJUCE_SIMD_FUNC_H
#define TREEJUCE_SIMD_FUNC_H

#include "treejuce/TargetPlatform.h"

#if defined JUCE_INTEL
#  include "treejuce/SIMD/sse2.h"
#elif defined JUCE_ARM
#  include "treejuce/SIMD/neon.h"
#else
#  error "SIMD function wrapper for CPU is not implemented"
#endif

#endif // TREEJUCE_SIMD_FUNC_H
