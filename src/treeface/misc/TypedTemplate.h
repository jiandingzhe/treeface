#ifndef TREEFACE_TYPED_TEMPLATE_H
#define TREEFACE_TYPED_TEMPLATE_H

#include "treeface/base/Common.h"

#include "treeface/gl/TypeUtils.h"
#include "treeface/gl/Enums.h"

#include <treecore/Identifier.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface
{

struct TypedTemplate
{
    TypedTemplate( const treecore::Identifier& name, treecore::int32 n_elem, GLType type )
        : name( name )
        , n_elem( n_elem )
        , type( type )
    {}

    TypedTemplate( const TypedTemplate& peer ) = default;

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


} // namespace treeface

#endif // TREEFACE_TYPED_TEMPLATE_H
