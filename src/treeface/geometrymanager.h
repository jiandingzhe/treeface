#ifndef TREEFACE_GEOMETRY_MANAGER_H
#define TREEFACE_GEOMETRY_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Singleton.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class String;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class Geometry;

class GeometryManager
{
public:
    JUCE_DECLARE_NON_COPYABLE(GeometryManager);
    JUCE_DECLARE_NON_MOVABLE(GeometryManager);
    juce_DeclareSingleton(GeometryManager, false);

    treejuce::Result get_geometry(const treejuce::String& name, Geometry** result);
    bool has_geometry(const treejuce::String& name);
    bool release_geometry_hold(const treejuce::String& name);

private:
    struct Impl;

    GeometryManager();
    ~GeometryManager();

    Impl* m_impl;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GEOMETRY_MANAGER_H
