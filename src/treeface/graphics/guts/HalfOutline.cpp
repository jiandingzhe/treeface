#include "treeface/graphics/guts/HalfOutline.h"

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"

namespace treeface
{


int HalfOutline::find_cross_from_tail(const Vec2f& p1, const Vec2f& p2, Vec2f& p_cross, int step_limit) const
{
    jassert(outline.size() > 1);
    jassert(outline.size() == outline_bounds.size() + 1);

    Vec2f v12 = p2 - p1;

    const BBox2f bound_input(p1, p2);

    for (int i = outline.size() - 2; i >= 0; i--)
    {
        if (outline.size() - i > step_limit) break;

        if (bound_input ^ outline_bounds[i])
        {
            const Vec2f& p3 = outline[i];
            const Vec2f& p4 = outline[i+1];
            Vec2f v34 = p4 - p3;

            if (cross_test_exc(p1, p2, v12, p3, p4, v34, p_cross))
                return i;
        }
    }

    return -1;
}

void HalfOutline::add_miter_point(const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style)
{
    jassert(std::abs(ortho_prev.length2() - 1.0f) < 0.0001f);
    jassert(std::abs(ortho_curr.length2() - 1.0f) < 0.0001f);

    // only do miter join if angle is not too sharp
    float turn_cosine = ortho_prev * ortho_curr;
    if (turn_cosine > style.miter_cutoff_cosine)
    {
        Vec2f ortho_mid = ortho_prev + ortho_curr;
        ortho_mid.normalize();

        float half_cosine = ortho_mid * ortho_prev;
        jassert(0.0f < half_cosine && half_cosine < 1.0f);
        Vec2f r_mid = ortho_mid * (style.half_width * side / half_cosine);

        add(skeleton1 + r_mid, id);
    }
}

void HalfOutline::add_round_points(const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style)
{
    jassert(std::abs(ortho_prev.length2() - 1.0f) < 0.0001f);
    jassert(std::abs(ortho_curr.length2() - 1.0f) < 0.0001f);

    // calculate step
    float turn_angle = std::acos(ortho_prev * ortho_curr);
    int num_step = turn_angle / PI * 32;
    if (num_step < 5) num_step = 5;
    float step_angle = turn_angle / num_step;

    Mat2f step_mat;
    step_mat.set_rotate(-side * step_angle);

    // do rotation
    Vec2f r_tmp = ortho_prev * side * style.half_width;
    for (int i = 1; i < num_step; i++)
    {
        r_tmp = step_mat * r_tmp;
        add(skeleton1 + r_tmp, id);
    }
}

void HalfOutline::process_inner(const HalfOutline& outer_peer,
                                             const Vec2f& skeleton1,
                                             const JointID id1,
                                             const Vec2f& skeleton2,
                                             const Vec2f& ortho_curr,
                                             const InternalStrokeStyle& style)
{
    const JointID id2 = id1 + 1;
    Vec2f p_cross;

    Vec2f r_curr = ortho_curr * (side * style.half_width);
    Vec2f p1 = skeleton1 + r_curr;
    Vec2f p2 = skeleton2 + r_curr;

    {
        int i_self = find_cross_from_tail(p1, p2, p_cross, TAIL_FIND_LIMIT);
        if (i_self >= 0)
        {
            resize(i_self+1);
            add(p_cross, id1);
            add(p2,      id2);
            sunken = false;
            return;
        }
    }

    {
        int i_peer = outer_peer.find_cross_from_tail(p1, p2, p_cross, TAIL_FIND_LIMIT);
        if (i_peer >= 0)
        {
            add(p_cross, id1);
            add(p2, id2);
            sunken = false;
            return;
        }
    }

    sunken = true;
}

void HalfOutline::close_inner()
{
    IndexType id_last = joint_ids.getLast();

    for (int i = 0; i < TAIL_FIND_LIMIT && i < outline.size() - 1; i++)
    {
        Vec2f cross;
        int i_tail = find_cross_from_tail(outline[i], outline[i+1], cross, TAIL_FIND_LIMIT);

        if (i_tail >= 0 && i_tail > i)
        {
            // modify tail
            resize(i_tail + 1);
            add(cross, id_last);

            // modify head
            outline.removeRange(0, i);
            outline_bounds.removeRange(0, i);
            joint_ids.removeRange(0, i);

            outline[0] = cross;
            outline_bounds[0] = BBox2f(outline[0], outline[1]);
            joint_ids[0] = 0;

            break;
        }
    }
}

void HalfOutline::accum_trip(treecore::Array<float>& results) const
{
    results.add(0.0f);

    for (int i = 1; i < outline.size(); i++)
        results.add((outline[i] - outline[i-1]).length() + results[i-1]);
}

} // namespace treeface
