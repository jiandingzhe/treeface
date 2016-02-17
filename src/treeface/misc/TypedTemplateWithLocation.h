#ifndef TREEFACE_TYPED_TEMPLATE_WITH_LOCATION_H
#define TREEFACE_TYPED_TEMPLATE_WITH_LOCATION_H

#include "treeface/misc/TypedTemplate.h"

namespace treeface
{

struct TypedTemplateWithLocation: public TypedTemplate
{
    TypedTemplateWithLocation( const treecore::Identifier& name, treecore::int32 n_elem, GLType type, GLint location )
        : TypedTemplate( name, n_elem, type )
        , location( location )
    {}
    GLint location;
};

} // namespace treeface

#endif // TREEFACE_TYPED_TEMPLATE_WITH_LOCATION_H
