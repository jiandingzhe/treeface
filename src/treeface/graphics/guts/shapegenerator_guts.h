#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/shapegenerator.h"
#include "treeface/graphics/guts/enums.h"

#include "treeface/gl/type.h"
#include "treeface/graphics/halfedge.h"
#include "treeface/graphics/guts/PathGlyph.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/vec3.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{

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
    void stroke_simple(treecore::Array<ShapeGenerator::StrokeVertex>& result_vertices, treecore::Array<IndexType>& result_indices) const;

    void stroke_complex(treecore::Array<ShapeGenerator::StrokeVertex>& result_vertices,
                        treecore::Array<IndexType>& result_indices,
                        ShapeGenerator::StrokeStyle style) const;
};

struct ShapeGenerator::Guts
{
    treecore::Array<SubPath> subpaths;

    void triangulate(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices);
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
