#include "treeface/graphics/guts/SubPath.h"

#include "treeface/graphics/guts/LineStroker.h"

using namespace treecore;

namespace treeface
{

void SubPath::stroke_complex( treecore::Array<StrokeVertex>& result_vertices,
                              treecore::Array<IndexType>&    result_indices,
                              StrokeStyle                    style ) const
{
    jassert( glyphs[0].type == GLYPH_TYPE_LINE );

    LineStroker stroker( style );

    //
    // generate stroke outline
    //
    Vec2f v_begin;
    Vec2f v_prev;

    // glyph 0 is start vertex, so glyph 1 is the actual first glyph
    for (int i_glyph = 1; i_glyph < glyphs.size(); i_glyph++)
    {
        const PathGlyph& glyph_prev = glyphs[i_glyph - 1];
        const PathGlyph& glyph = glyphs[i_glyph];

        Array<Vec2f> curr_glyph_skeleton;
        glyph.segment( glyph_prev.end, curr_glyph_skeleton );

        jassert( curr_glyph_skeleton.size() );

        // first glyph, render cap
        if (i_glyph == 1)
        {
            v_begin = curr_glyph_skeleton.getFirst() - glyphs[0].end;
            v_begin.normalize();
            v_prev = v_begin;

            if (closed)
                stroker.close_stroke_begin( glyph_prev.end, v_begin );
            else
                stroker.cap_begin( glyph_prev.end, v_begin );
        }

        // render line segments of this glyph
        for (int i = 0; i < curr_glyph_skeleton.size(); i++)
        {
            if (i == 0)
                v_prev = stroker.extend_stroke( v_prev, glyph_prev.end, curr_glyph_skeleton[i], true );
            else
                v_prev = stroker.extend_stroke( v_prev, curr_glyph_skeleton[i - 1], curr_glyph_skeleton[i], false );
        }
    }

    if (closed)
    {
        const Vec2f& p_last  = glyphs.getLast().end;
        const Vec2f& p_first = glyphs.getFirst().end;

        if (p_last != p_first)
        {
            stroker.extend_stroke( v_prev, p_last, p_first, true );

            SUCK_GEOM_BLK( OutlineSucker sucker( stroker.part_left, "going to close stroke using actual end" );
                           sucker.draw_outline( stroker.part_right );
                           sucker.draw_vtx( p_last );
                           sucker.draw_vtx( p_first );
                           sucker.draw_unit_vector( p_first, v_begin );
            )
            stroker.close_stroke_end( p_last, p_first, v_begin );
        }
        else
        {
            Vec2f p_prev_calc = p_first - v_prev * (style.width * 2);
            SUCK_GEOM_BLK( OutlineSucker sucker( stroker.part_left, "end overlap start, going to close stroke using calculated end" );
                           sucker.draw_outline( stroker.part_right );
                           sucker.draw_vtx( p_prev_calc );
                           sucker.draw_vtx( p_first );
                           sucker.draw_unit_vector( p_first, v_begin );
            )
            stroker.close_stroke_end( p_prev_calc, p_first, v_begin );
        }
    }
    else
    {
        stroker.cap_end( glyphs.getLast().end, v_prev );
    }

    //
    // triangulate stroke outline, which provides final result
    //
    stroker.triangulate( result_vertices, result_indices, closed );
}

} // namespace treeface
