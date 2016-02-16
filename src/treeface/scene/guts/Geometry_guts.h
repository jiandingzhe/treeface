#ifndef TREEFACE_GEOMETRY_GUTS_H
#define TREEFACE_GEOMETRY_GUTS_H

#include "treeface/gl/GLBuffer.h"
#include "treeface/gl/VertexTemplate.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/guts/Utils.h"
#include "treeface/scene/guts/VisualObject_guts.h"

namespace treeface
{

struct Geometry::Guts
{
    Guts( const VertexTemplate& vtx_temp, GLPrimitive primitive, bool is_dynamic );
    ~Guts();

    void upload_data();

    void invalidate_user_uniform_cache();

    VisualObject::Impl* user_head = nullptr;
    VisualObject::Impl* user_tail = nullptr;

    const bool dynamic;
    bool       drawing = false;
    bool       dirty   = false;
    int32      num_idx = 0;
    const GLPrimitive primitive;

    const VertexTemplate vtx_temp;

    treecore::RefCountHolder<GLBuffer> buf_vtx;
    treecore::RefCountHolder<GLBuffer> buf_idx;

    UniformMap uniforms;

    Geometry::HostVertexCache  host_data_vtx;
    treecore::Array<IndexType> host_data_idx;
};

} // namespace treeface

#endif // TREEFACE_GEOMETRY_GUTS_H
