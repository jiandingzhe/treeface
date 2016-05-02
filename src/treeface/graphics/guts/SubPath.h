#ifndef TREEFACE_SUB_PATH_H
#define TREEFACE_SUB_PATH_H

#include "treeface/gl/TypeUtils.h"
#include "treeface/graphics/guts/PathGlyph.h"
#include "treeface/graphics/Utils.h"
#include "treeface/graphics/BBox2.h"
#include "treeface/scene/Geometry.h"

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
            treecore_assert( glyphs.size() > 1 );
            const PathGlyph& first_glyph = glyphs.getFirst();
            treecore_assert( first_glyph.type == GLYPH_TYPE_LINE );
            glyphs.add( first_glyph );
            closed = false;
        }
    }

    // generates line strip directly, do not support join and cap shape at all
    void stroke_simple( Geometry::HostVertexCache&  result_vertices,
                        Vec2f& result_skeleton_min,
                        Vec2f& result_skeleton_max,
                        treecore::Array<IndexType>& result_indices ) const;

    void stroke_complex( Geometry::HostVertexCache&  result_vertices,
                         treecore::Array<IndexType>& result_indices,
                         Vec2f& result_skeleton_min,
                         Vec2f& result_skeleton_max,
                         const StrokeStyle& style ) const;
};

} // namespace treeface

#endif // TREEFACE_SUB_PATH_H
