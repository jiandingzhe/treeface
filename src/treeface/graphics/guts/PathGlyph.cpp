#include "treeface/graphics/guts/PathGlyph.h"

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"

namespace treeface
{

void PathGlyph::segment_arc(const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices) const
{
    jassert(type == GLYPH_TYPE_ARC);
    jassert(result_vertices.size() == 0);

    // roll to 0-360 degree
    float angle_use = arc.angle;
    while (angle_use < 0.0f) angle_use += 2.0f * PI;
    while (angle_use > 2.0f * PI) angle_use -= 2.0f * PI;

    // determine step
    int num_step = std::round(angle_use / PI * 32);
    if (num_step < 5 && angle_use > 0.0f) num_step = 5;
    float step = angle_use / num_step;

    // rotate from end point
    Mat2f rot;
    if (arc.is_cclw) rot.set_rotate(step);
    else             rot.set_rotate(-step);

    // generate points
    Vec2f v = prev_end - arc.center;

    for (int i = 1; i < num_step; i--)
    {
        v = rot * v;
        result_vertices.add(arc.center + v);
    }

    result_vertices.add(end);
}

#define INTERPO(_a_, _frac_, _b_) _a_ * _frac_ + _b_ * (1.0f - _frac_)

inline Vec2f _bessel3_interpo_(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, float frac)
{
    Vec2f tmp1 = INTERPO(p1, frac, p2);
    Vec2f tmp2 = INTERPO(p2, frac, p3);
    return INTERPO(tmp1, frac, tmp2);
}

inline Vec2f _bessel4_interpo_(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, const Vec2f& p4, float frac)
{
    Vec2f tmp_a1 = INTERPO(p1, frac, p2);
    Vec2f tmp_a2 = INTERPO(p2, frac, p3);
    Vec2f tmp_a3 = INTERPO(p3, frac, p4);

    Vec2f tmp_b1 = INTERPO(tmp_a1, frac, tmp_a2);
    Vec2f tmp_b2 = INTERPO(tmp_a2, frac, tmp_a3);

    return INTERPO(tmp_b1, frac, tmp_b2);
}

void PathGlyph::segment_bessel3(const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices) const
{
    jassert(type == GLYPH_TYPE_ARC);
    jassert(result_vertices.size() == 0);

    float step = 1.0f / 32;

    for (float frac = 0.0f; frac < 1.0f;)
    {
        const Vec2f& vtx_prev = result_vertices.getLast();
        float step_use = step;

        for (;;)
        {
            Vec2f vtx_estimator = _bessel3_interpo_(prev_end, bessel3.ctrl, end, frac + step_use / 2);
            Vec2f vtx_curr      = _bessel3_interpo_(prev_end, bessel3.ctrl, end, frac + step_use);
            jassert(vtx_prev != vtx_estimator);
            jassert(vtx_estimator != vtx_curr);
            jassert(vtx_curr != vtx_prev);

            float cosine = (vtx_curr - vtx_estimator) * (vtx_estimator - vtx_prev);
            if (cosine > 0.6f)
            {
                result_vertices.add(vtx_curr);
                break;
            }

            step_use *= 0.67f;
        }
    }

    result_vertices.add(end);
}

void PathGlyph::segment_bessel4(const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices) const
{
    jassert(type == GLYPH_TYPE_ARC);
    jassert(result_vertices.size() == 0);

    float step = 1.0f / 32;

    for (float frac = 0.0f; frac < 1.0f;)
    {
        const Vec2f& vtx_prev = result_vertices.getLast();
        float step_use = step;

        for (;;)
        {
            Vec2f vtx_estimator = _bessel4_interpo_(prev_end, bessel4.ctrl1, bessel4.ctrl2, end, frac + step_use / 2);
            Vec2f vtx_curr      = _bessel4_interpo_(prev_end, bessel4.ctrl1, bessel4.ctrl2, end, frac + step_use);
            jassert(vtx_prev != vtx_estimator);
            jassert(vtx_estimator != vtx_curr);
            jassert(vtx_curr != vtx_prev);

            float cosine = (vtx_curr - vtx_estimator) * (vtx_estimator - vtx_prev);
            if (cosine > 0.6f)
            {
                result_vertices.add(vtx_curr);
                break;
            }

            step_use *= 0.67f;
        }
    }

    result_vertices.add(end);
}

} // namespace treeface
