#ifndef TREEFACE_ATOMIC_H
#define TREEFACE_ATOMIC_H

#ifdef __GNUC__
#  include "treeface/atomic/gcc.h"
#elif defined _MSVC_VER
#  include "treeface/atomic/msvc.h"
#else
#  error "atomic support is not implemented"
#endif

#endif // TREEFACE_ATOMIC_H
