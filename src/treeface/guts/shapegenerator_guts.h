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

typedef enum
{
    GLYPH_TYPE_INVALID = -1,
    GLYPH_TYPE_LINE,
    GLYPH_TYPE_ARC,
    GLYPH_TYPE_BESSEL3,
    GLYPH_TYPE_BESSEL4,
} GlyphType;


struct PathGlyphArc
{
    Vec2f center;
    float angle;
    bool is_cclw;
};

struct PathGlyphBessel3
{
    Vec2f ctrl;
};

struct PathGlyphBessel4
{
    Vec2f ctrl1;
    Vec2f ctrl2;
};

struct PathGlyph
{
    explicit PathGlyph(const Vec2f& line_end)
        : type(GLYPH_TYPE_LINE)
        , end(line_end)
    {
    }

    PathGlyph(const Vec2f& center, const Vec2f& end, float angle, bool is_cclw)
        : type(GLYPH_TYPE_ARC)
        , end(end)
        , arc{center, angle, is_cclw}
    {
    }

    PathGlyph(const Vec2f& ctrl, const Vec2f& end)
        : type(GLYPH_TYPE_BESSEL3)
        , end(end)
        , bessel3{ctrl}
    {
    }

    PathGlyph(const Vec2f& ctrl1, const Vec2f& ctrl2, const Vec2f& end)
        : type(GLYPH_TYPE_BESSEL4)
        , bessel4{ctrl1, ctrl2}
    {
    }

    GlyphType type;
    Vec2f end;
    union
    {
        PathGlyphArc arc;
        PathGlyphBessel3 bessel3;
        PathGlyphBessel4 bessel4;
    };
};

typedef enum {
    VTX_ROLE_INVALID = -1,
    VTX_ROLE_START,
    VTX_ROLE_END,
    VTX_ROLE_LEFT,
    VTX_ROLE_RIGHT,
    VTX_ROLE_SPLIT,
    VTX_ROLE_MERGE
} VertexRole;

void partition_polygon_monotone(const treecore::Array<Vec2f>& vertices,
                                const treecore::Array<HalfEdge>& edges_input,
                                treecore::Array<HalfEdge>& edges_result);

inline bool is_convex(const Vec2f& edge1, const Vec2f& edge2) noexcept
{
    float cross = edge1 ^ edge2;
    return cross > 0;
}

inline bool is_convex(const Vec2f& vtx1, const Vec2f& vtx2, const Vec2f& vtx3) noexcept
{
    return is_convex(vtx2 - vtx1, vtx3 - vtx2);
}

double clockwise_accum(const treecore::Array<Vec2f>& vertices, IndexType i_begin, IndexType i_end) noexcept;

int segment_arc(treecore::Array<Vec2f>& result_vertices, const PathGlyph& glyph);

int segment_bessel3(treecore::Array<Vec2f>& result_vertices, const PathGlyph& glyph);

int segment_bessel4(treecore::Array<Vec2f>& result_vertices, const PathGlyph& glyph);

struct SubPath
{
    treecore::Array<PathGlyph> glyphs;
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
            jassert(glyphs.size() > 1);
            const PathGlyph& first_glyph = glyphs.getFirst();
            jassert(first_glyph.type == GLYPH_TYPE_LINE);
            glyphs.add(first_glyph);
            closed = false;
        }
    }

    // generates line strip directly, do not support join and cap shape at all
    void stroke_simple(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices) const;

    void stroke_complex(treecore::Array<Vec2f>& result_vertices,
                        treecore::Array<IndexType>& result_indices,
                        LineCapStyle cap_style,
                        LineJoinStyle join_style,
                        float line_width) const;
};

struct ShapeGenerator::Guts
{
    treecore::Array<SubPath> subpaths;

    void triangulate(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices);
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
