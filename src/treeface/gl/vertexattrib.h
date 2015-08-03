#ifndef TREEFACE_VERTEX_ATTRIB_H
#define TREEFACE_VERTEX_ATTRIB_H

#include "treeface/common.h"

#include "treeface/gl/type.h"

#include <treecore/String.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

struct VertexAttrib
{
    VertexAttrib(const treecore::String& name, treecore::int32 n_elem, GLenum type)
        : name(name)
        , n_elem(n_elem)
        , type(type)
    {
    }

    inline size_t get_elem_offset(treecore::int32 index) const NOEXCEPT
    {
        jassert(index < n_elem);
        return index * size_of_gl_type(type);
    }

    inline size_t size() const NOEXCEPT
    {
        return n_elem * size_of_gl_type(type);
    }

    treecore::String name;
    treecore::int32 n_elem;
    GLenum type;
};

struct HostVertexAttrib: VertexAttrib
{
    HostVertexAttrib(const VertexAttrib& attr, GLsizei offset, bool normalize)
        : VertexAttrib(attr)
        , offset(offset)
        , normalize(normalize)
    {
    }

    HostVertexAttrib(const treecore::String& name, GLsizei n_elem, GLenum type, GLsizei offset, bool normalize)
        : VertexAttrib(name, n_elem, type)
        , offset(offset)
        , normalize(normalize)
    {
    }

    GLsizei offset;
    bool normalize;
};

} // namespace treeface

#endif // TREEFACE_VERTEX_ATTRIB_H
