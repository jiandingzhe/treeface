#ifndef TREEFACE_POST_SURFACE_GUTS_H
#define TREEFACE_POST_SURFACE_GUTS_H

#include "treeface/post/PostSurface.h"

#include <treecore/SortedSet.h>

namespace treeface
{

struct PostSurface::Guts
{
    PostTarget* m_target = nullptr;
    treecore::SortedSet<PostProcess> m_users;
};

} // namespace treeface

#endif // TREEFACE_POST_SURFACE_GUTS_H
