#include "treeface/scene/geometrymanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/MemoryBlock.h>
#include <treejuce/Result.h>
#include <treejuce/ScopedPointer.h>
#include <treejuce/String.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

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
    MemoryBlock data;
    Result item_re = PackageManager::getInstance()->get_item_data(name, data);

    if (!item_re)
    {
        *result = nullptr;
        return Result::fail("failed to get geometry data:\n" +
                            item_re.getErrorMessage());
    }

    // parse JSON
    var geom_root_node;
    Result json_re = JSON::parse((const char*)data.getData(), geom_root_node);

    if (!json_re)
    {
        *result = nullptr;
        return Result::fail(String("failed to parse geometry JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            String((const char*)data.getData())
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
                            String((const char*)data.getData())
                            );
    }

    *result = geom.get();
    m_impl->items.set(name, geom);
    return Result::ok();
}

treejuce::Result GeometryManager::get_geometry(const treejuce::String& name, treejuce::Holder<Geometry>& result)
{
    Geometry* geom = nullptr;
    Result re = get_geometry(name, &geom);
    result = geom;
    return re;
}

bool GeometryManager::has_geometry(const treejuce::String& name) const NOEXCEPT
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
