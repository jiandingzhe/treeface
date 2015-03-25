#ifndef TREEJUCE_SIMD_FUNC_H
#define TREEJUCE_SIMD_FUNC_H

#include "treeface/config.h"

#ifdef TREEFACE_CPU_x86_64
#  include "treejuce/SIMD/sse2.h"
#else
#  warn "SIMD function wrapper for CPU is not implemented. Using non-simd fallback implementation."
#  include "treejuce/SIMD/fallback.h"
#endif

#endif // TREEJUCE_SIMD_FUNC_H
