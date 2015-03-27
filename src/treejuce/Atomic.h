#ifndef TREEJUCE_ATOMIC_H
#define TREEJUCE_ATOMIC_H

#ifdef __GNUC__
#  include "treejuce/Atomic/gcc.h"
#elif defined _MSC_VER
#  include "treejuce/Atomic/msvc.h"
#else
#  error "atomic support is not implemented"
#endif

#endif // TREEJUCE_ATOMIC_H
