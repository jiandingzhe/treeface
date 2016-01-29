#ifndef TREEFACE_COMMON_H
#define TREEFACE_COMMON_H

#include <treecore/Common.h>
#include <treecore/IntTypes.h>

#define STRINGIFY( content ) #content
#define EXPAND_AND_STRINGIFY( input ) STRINGIFY( input )

namespace treeface
{

using treecore::size_t;

using treecore::int8;
using treecore::uint8;
using treecore::int16;
using treecore::uint16;
using treecore::int32;
using treecore::uint32;
using treecore::int64;
using treecore::uint64;

using treecore::pointer_sized_int;
using treecore::pointer_sized_uint;

using treecore::ssize_t;

void die( const char* format, ... );
void warn( const char* format, ... );

///
/// \brief type of vertex index
///
/// 16 bit seems enough for us
///
typedef treecore::uint16 IndexType;

} // namespace treeface

#endif // TREEFACE_COMMON_H
