#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/shapegenerator.h"

#include "treeface/gl/type.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/vec3.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{

struct SubPath
{
    treecore::Array<Vec2f> vertices;
    bool closed = false;

    ///
    /// If a subpath is closed, it assumes the last stroke glyph is from last
    /// point to first point. So if we need to continue to add glyph to it, one
    /// additional point need to be added using the first point.
    ///
    void try_reopen_closed_path()
    {
        if (closed)
        {
            jassert(vertices.size() > 1);
            vertices.add(vertices.getFirst());
            closed = false;
        }
    }

    void triangulate_simple(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices) const;

    void triangulate_complex(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices) const;
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
