#ifndef TREEFACE_GEOMETRY_MANAGER_H
#define TREEFACE_GEOMETRY_MANAGER_H

#include "treeface/common.h"

#include <treecore/Holder.h>
#include <treecore/Object.h>
#include <treecore/String.h>
#include <treecore/Result.h>


namespace treeface {

class Geometry;

class GeometryManager: public treecore::Object
{
public:
    GeometryManager();

    JUCE_DECLARE_NON_COPYABLE(GeometryManager);
    JUCE_DECLARE_NON_MOVABLE(GeometryManager);

    virtual ~GeometryManager();

    treecore::Result get_geometry(const treecore::String& name, treecore::Holder<Geometry>& result);

    bool geometry_is_cached(const treecore::String& name) const NOEXCEPT;
    bool release_geometry_hold(const treecore::String& name);

private:
    struct Impl;

    Impl* m_impl;
};

} // namespace treeface

#endif // TREEFACE_GEOMETRY_MANAGER_H
