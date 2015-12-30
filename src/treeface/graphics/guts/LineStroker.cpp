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
    JointID joint_id2 = joint_id1 + 1;

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

        part_outer->add(p + r_next * part_outer->side, joint_id2);

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
        }

        if (!part_right.sunken)
        {
            jassert(part_right.outline.getFirst() == part_right.outline.getLast());
        }
    }

    stroke_done = true;
}

inline bool _should_advance_(const Array<JointID>& ids_self, const Array<JointID>& ids_peer, int i_self, int i_peer)
{
    if (i_self == ids_self.size() - 1)
        return false;

    if (i_peer == ids_peer.size() - 1)
        return true;

    if (ids_self[i_self + 1] == ids_self[i_self])
        return true;

    if (ids_peer[i_peer + 1] == ids_peer[i_peer])
        return false;

    // now both side have increasing next
    if (ids_self[i_self] <= ids_peer[i_peer])
    {
        return true;
    }
    else
    {
        if (ids_self[i_self+1] <= ids_peer[i_peer+1])
            return true;
        else
            return false;
    }
}

void LineStroker::triangulate(treecore::Array<StrokeVertex>& result_vertices, treecore::Array<IndexType>& result_indices, bool path_is_closed) const
{
    jassert(stroke_done);

    Array<float> trip_left;
    Array<float> trip_right;
    part_left.accum_trip(trip_left);
    part_right.accum_trip(trip_right);

    jassert(trip_left.size() == part_left.size());
    jassert(trip_right.size() == part_right.size());

    // initial state
    int i_left_prev = 0;
    int i_right_prev = 0;

    const IndexType idx_left_begin = result_vertices.size();
    result_vertices.add({part_left.outline[0],
                         part_left.get_tangent_unorm(0, path_is_closed),
                         trip_left[0] / trip_left.getLast(),
                         0.0f});
    const IndexType idx_right_begin = result_vertices.size();
    result_vertices.add({part_right.outline[0],
                         part_right.get_tangent_unorm(0, path_is_closed),
                         trip_right[0] / trip_right.getLast(),
                         1.0f});

    IndexType idx_left_prev = idx_left_begin;
    IndexType idx_right_prev = idx_right_begin;

    // move on two sides
    for (;;)
    {
        int i_left = i_left_prev;
        int i_right = i_right_prev;

        IndexType idx_left = idx_left_prev;
        IndexType idx_right = idx_right_prev;

        bool left_move_on = _should_advance_(part_left.joint_ids, part_right.joint_ids, i_left_prev, i_right_prev);
        bool right_move_on = _should_advance_(part_right.joint_ids, part_left.joint_ids, i_right_prev, i_left_prev);

        jassert(left_move_on || right_move_on);

        // perform vertex move on
        if (left_move_on)
        {
            i_left++;
            idx_left = result_vertices.size();
            result_vertices.add({part_left.outline[i_left],
                                 part_left.get_tangent_unorm(i_left, path_is_closed),
                                 trip_left[i_left] / trip_left.getLast(),
                                 0.0f});
        }

        if (right_move_on)
        {
            i_right++;
            idx_right = result_vertices.size();
            result_vertices.add({part_right.outline[0],
                                 part_right.get_tangent_unorm(i_right, path_is_closed),
                                 trip_right[i_right] / trip_right.getLast(),
                                 1.0f});
        }

        // generate triangle
        if (left_move_on)
        {
            jassert(idx_left_prev < idx_left);
            result_indices.add(idx_left_prev);
            result_indices.add(idx_right_prev);
            result_indices.add(idx_left);

            if (right_move_on)
            {
                jassert(idx_right_prev < idx_right);
                result_indices.add(idx_right_prev);
                result_indices.add(idx_right);
                result_indices.add(idx_left);
            }
        }
        else
        {
            if (right_move_on)
            {
                jassert(idx_right_prev < idx_right);
                result_indices.add(idx_left_prev);
                result_indices.add(idx_right_prev);
                result_indices.add(idx_right);
            }
            else
            {
                abort();
            }
        }

        i_left_prev = i_left;
        i_right_prev = i_right;
        idx_left_prev = idx_left;
        idx_right_prev = idx_right;

        if (i_left  == part_left.size() - 1 &&
            i_right == part_right.size() - 1)
            break;
    }
}

} // namespace treeface
