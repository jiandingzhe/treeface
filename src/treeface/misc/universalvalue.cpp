#include "treeface/misc/universalvalue.h"

#include "treeface/misc/stringcast.h"
#include <treecore/MPL.h>

using namespace treecore;

static_assert( mpl_same_type<GLboolean, GLubyte>::value, "GLboolean is GLubyte" );

namespace treeface
{} // namespace treeface
