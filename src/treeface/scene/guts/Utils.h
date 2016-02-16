#ifndef TREEFACE_SCENE_GUTS_UTILS_H
#define TREEFACE_SCENE_GUTS_UTILS_H

#include "treeface/misc/UniversalValue.h"
#include <treecore/HashMap.h>
#include <treecore/Identifier.h>

namespace treeface
{

typedef std::pair<GLint, UniversalValue>         UniformKV;
typedef treecore::HashMap<treecore::Identifier, UniversalValue> UniformMap;

inline void collect_uniforms(const UniformMap& store, UniformMap result)
{
    for (UniformMap::ConstIterator it(store); it.next(); )
        result.tryInsert(it.key(), it.value());
}

} // namespace treeface

#endif // TREEFACE_SCENE_GUTS_UTILS_H
