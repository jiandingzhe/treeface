#ifndef TYPED_TEMPLATE_WITH_OFFSET_H
#define TYPED_TEMPLATE_WITH_OFFSET_H

#include "treeface/misc/TypedTemplate.h"

namespace treeface
{

struct TypedTemplateWithOffset: public TypedTemplate
{
    TypedTemplateWithOffset( const TypedTemplate& attr, GLsizei offset, bool normalize )
        : TypedTemplate( attr )
        , offset( offset )
        , normalize( normalize )
    {}

    TypedTemplateWithOffset( const TypedTemplateWithOffset& peer ) = default;

    const GLsizei offset;
    const bool    normalize;
};

} // namespace treeface

#endif // TYPED_TEMPLATE_WITH_OFFSET_H
