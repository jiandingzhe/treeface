#ifndef TREEFACE_VERTEX_ATTRIB_H
#define TREEFACE_VERTEX_ATTRIB_H

#include "treeface/common.h"

#include <treejuce/String.h>

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

struct VertexAttrib
{
    treejuce::String name;
    GLsizei n_elem;
    GLenum type;
};

struct HostVertexAttrib
{
    treejuce::String name;
    GLsizei n_elem;
    GLenum type;
    GLsizei offset;
    bool normalize;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VERTEX_ATTRIB_H
