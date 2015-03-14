#ifndef TREEFACE_SIMD_FUNC_H
#define TREEFACE_SIMD_FUNC_H

#include "treeface/Config.h"

#ifdef TREEFACE_CPU_x86_64
#  include "treeface/SIMDFunc_SSE2.h"
#else
#  warn "SIMD function wrapper for CPU is not implemented. Using non-simd fallback implementation."
#  include "treeface/SIMDFunc_fallback.h"
#endif

#endif // TREEFACE_SIMD_FUNC_H
