#ifndef TREEFACE_GEOMETRY_MANAGER_H
#define TREEFACE_GEOMETRY_MANAGER_H

#include "treeface/base/Common.h"

#include <treecore/Identifier.h>
#include <treecore/RefCountHolder.h>
#include <treecore/RefCountObject.h>

namespace treeface {

class Geometry;

class GeometryManager: public treecore::RefCountObject
{
public:
    GeometryManager();

    TREECORE_DECLARE_NON_COPYABLE( GeometryManager );
    TREECORE_DECLARE_NON_MOVABLE( GeometryManager );

    virtual ~GeometryManager();

    Geometry* get_geometry( const treecore::Identifier& name );

    bool geometry_is_cached( const treecore::Identifier& name ) const noexcept;
    bool release_geometry_hold( const treecore::Identifier& name );

private:
    struct Impl;

    Impl* m_impl;
};

} // namespace treeface

#endif // TREEFACE_GEOMETRY_MANAGER_H
