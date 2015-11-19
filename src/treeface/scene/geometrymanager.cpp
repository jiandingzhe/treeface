#include "treeface/scene/geometrymanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"

#include <treecore/HashMap.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/Result.h>
#include <treecore/ScopedPointer.h>
#include <treecore/String.h>

using namespace treecore;

namespace treeface {

struct GeometryManager::Impl
{
    HashMap<String, RefCountHolder<Geometry> > items;
};

GeometryManager::GeometryManager(): m_impl(new Impl())
{
}

GeometryManager::~GeometryManager()
{
    if (m_impl)
        delete m_impl;
}

treecore::Result GeometryManager::get_geometry(const treecore::String& name, RefCountHolder<Geometry>& result)
{
    if (m_impl->items.contains(name))
    {
        result = m_impl->items[name];
        return Result::ok();
    }

    // get raw data from package manager
    MemoryBlock data;
    Result item_re = PackageManager::getInstance()->get_item_data(name, data);

    if (!item_re)
    {
        result = nullptr;
        return Result::fail("failed to get geometry data:\n" +
                            item_re.getErrorMessage());
    }

    // parse JSON
    var geom_root_node;
    Result json_re = JSON::parse((const char*)data.getData(), geom_root_node);

    if (!json_re)
    {
        result = nullptr;
        return Result::fail(String("failed to parse geometry JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            String((const char*)data.getData())
                            );
    }

    // create geometry object
    result = new Geometry();
    Result geom_re = result->build(geom_root_node);

    if (!geom_re)
    {
        result = nullptr;
        return Result::fail(String("failed to build geometry using JSON content for \"") + name + String("\":\n") +
                            geom_re.getErrorMessage() + String("\n") +
                            String((const char*)data.getData())
                            );
    }

    m_impl->items.set(name, result);
    return Result::ok();
}

bool GeometryManager::geometry_is_cached(const treecore::String& name) const noexcept
{
    return m_impl->items.contains(name);
}

bool GeometryManager::release_geometry_hold(const treecore::String& name)
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

} // namespace treeface
