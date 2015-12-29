#include "treeface/graphics/guts/LineStroker.h"

#include "treeface/graphics/guts/utils.h"

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"

using namespace treecore;

namespace treeface
{


void LineStroker::cap_begin(const Vec2f& skeleton, const Vec2f& direction)
{
    jassert(!stroke_done);
    jassert(!part_left.sunken && !part_right.sunken);
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

    part_left.sunken = false;
    part_right.sunken = false;
}


void LineStroker::cap_end(const Vec2f& skeleton, const Vec2f& direction)
{
    jassert(!stroke_done);
    jassert(! (part_left.sunken && part_right.sunken)); // beishuiyanmo, buzhisuocuo, tiqianshejing
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
    jassert(! (part_left.sunken && part_right.sunken)); // beishuiyanmo, buzhisuocuo, tiqianshejing
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

void LineStroker::close_stroke(const Vec2f& v_prev, const Vec2f& p, const Vec2f& v_next)
{
    jassert(!stroke_done);
    jassert(! (part_left.sunken && part_right.sunken)); // beishuiyanmo, buzhisuocuo, tiqianshejing
    jassert(std::abs(v_prev.length2() - 1.0f) < 0.0001f);
    jassert(std::abs(v_next.length2() - 1.0f) < 0.0001f);

    Vec2f ortho_prev = v_prev.get_ortholog();
    Vec2f ortho_next = v_next.get_ortholog();

    Vec2f r_prev = ortho_prev * style.half_width;
    Vec2f r_next = ortho_next * style.half_width;

    JointID joint_id1 = std::max(part_left.joint_ids.getLast(), part_right.joint_ids.getLast());

    float turn_sine = v_prev ^ v_next;

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
        part_outer->salvage(p, r_prev, joint_id1);

        switch (style.join)
        {
        case LINE_JOIN_MITER:
            part_outer->add_miter_point(p, joint_id1, ortho_prev, ortho_next, style);
            break;
        case LINE_JOIN_ROUND:
            part_outer->add_round_points(p, joint_id1, ortho_prev, ortho_next, style);
            break;
        case LINE_JOIN_BEVEL:
            break;
        }

        // process inner side
        part_inner->close_inner();
    }
    //
    // no turn
    //
    else
    {
        // the last two vertices is duplicate with the first two vertices
        if (!part_left.sunken)
        {
            jassert(part_left.outline.getFirst() == part_left.outline.getLast());
            part_left.resize(part_left.size() - 1);
        }

        if (!part_right.sunken)
        {
            jassert(part_right.outline.getFirst() == part_right.outline.getLast());
            part_right.resize(part_right.size() - 1);
        }
    }

    stroke_done = true;
}

void LineStroker::triangulate(treecore::Array<StrokeVertex>& result_vertices, treecore::Array<IndexType>& result_indices) const
{

}

} // namespace treeface
