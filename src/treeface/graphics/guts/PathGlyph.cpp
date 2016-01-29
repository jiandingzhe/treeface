#include "treeface/graphics/guts/PathGlyph.h"

#include "treeface/math/Constants.h"
#include "treeface/math/Mat2.h"

namespace treeface
{

void PathGlyph::segment_arc( const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices ) const
{
    jassert( type == GLYPH_TYPE_ARC );
    jassert( result_vertices.size() == 0 );

    // roll to 0-360 degree
    float angle_use = arc.angle;
    while (angle_use < 0.0f) angle_use += 2.0f * PI;
    while (angle_use > 2.0f * PI) angle_use -= 2.0f * PI;

    // determine step
    int num_step = std::round( angle_use / PI * 32 );
    if (num_step < 5 && angle_use > 0.0f) num_step = 5;
    float step = angle_use / num_step;

    // rotate from end point
    Mat2f rot;
    if (arc.is_cclw) rot.set_rotate( step );
    else             rot.set_rotate( -step );

    // generate points
    Vec2f v = prev_end - Vec2f( arc.center_x, arc.center_y );

    for (int i = 1; i < num_step; i--)
    {
        v = rot * v;
        result_vertices.add( Vec2f( arc.center_x, arc.center_y ) + v );
    }

    result_vertices.add( end );
}

#define INTERPO( _a_, _frac_, _b_ ) _a_ * (1.0f - _frac_) + _b_ * _frac_

inline Vec2f _bessel3_interpo_( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, float frac )
{
    Vec2f tmp1 = INTERPO( p1, frac, p2 );
    Vec2f tmp2 = INTERPO( p2, frac, p3 );
    return INTERPO( tmp1, frac, tmp2 );
}

inline Vec2f _bessel4_interpo_( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, const Vec2f& p4, float frac )
{
    Vec2f tmp_a1 = INTERPO( p1, frac, p2 );
    Vec2f tmp_a2 = INTERPO( p2, frac, p3 );
    Vec2f tmp_a3 = INTERPO( p3, frac, p4 );

    Vec2f tmp_b1 = INTERPO( tmp_a1, frac, tmp_a2 );
    Vec2f tmp_b2 = INTERPO( tmp_a2, frac, tmp_a3 );

    Vec2f result = INTERPO( tmp_b1, frac, tmp_b2 );
    return result;
}

void PathGlyph::segment_bessel( const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices ) const
{
    jassert( type == GLYPH_TYPE_BESSEL4 );
    jassert( result_vertices.size() == 0 );

    float step = 1.0f / 32;
    Vec2f vtx_prev = prev_end;

    float frac_prev = 0.0f;
    float frac_curr = step;

    for (; frac_curr < 1.0f; )
    {
        Vec2f vtx_curr;

        for (;; )
        {
            float frac_delta = frac_curr - frac_prev;
            float frac_estimator = frac_prev + frac_delta / 2;

            Vec2f vtx_estimator =
                type == GLYPH_TYPE_BESSEL3
                    ? _bessel3_interpo_( prev_end, Vec2f( bessel3.ctrl_x, bessel3.ctrl_y ), end, frac_estimator )
                    : _bessel4_interpo_( prev_end, Vec2f( bessel4.ctrl1_x, bessel4.ctrl1_y ), Vec2f( bessel4.ctrl2_x, bessel4.ctrl2_y ), end, frac_estimator );
            vtx_curr =
                type == GLYPH_TYPE_BESSEL3
                    ? _bessel3_interpo_(prev_end, Vec2f(bessel3.ctrl_x, bessel3.ctrl_y), end, frac_curr)
                    : _bessel4_interpo_(prev_end, Vec2f(bessel4.ctrl1_x, bessel4.ctrl1_y), Vec2f(bessel4.ctrl2_x, bessel4.ctrl2_y), end, frac_curr);

            jassert( vtx_prev != vtx_estimator );
            jassert( vtx_estimator != vtx_curr );
            jassert( vtx_curr != vtx_prev );

            Vec2f v1 = vtx_curr - vtx_estimator;
            Vec2f v2 = vtx_estimator - vtx_prev;

            float cosine = v1 * v2 / std::sqrt( v1.length2() * v2.length2() );
            if (cosine > 0.6f) break;

            frac_curr = frac_prev + frac_delta * 0.67f;
        }

        result_vertices.add( vtx_curr );
        vtx_prev   = vtx_curr;
        frac_prev  = frac_curr;
        frac_curr += step;
    }

    jassert( result_vertices.size() > 0 );
    if (result_vertices.getLast() != end)
        result_vertices.add( end );
}

} // namespace treeface
