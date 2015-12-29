#include "treeface/graphics/guts/LineStroker.h"

#include "treeface/graphics/guts/utils.h"

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"

#define TAIL_FIND_LIMIT 5

using namespace treecore;

namespace treeface
{

int LineStroker::HalfOutline::find_cross_from_tail(const Vec2f& p1, const Vec2f& p2, Vec2f& p_cross, int step_limit) const
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

void LineStroker::HalfOutline::add_miter_point(const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style)
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

void LineStroker::HalfOutline::add_round_points(const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style)
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

void LineStroker::HalfOutline::process_inner(const HalfOutline& outer_peer,
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

void LineStroker::cap_begin(const Vec2f& skeleton, const Vec2f& direction)
{
    jassert(!stroke_done);
    jassert(std::abs(direction.length2() - 1.0f) < 0.0001f);
    Vec2f ortho = direction.get_ortholog();

    const Vec2f r = ortho * style.half_width;
    const Vec2f r_pre = direction * - style.half_width;

    switch (style.cap)
    {
    case LINE_CAP_BUTT:
        break;
    case LINE_CAP_ROUND:
    {
        Vec2f r_tmp = r_pre;
        Mat2f rot; rot.set_rotate(-PI / 16);
        for (int i = 1; i <= 14; i++)
        {
            r_tmp = rot * r_tmp;
            part_left.add(skeleton + r_tmp, 0);
        }

        r_tmp = r_pre;
        rot.set_rotate(PI / 16);
        for (int i = 1; i <= 14; i++)
        {
            r_tmp = rot * r_tmp;
            part_right.add(skeleton + r_tmp, 0);
        }

        break;
    }
    case LINE_CAP_SQUARE:
        part_left.add(skeleton + r + r_pre, 0);
        part_right.add(skeleton - r + r_pre, 0);
        break;
    }

    part_left.add(skeleton + r, 0);
    part_right.add(skeleton - r, 0);
    part_left.sunken = false;
    part_right.sunken = false;
}


void LineStroker::cap_end(const Vec2f& skeleton, const Vec2f& direction)
{
    jassert(!stroke_done);
    jassert(std::abs(direction.length2() - 1.0f) < 0.0001f);

    JointID id = std::max(part_left.joint_ids.getLast(), part_right.joint_ids.getLast()) + 1;
    Vec2f ortho = direction.get_ortholog();

    const Vec2f r = ortho * style.half_width;

    switch (style.cap)
    {
    case LINE_CAP_BUTT:
        break;
    case LINE_CAP_ROUND:
    {
        Vec2f r_tmp = r;
        Mat2f rot; rot.set_rotate(-PI / 16);
        for (int i = 1; i <= 14; i++)
        {
            r_tmp = rot * r_tmp;
            part_left.add(skeleton + r_tmp, 0);
        }

        r_tmp = r * -1.0f;
        rot.set_rotate(PI / 16);
        for (int i = 1; i <= 14; i++)
        {
            r_tmp = rot * r_tmp;
            part_right.add(skeleton + r_tmp, 0);
        }

        break;
    }
    case LINE_CAP_SQUARE:
    {
        const Vec2f r_post = direction * style.half_width;
        part_left.add(skeleton + r + r_post, 0);
        part_right.add(skeleton - r + r_post, 0);
        break;
    }
    }

    stroke_done = true;
}

Vec2f LineStroker::extend_stroke(const Vec2f& v_prev, const Vec2f& p1, const Vec2f& p2, bool use_line_join)
{
    jassert(!stroke_done);
    jassert(std::abs(v_prev.length2() - 1.0f) < 0.0001f);
    jassert(p2 != p1);

    Vec2f v_curr = p2 - p1;
    v_curr.normalize();
    Vec2f ortho_prev = v_prev.get_ortholog();
    Vec2f ortho_curr = v_curr.get_ortholog();

    Vec2f r_prev = ortho_prev * style.half_width;
    Vec2f r_curr = ortho_curr * style.half_width;

    float turn_sine = v_prev ^ v_curr;

    JointID joint_id1 = std::max(part_left.joint_ids.getLast(), part_right.joint_ids.getLast());
    JointID joint_id2 = joint_id1 + 1;

    // is turning
    if (turn_sine != 0.0f)
    {
        HalfOutline* part_inner;
        HalfOutline* part_outer;

        // decide turn side
        if (turn_sine > 0.0f)
        {
            part_inner = &part_left;
            part_outer = &part_right;
        }
        else
        {
            part_inner = &part_right;
            part_outer = &part_left;
        }

        // process outer side
        part_outer->salvage(p1, r_prev, joint_id1);

        if (use_line_join)
        {
            switch (style.join)
            {
            case LINE_JOIN_MITER:
                part_outer->add_miter_point(p1, joint_id1, ortho_prev, ortho_curr, style);
                break;
            case LINE_JOIN_ROUND:
                part_outer->add_round_points(p1, joint_id1, ortho_prev, ortho_curr, style);
                break;
            case LINE_JOIN_BEVEL:
                break;
            }
        }

        part_outer->add(p1 + r_curr * part_outer->side, joint_id1);
        part_outer->add(p2 + r_curr * part_outer->side, joint_id2);

        // process inner side
        part_inner->process_inner(*part_outer, p1, joint_id1, p2, ortho_curr, style);
    }
    // no turn
    else
    {
        part_left.add(p2 + r_curr, joint_id2);
        part_right.add(p2 - r_curr, joint_id2);
    }

    return v_curr;
}

} // namespace treeface
