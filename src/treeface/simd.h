#ifndef TREEFACE_SIMD_FUNC_H
#define TREEFACE_SIMD_FUNC_H

#include "treeface/config.h"

#if TREEFACE_CPU_x86_64
#  include "treeface/simd/sse2.h"
#else
#  warn "SIMD function wrapper for CPU is not implemented. Using non-simd fallback implementation."
#  include "treeface/fallback.h"
#endif

#endif // TREEFACE_SIMD_FUNC_H
