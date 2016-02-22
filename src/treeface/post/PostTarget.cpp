#include "treeface/post/PostTarget.h"

#include <treecore/HashMap.h>

#include "treeface/gl/Enums.h"

#include "treeface/post/guts/PostTarget_guts.h"

using namespace treecore;

namespace treeface
{

PostTarget::PostTarget(): m_guts(new Guts())
{
}

} // namespace treeface
