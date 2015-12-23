#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/shapegenerator.h"
#include "treeface/graphics/guts/enums.h"

#include "treeface/gl/type.h"
#include "treeface/graphics/halfedge.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/vec3.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{



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
