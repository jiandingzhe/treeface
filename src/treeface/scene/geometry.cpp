#include "treeface/scene/geometry.h"

#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexattrib.h"
#include "treeface/gl/vertexindexbuffer.h"

#include <treejuce/Array.h>
#include <treejuce/DynamicObject.h>
#include <treejuce/HashSet.h>
#include <treejuce/Holder.h>
#include <treejuce/NamedValueSet.h>
#include <treejuce/Result.h>
#include <treejuce/Variant.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct Geometry::Impl
{
    Array<HostVertexAttrib> attrs;
    VertexIndexBuffer buffer;
};

Geometry::Geometry(): m_impl(new Impl())
{
}

Geometry::~Geometry()
{
    if (m_impl)
        delete m_impl;
}

treejuce::Result _validate_root_kv_(const NamedValueSet& kv)
{
    if (!kv.contains("attributes"))
        return Result::fail("geometry node don't contain property \"attributes\"");
    if (!kv.contains("vertex"))
        return Result::fail("geometry node don't contain property \"vertex\"");
    if (!kv.contains("index"))
        return Result::fail("geometry node don't contain property \"index\"");

    HashSet<String> wanted_keys;
    wanted_keys.insert("attributes");
    wanted_keys.insert("vertex");
    wanted_keys.insert("index");

    for (int i = 0; i < kv.size(); i++)
    {
        String key = kv.getName(i).toString();
        if (!wanted_keys.contains(key))
            return Result::fail("geometry node contains unidentified property \""+key+"\"");
    }

    return Result::ok();
}

treejuce::Result Geometry::build(const treejuce::var& geom_root_node) NOEXCEPT
{
    if (!geom_root_node.isObject())
        return Result::fail("geometry root node is not a object");

    const NamedValueSet& geom_root_kv = geom_root_node.getDynamicObject()->getProperties();
    {
        Result re = _validate_root_kv_(geom_root_kv);
        if (!re)
            return re;
    }

    const var& node_attrib_list = geom_root_kv["attributes"];
    if (!node_attrib_list.isArray())
        return Result::fail("geometry attributes node is not array");

    const Array<var>* attrib_list = node_attrib_list.getArray();
    for (int i_attrib = 0; i_attrib < attrib_list->size(); i_attrib++)
    {
        const var& node_attrib = attrib_list->getReference(i_attrib);
        // TODO
    }

    // TODO

    return Result::ok();
}

treejuce::MemoryBlock& Geometry::get_vertex_store() NOEXCEPT
{
    // TODO
}

treejuce::Array<treejuce::uint16>& Geometry::get_index_store() NOEXCEPT
{
    // TODO
}

TREEFACE_NAMESPACE_END
