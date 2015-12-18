#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/shapegenerator.h"

#include "treeface/gl/type.h"
#include "treeface/graphics/halfedge.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/vec3.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{

void partition_polygon_monotone(const treecore::Array<Vec2f>& vertices,
                                const treecore::Array<HalfEdge>& edges_input,
                                treecore::Array<HalfEdge>& edges_result,
                                treecore::Array<IndexType>& edge_idx_by_y);

inline bool is_convex(const Vec2f& edge1, const Vec2f& edge2) noexcept
{
    float cross = edge1 ^ edge2;
    return cross > 0;
}

inline bool is_convex(const Vec2f& vtx1, const Vec2f& vtx2, const Vec2f& vtx3) noexcept
{
    return is_convex(vtx2 - vtx1, vtx3 - vtx2);
}

bool is_counter_clockwise(const treecore::Array<Vec2f>& vertices) noexcept;

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

    // generates line strip directly, do not support join and cap shape at all
    void stroke_simple() const;

    void stroke_complex() const;
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
