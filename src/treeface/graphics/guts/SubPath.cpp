#include "treeface/graphics/guts/SubPath.h"

#include "treeface/graphics/guts/LineStroker.h"

using namespace treecore;

namespace treeface
{

void SubPath::stroke_complex(treecore::Array<StrokeVertex>& result_vertices,
                             treecore::Array<IndexType>& result_indices,
                             StrokeStyle style) const
{
    jassert(glyphs[0].type == GLYPH_TYPE_LINE);

    LineStroker stroker(style);

    //
    // generate stroke outline
    //
    Array<Vec2f> skeleton;
    Vec2f v_begin;
    Vec2f v_prev;

    // glyph 0 is start vertex, so glyph 1 is the actual first glyph
    for (int i_glyph = 1; i_glyph < glyphs.size(); i_glyph++)
    {
        const PathGlyph& glyph_prev = glyphs[i_glyph - 1];
        const PathGlyph& glyph = glyphs[i_glyph];
        glyph.segment(glyph_prev.end, skeleton);

        // first
        if (i_glyph == 1)
        {
            v_begin = skeleton.getFirst() - glyphs[0].end;
            v_begin.normalize();
            v_prev = v_begin;
            if (!closed)
                stroker.cap_begin(glyph_prev.end, v_begin);
        }

        for (int i = 0; i < skeleton.size(); i++)
        {
            if (i == 0)
                v_prev = stroker.extend_stroke(v_prev, glyph_prev.end, skeleton[i], true);
            else
                v_prev = stroker.extend_stroke(v_prev, skeleton[i-1], skeleton[i], false);
        }
    }

    if (closed)
        stroker.close_stroke(v_prev, glyphs.getLast().end, v_begin);
    else
        stroker.cap_end(glyphs.getLast().end, v_prev);

    //
    // triangulate stroke outline, which provides final result
    //
    stroker.triangulate(result_vertices, result_indices, closed);
}

} // namespace treeface
