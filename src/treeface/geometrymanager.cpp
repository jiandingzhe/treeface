#include "treeface/geometrymanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/Result.h>
#include <treejuce/ScopedPointer.h>
#include <treejuce/String.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
template <>
struct ContainerDeletePolicy<uint8>
{
    static void destroy (uint8* ptr)
    {
        free(ptr);
    }
};
TREEFACE_JUCE_NAMESPACE_END

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

juce_ImplementSingleton(GeometryManager);

struct GeometryManager::Impl
{
    HashMap<String, Holder<Geometry> > items;
};

GeometryManager::GeometryManager(): m_impl(new Impl())
{
}

GeometryManager::~GeometryManager()
{
    if (m_impl)
        delete m_impl;
}

treejuce::Result GeometryManager::get_geometry(const treejuce::String& name, Geometry** result)
{
    if (m_impl->items.contains(name))
    {
        *result = m_impl->items[name].get();
        return Result::ok();
    }

    // get raw data from package manager
    ArrayRef<uint8> data = PackageManager::getInstance()->get_item_data(name);
    ScopedPointer<uint8> data_holder(data.get_data());

    if (!data.get_data())
    {
        *result = nullptr;
        return Result::fail("failed to get geometry data from package manager using name \""+name+"\"");
    }

    // parse JSON
    var geom_root_node;
    Result json_re = JSON::parse((const char*)data.get_data(), geom_root_node);

    if (!json_re)
    {
        *result = nullptr;
        return Result::fail(String("failed to parse geometry JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            String((const char*)data.get_data())
                            );
    }

    // create geometry object
    Holder<Geometry> geom = new Geometry();
    Result geom_re = geom->build(geom_root_node);

    if (!geom_re)
    {
        *result = nullptr;
        return Result::fail(String("failed to build geometry using JSON content for \"") + name + String("\":\n") +
                            geom_re.getErrorMessage() + String("\n") +
                            String((const char*)data.get_data())
                            );
    }

    *result = geom.get();
    m_impl->items.set(name, geom);
    return Result::ok();
}

bool GeometryManager::has_geometry(const treejuce::String& name)
{
    return m_impl->items.contains(name);
}

bool GeometryManager::release_geometry_hold(const treejuce::String& name)
{
    if (m_impl->items.contains(name))
    {
        m_impl->items.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

TREEFACE_NAMESPACE_END
