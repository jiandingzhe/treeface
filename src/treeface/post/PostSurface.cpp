#include "treeface/post/PostSurface.h"

#include "treeface/post/PostProcess.h"

#include "treeface/post/guts/PostSurface_guts.h"

namespace treeface
{

PostSurface::PostSurface(): m_guts( new Guts() ) {}

PostSurface::~PostSurface()
{
    treecore_assert( m_guts != nullptr );
    delete m_guts;
}

} // namespace treeface
