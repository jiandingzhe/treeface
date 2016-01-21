#ifndef TREEFACE_VERTEX_ATTRIB_H
#define TREEFACE_VERTEX_ATTRIB_H

#include "treeface/common.h"

#include "treeface/gl/type.h"
#include "treeface/gl/enums.h"

#include <treecore/Identifier.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

struct VertexAttrib
{
    VertexAttrib( const treecore::Identifier& name, treecore::int32 n_elem, GLType type )
        : name( name )
        , n_elem( n_elem )
        , type( type )
    {}

    VertexAttrib( const VertexAttrib& peer ) = default;

    virtual ~VertexAttrib() = default;

    inline size_t get_elem_offset( treecore::int32 index ) const noexcept
    {
        jassert( 0 <= index && index < n_elem );
        return index * size_of_gl_type( type );
    }

    inline size_t size() const noexcept
    {
        return n_elem * size_of_gl_type( type );
    }

    const treecore::Identifier name;
    const treecore::int32  n_elem;
    const GLType type;
};

struct HostVertexAttrib: VertexAttrib
{
    HostVertexAttrib( const VertexAttrib& attr, GLsizei offset, bool normalize )
        : VertexAttrib( attr )
        , offset( offset )
        , normalize( normalize )
    {}

    HostVertexAttrib( const HostVertexAttrib& peer ) = default;

    virtual ~HostVertexAttrib() = default;

    const GLsizei offset;
    const bool    normalize;
};

} // namespace treeface

#endif // TREEFACE_VERTEX_ATTRIB_H
