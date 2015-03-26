#ifndef TREEJUCE_SIMD_FUNC_H
#define TREEJUCE_SIMD_FUNC_H

#include "treeface/config.h"

#if defined TREEFACE_CPU_X86_64
#  include "treejuce/SIMD/sse2.h"
#else
#  error "SIMD function wrapper for CPU is not implemented"
#endif

#endif // TREEJUCE_SIMD_FUNC_H
