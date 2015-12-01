#include "treeface/scene/geometry.h"

#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexattrib.h"
#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertextemplate.h"

#include "treeface/misc/propertyvalidator.h"

#include "treeface/misc/stringcast.h"

#include <treecore/Array.h>
#include <treecore/CriticalSection.h>
#include <treecore/DynamicObject.h>
#include <treecore/HashSet.h>
#include <treecore/NamedValueSet.h>
#include <treecore/RefCountHolder.h>
#include <treecore/RefCountSingleton.h>
#include <treecore/Result.h>
#include <treecore/ScopedLock.h>
#include <treecore/Singleton.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

struct Geometry::Impl
{
    Array<HostVertexAttrib> attrs;
    VertexTemplate vtx_template;
    RefCountHolder<VertexIndexBuffer> buffer;
    GLenum primitive;
};

Geometry::Geometry(): m_impl(new Impl())
{
    m_impl->buffer = new VertexIndexBuffer;
}

Geometry::~Geometry()
{
    if (m_impl)
        delete m_impl;
}

#define KEY_ATTR "attributes"
#define KEY_VTX  "vertices"
#define KEY_IDX  "indices"
#define KEY_PRIM "primitive"

class GeometryPropertyValidator: public PropertyValidator, public treecore::RefCountSingleton<GeometryPropertyValidator>
{
public:
    GeometryPropertyValidator()
    {
        add_item(KEY_ATTR, PropertyValidator::ITEM_ARRAY, true);
        add_item(KEY_VTX, PropertyValidator::ITEM_ARRAY, true);
        add_item(KEY_IDX, PropertyValidator::ITEM_ARRAY, true);
        add_item(KEY_PRIM, PropertyValidator::ITEM_SCALAR, true);
    }
};

treecore::Result Geometry::build(const treecore::var& geom_root_node) noexcept
{
    if (!geom_root_node.isObject())
        return Result::fail("geometry root node is not a object");

    const NamedValueSet& geom_root_kv = geom_root_node.getDynamicObject()->getProperties();
    {
        Result re = GeometryPropertyValidator::getInstance()->validate(geom_root_kv);
        if (!re)
            return Result::fail("geometry JSON node is invalid: " + re.getErrorMessage());
    }

    //
    // load primitive type (triangle list / fan / ....)
    //
    m_impl->primitive = gl_primitive_from_string(geom_root_kv[KEY_PRIM].toString());

    //
    // load vertex attribute template
    //
    const var& attrib_list_node = geom_root_kv[KEY_ATTR];
    m_impl->vtx_template.add_attribs(attrib_list_node);

    size_t vtx_size = m_impl->vtx_template.vertex_size();
    int n_vtx_elem  = m_impl->vtx_template.n_elems();
    DBG("geometry vertex size: "+String(vtx_size) + ", elems: "+String(n_vtx_elem));

    //
    // load vertices
    //
    const Array<var>* vtx_nodes = geom_root_node[KEY_VTX].getArray();

    m_impl->buffer->m_data_vtx.setSize(vtx_size * vtx_nodes->size(), false);

    GLbyte* vtx_data = (GLbyte*) m_impl->buffer->m_data_vtx.getData();

    for (int i_vtx = 0; i_vtx < vtx_nodes->size(); i_vtx++)
    {
        // get and validate vertex node
        const var& vtx_node = (*vtx_nodes)[i_vtx];

        if (!vtx_node.isArray())
            return Result::fail("vertex node at "+String(i_vtx)+" is not an array");

        const Array<var>* vtx_elems = vtx_node.getArray();

        if (vtx_elems->size() != n_vtx_elem)
            return Result::fail("vertex template specified "+String(n_vtx_elem)+" elements, but vertex node "+String(i_vtx)+" has only "+String(vtx_elems->size())+" elements");

        // fill data
        for (int i_elem = 0; i_elem < n_vtx_elem; i_elem++)
        {
            m_impl->vtx_template.set_value_at(vtx_data, i_elem, (*vtx_elems)[i_elem]);
        }

        // move forward pointer of current vertex data
        vtx_data += vtx_size;
    }

    //
    // build vertex indices
    //
    const Array<var>* idx_nodes = geom_root_node[KEY_IDX].getArray();

    for (int i_idx = 0; i_idx < idx_nodes->size(); i_idx++)
    {
        int idx = int((*idx_nodes)[i_idx]);
        if (idx >= vtx_nodes->size())
            return Result::fail("vertex amount is "+String(vtx_nodes->size())+", but got index "+String(idx));
        m_impl->buffer->m_data_idx.add(idx);
    }

    // mark data change
    m_impl->buffer->m_data_changed = true;

    return Result::ok();
}

bool Geometry::is_dirty() const noexcept
{
    return m_impl->buffer->m_data_changed;
}

void Geometry::mark_dirty() noexcept
{
    m_impl->buffer->m_data_changed = true;
}

GLenum Geometry::get_primitive() const noexcept
{
    return m_impl->primitive;
}

void Geometry::set_primitive(GLenum value) noexcept
{
    m_impl->primitive = value;
}

treeface::VertexIndexBuffer* Geometry::get_buffer() noexcept
{
    return m_impl->buffer.get();
}

const VertexTemplate& Geometry::get_vertex_template() const noexcept
{
    return m_impl->vtx_template;
}

} // namespace treeface
