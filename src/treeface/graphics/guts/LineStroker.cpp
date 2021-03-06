#include "treeface/graphics/guts/LineStroker.h"

#include "treeface/graphics/guts/Utils.h"

#include "treeface/math/Constants.h"
#include "treeface/math/Mat2.h"

using namespace treecore;

namespace treeface
{

void LineStroker::cap_begin( const Vec2f& skeleton, const Vec2f& direction )
{
    treecore_assert( !stroke_done );
    treecore_assert( !part_left.sunken && !part_right.sunken );
    treecore_assert( std::abs( direction.length2() - 1.0f ) < 0.0001f );

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "begin cap" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )

    Vec2f ortho = direction.get_ortholog();

    const Vec2f r     = ortho * style.half_width;
    const Vec2f r_pre = direction * -style.half_width;

    switch (style.cap)
    {
    case LINE_CAP_BUTT:
        break;
    case LINE_CAP_ROUND:
    {
        float angle_step = PI / (STROKE_ROUNDNESS / 2);

        Vec2f r_tmp = r_pre;
        Mat2f rot; rot.set_rotate( -angle_step );
        for (int i = 1; i <= (STROKE_ROUNDNESS / 4 - 1); i++)
        {
            r_tmp = rot * r_tmp;
            part_left.add( skeleton + r_tmp, 0 );
        }

        r_tmp = r_pre;
        rot.set_rotate( angle_step );
        for (int i = 1; i <= (STROKE_ROUNDNESS / 4 - 1); i++)
        {
            r_tmp = rot * r_tmp;
            part_right.add( skeleton + r_tmp, 0 );
        }

        break;
    }
    case LINE_CAP_SQUARE:
        part_left.add( skeleton + r + r_pre, 0 );
        part_right.add( skeleton - r + r_pre, 0 );
        break;
    }

    part_left.add( skeleton + r, 0 );
    part_right.add( skeleton - r, 0 );

    part_left.sunken  = false;
    part_right.sunken = false;

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "begin cap done" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )
}

void LineStroker::close_stroke_begin( const Vec2f& skeleton, const Vec2f& direction )
{
    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "begin closed stroke" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )

    const Vec2f ortho = direction.get_ortholog();
    const Vec2f r = ortho * style.half_width;
    part_left.add( skeleton + r, 0 );
    part_right.add( skeleton - r, 0 );
    part_left.sunken  = false;
    part_right.sunken = false;

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "after beginning closed stroke" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )
}

void LineStroker::cap_end( const Vec2f& skeleton, const Vec2f& direction )
{
    treecore_assert( !stroke_done );
    treecore_assert( !(part_left.sunken && part_right.sunken) ); // beishuiyanmo, buzhisuocuo, tiqianshejing
    treecore_assert( std::abs( direction.length2() - 1.0f ) < 0.0001f );

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "end cap" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )

    JointID id = std::max( part_left.joint_ids.getLast(), part_right.joint_ids.getLast() ) + 1;
    Vec2f ortho = direction.get_ortholog();

    const Vec2f r = ortho * style.half_width;

    part_left.salvage( skeleton, r, id );
    part_right.salvage( skeleton, r, id );

    switch (style.cap)
    {
    case LINE_CAP_BUTT:
        break;
    case LINE_CAP_ROUND:
    {
        float angle_step = PI / (STROKE_ROUNDNESS / 2);

        Vec2f r_tmp = r;
        Mat2f rot; rot.set_rotate( -angle_step );
        for (int i = 1; i <= (STROKE_ROUNDNESS / 4 - 1); i++)
        {
            r_tmp = rot * r_tmp;
            part_left.add( skeleton + r_tmp, id );
        }

        r_tmp = r * -1.0f;
        rot.set_rotate( angle_step );
        for (int i = 1; i <= (STROKE_ROUNDNESS / 4 - 1); i++)
        {
            r_tmp = rot * r_tmp;
            part_right.add( skeleton + r_tmp, id );
        }

        break;
    }
    case LINE_CAP_SQUARE:
    {
        const Vec2f r_post = direction * style.half_width;
        part_left.add( skeleton + r + r_post, id );
        part_right.add( skeleton - r + r_post, id );
        break;
    }
    }

    stroke_done = true;

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "end cap done" );
                   sucker.draw_outline( part_right );
                   sucker.draw_unit_vector( skeleton, direction );
    )
}

Vec2f LineStroker::extend_stroke( const Vec2f& v_prev, const Vec2f& p1, const Vec2f& p2, bool use_line_join )
{
    treecore_assert( !stroke_done );
    treecore_assert( !(part_left.sunken && part_right.sunken) ); // beishuiyanmo, buzhisuocuo, tiqianshejing
    treecore_assert( std::abs( v_prev.length2() - 1.0f ) < 0.0001f );
    treecore_assert( p2 != p1 );

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "extend stroke" );
                   sucker.draw_outline( part_right );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
                   sucker.rgba( SUCKER_BLUE, 0.5f );
                   sucker.draw_vector( p1, p2 );
                   sucker.rgba( SUCKER_BLACK, 0.5f );
                   sucker.draw_unit_vector( p1, v_prev * -1.0f );
    )

    Vec2f v_curr = p2 - p1;
    v_curr.normalize();
    Vec2f ortho_prev = v_prev.get_ortholog();
    Vec2f ortho_curr = v_curr.get_ortholog();

    Vec2f r_prev = ortho_prev * style.half_width;
    Vec2f r_curr = ortho_curr * style.half_width;

    float turn_sine = v_prev % v_curr;

    JointID joint_id1;
    {
        JointID prev_id_left  = part_left.joint_ids.size() > 0 ? part_left.joint_ids.getLast() : 0;
        JointID prev_id_right = part_right.joint_ids.size() > 0 ? part_right.joint_ids.getLast() : 0;
        joint_id1 = std::max( prev_id_left, prev_id_right );
    }
    JointID joint_id2 = joint_id1 + 1;

    // is turning
    if (std::abs(turn_sine) > 0.05f)
    {
        HalfOutline* part_inner;
        HalfOutline* part_outer;

        // decide turn side
        if (turn_sine > 0.0f)
        {
            SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "turn left" );
                           sucker.draw_outline( part_right );
                           sucker.rgba( SUCKER_BLUE, 0.5f );
                           sucker.draw_unit_vector( p1, v_curr );
                           sucker.rgba( SUCKER_BLACK, 0.5f );
                           sucker.draw_unit_vector( p1, -v_prev );
            )
            part_inner = &part_left;
            part_outer = &part_right;
        }
        else
        {
            SUCK_GEOM_BLK( OutlineSucker sucker( part_right, "turn right" );
                           sucker.draw_outline( part_left );
                           sucker.rgba( SUCKER_BLUE, 0.5f );
                           sucker.draw_unit_vector( p1, v_curr );
                           sucker.rgba( SUCKER_BLACK, 0.5f );
                           sucker.draw_unit_vector( p1, -v_prev );
            )
            part_inner = &part_right;
            part_outer = &part_left;
        }

        // process outer side
        part_outer->salvage( p1, r_prev, joint_id1 );

        if (use_line_join)
        {
            switch (style.join)
            {
            case LINE_JOIN_MITER:
                part_outer->add_miter_point( p1, joint_id1, ortho_prev, ortho_curr, style );
                break;
            case LINE_JOIN_ROUND:
                part_outer->add_round_points( p1, joint_id1, ortho_prev, ortho_curr, style );
                break;
            case LINE_JOIN_BEVEL:
                break;
            }
        }

        part_outer->add( p1 + r_curr * part_outer->side, joint_id1 );
        part_outer->add( p2 + r_curr * part_outer->side, joint_id2 );

        SUCK_GEOM_BLK( OutlineSucker sucker( *part_outer, "plotted last two points for outer part" );
                       sucker.draw_outline( *part_inner );
                       sucker.rgb( SUCKER_GREEN );
                       sucker.draw_vtx( part_outer->outline.size() - 2 );
                       sucker.draw_vtx( part_outer->outline.size() - 1 );
        )

        // process inner side
        part_inner->process_inner( *part_outer, p1, joint_id1, p2, ortho_curr, style );

        SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "inner part plotted" );
                       sucker.draw_outline( *part_outer );
        )
    }
    // no turn
    else
    {
        part_left.add( p2 + r_curr, joint_id2 );
        part_right.add( p2 - r_curr, joint_id2 );

        SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "extend with no turn" );
                       sucker.draw_outline( part_right );
                       sucker.rgb( SUCKER_GREEN );
                       sucker.draw_vtx( part_left.outline.getLast() );
                       sucker.draw_vtx( part_right.outline.getLast() );

        )
    }

    return v_curr;
}

void LineStroker::close_stroke_end( const Vec2f& skeleton_last, const Vec2f& skeleton_first, const Vec2f& v_begin )
{
    treecore_assert( !stroke_done );
    treecore_assert( !(part_left.sunken && part_right.sunken) ); // beishuiyanmo, buzhisuocuo, tiqianshejing
    treecore_assert( skeleton_last != skeleton_first );
    treecore_assert( std::abs( v_begin.length2() - 1.0f ) < 0.0001f );

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "close stroke" );
                   sucker.draw_outline( part_right );
                   sucker.draw_vtx( skeleton_last );
                   sucker.draw_vtx( skeleton_first );
                   sucker.draw_unit_vector( skeleton_first, v_begin );
    );

    Vec2f v_curr = skeleton_first - skeleton_last;
    float l_curr = v_curr.normalize();

    Vec2f ortho_curr  = v_curr.get_ortholog();
    Vec2f ortho_begin = v_begin.get_ortholog();

    Vec2f r_curr  = ortho_curr * style.half_width;
    Vec2f r_begin = ortho_begin * style.half_width;

    JointID end_id    = std::max( part_left.joint_ids.getLast(), part_right.joint_ids.getLast() );
    float   turn_sine = v_curr % v_begin;

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
            SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "turn left" );
                           sucker.draw_outline( *part_outer );
            )
        }
        else
        {
            part_inner = &part_right;
            part_outer = &part_left;
            SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "turn right" );
                           sucker.draw_outline( *part_outer );
            )
        }

        // process outer side
        part_outer->salvage( skeleton_first, r_curr, end_id );

        switch (style.join)
        {
        case LINE_JOIN_MITER:
            part_outer->add_miter_point( skeleton_first, end_id, ortho_curr, ortho_begin, style );
            break;
        case LINE_JOIN_ROUND:
            part_outer->add_round_points( skeleton_first, end_id, ortho_curr, ortho_begin, style );
            break;
        case LINE_JOIN_BEVEL:
            break;
        }

        part_outer->add( skeleton_first + r_begin * part_outer->side, end_id );

        SUCK_GEOM_BLK( OutlineSucker sucker( *part_outer, "last point of outer side" );
                       sucker.draw_outline( *part_outer );
                       sucker.rgb( SUCKER_GREEN );
                       sucker.draw_vtx( part_outer->outline.getLast() );
        );

        // process inner side
        {
            Vec2f head_cross;
            int   i_head = part_inner->find_cross_from_head( skeleton_last + r_curr * part_inner->side,
                                                             skeleton_first + r_curr * part_inner->side,
                                                             head_cross,
                                                             TAIL_FIND_LIMIT );

            if (0 <= i_head && i_head < part_inner->size() - 2)
            {
                // process head
                if (i_head > 0)
                {
                    SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "shift from inner head" );
                                   sucker.rgb( SUCKER_RED );
                                   for (int i = 0; i < i_head; i++)
                                       sucker.draw_vtx( i );
                    );

                    part_inner->outline.removeRange( 0, i_head );
                    part_inner->joint_ids.removeRange( 0, i_head );
                    part_inner->outline_bounds.removeRange( 0, i_head );

                    SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "after shift" ); );
                }

                part_inner->outline[0]        = head_cross;
                part_inner->outline_bounds[0] = BBox2f( part_inner->outline[0], part_inner->outline[1] );

                SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "inner head modified" );
                               sucker.rgb( SUCKER_BLUE );
                               sucker.draw_vtx( part_inner->outline.getFirst() );
                );

                // process tail
                SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "modify tail" );
                               sucker.rgb( SUCKER_BLUE );
                               sucker.draw_vtx( head_cross );
                               sucker.draw_vector( part_inner->outline.getLast(), head_cross );
                );

                int last_i = part_inner->size() - 1;
                part_inner->outline[last_i] = head_cross;
                part_inner->outline_bounds.getLast() = BBox2f( part_inner->outline[last_i], part_inner->outline[last_i - 1] );

                SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "inner part closed" );
                               sucker.draw_outline( *part_outer );
                );
            }
            else
            {
                part_inner->add( skeleton_first + r_begin * part_inner->side, end_id );
                SUCK_GEOM_BLK( OutlineSucker sucker( *part_inner, "inner part not closed, link back directly" );
                               sucker.draw_outline( *part_outer );
                               sucker.rgb( SUCKER_GREEN );
                               sucker.draw_vtx( part_inner->outline.getLast() );
                )
            }
        }
    }
    //
    // no turn
    //
    else
    {
        // the last two vertices is duplicate with the first two vertices
        if (!part_left.sunken)
        {
            treecore_assert( part_left.outline.getFirst() == part_left.outline.getLast() );
        }

        if (!part_right.sunken)
        {
            treecore_assert( part_right.outline.getFirst() == part_right.outline.getLast() );
        }

        SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "no turn, don't need operation" );
                       sucker.draw_outline( part_right );
        );
    }

    stroke_done = true;
}

inline bool _should_advance_( const Array<JointID>& ids_self, const Array<JointID>& ids_peer, int i_self, int i_peer )
{
    if (i_self == ids_self.size() - 1)
        return false;

    if (i_peer == ids_peer.size() - 1)
        return true;

    // now both side have next thing
    if (ids_self[i_self] < ids_peer[i_peer])
    {
        return true;
    }
    else if (ids_self[i_self] == ids_peer[i_peer])
    {
        if (ids_self[i_self + 1] <= ids_peer[i_peer + 1])
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

void LineStroker::triangulate( Geometry::HostVertexCache& result_vertices, treecore::Array<IndexType>& result_indices, bool path_is_closed ) const
{
    treecore_assert( stroke_done );

    SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "begin triangulate" );
                   sucker.draw_outline( part_right );
    )

    Array<float> trip_left;
    Array<float> trip_right;
    part_left.accum_trip( trip_left );
    part_right.accum_trip( trip_right );

    treecore_assert( trip_left.size() == part_left.size() );
    treecore_assert( trip_right.size() == part_right.size() );

    // initial state
    int i_left_prev  = 0;
    int i_right_prev = 0;

    const IndexType idx_left_begin = result_vertices.size();
    result_vertices.add( StrokeVertex{ part_left.outline[0],
                                       part_left.get_tangent_unorm( 0, path_is_closed ),
                                       trip_left[0] / trip_left.getLast(),
                                       0.0f } );
    const IndexType idx_right_begin = result_vertices.size();
    result_vertices.add( StrokeVertex{ part_right.outline[0],
                                       part_right.get_tangent_unorm( 0, path_is_closed ),
                                       trip_right[0] / trip_right.getLast(),
                                       1.0f } );

    IndexType idx_left_prev  = idx_left_begin;
    IndexType idx_right_prev = idx_right_begin;

    // move on two sides
    for (;; )
    {
        int i_left  = i_left_prev;
        int i_right = i_right_prev;

        IndexType idx_left  = idx_left_prev;
        IndexType idx_right = idx_right_prev;

        bool left_move_on  = _should_advance_( part_left.joint_ids, part_right.joint_ids, i_left_prev, i_right_prev );
        bool right_move_on = _should_advance_( part_right.joint_ids, part_left.joint_ids, i_right_prev, i_left_prev );

        treecore_assert( left_move_on || right_move_on );

        // perform vertex move on
        if (left_move_on)
        {
            i_left++;
            idx_left = result_vertices.size();
            result_vertices.add( StrokeVertex{ part_left.outline[i_left],
                                               part_left.get_tangent_unorm( i_left, path_is_closed ),
                                               trip_left[i_left] / trip_left.getLast(),
                                               0.0f } );

            SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "left move on" );
                           sucker.rgba( SUCKER_GREEN, 0.5f );
                           sucker.draw_vtx( i_left_prev );
                           sucker.rgb( SUCKER_GREEN );
                           sucker.draw_vtx( i_left );
            )
        }

        if (right_move_on)
        {
            i_right++;
            idx_right = result_vertices.size();
            result_vertices.add( StrokeVertex{ part_right.outline[i_right],
                                               part_right.get_tangent_unorm( i_right, path_is_closed ),
                                               trip_right[i_right] / trip_right.getLast(),
                                               1.0f } );

            SUCK_GEOM_BLK( OutlineSucker sucker( part_right, "right move on" );
                           sucker.rgba( SUCKER_GREEN, 0.5f );
                           sucker.draw_vtx( i_right_prev );
                           sucker.rgb( SUCKER_GREEN );
                           sucker.draw_vtx( i_right );
            )
        }

        // generate triangle
        if (left_move_on)
        {
            treecore_assert( idx_left_prev < idx_left );
            result_indices.add( idx_left_prev );
            result_indices.add( idx_right_prev );
            result_indices.add( idx_left );

            SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "fill" );
                           sucker.draw_outline( part_right );
                           sucker.rgba( SUCKER_GREEN, 0.7f );
                           sucker.draw_trig( result_vertices.get<StrokeVertex>( idx_left_prev ).position,
                                             result_vertices.get<StrokeVertex>( idx_right_prev ).position,
                                             result_vertices.get<StrokeVertex>( idx_left ).position );
            )

            if (right_move_on)
            {
                treecore_assert( idx_right_prev < idx_right );
                result_indices.add( idx_right_prev );
                result_indices.add( idx_right );
                result_indices.add( idx_left );

                SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "fill" );
                               sucker.draw_outline( part_right );
                               sucker.rgba( SUCKER_GREEN, 0.7f );
                               sucker.draw_trig( result_vertices.get<StrokeVertex>( idx_right_prev ).position,
                                                 result_vertices.get<StrokeVertex>( idx_right ).position,
                                                 result_vertices.get<StrokeVertex>( idx_left ).position );
                )
            }
        }
        else
        {
            if (right_move_on)
            {
                treecore_assert( idx_right_prev < idx_right );
                result_indices.add( idx_left_prev );
                result_indices.add( idx_right_prev );
                result_indices.add( idx_right );

                SUCK_GEOM_BLK( OutlineSucker sucker( part_left, "fill" );
                               sucker.draw_outline( part_right );
                               sucker.rgba( SUCKER_GREEN, 0.7f );
                               sucker.draw_trig( result_vertices.get<StrokeVertex>( idx_left_prev ).position,
                                                 result_vertices.get<StrokeVertex>( idx_right_prev ).position,
                                                 result_vertices.get<StrokeVertex>( idx_right ).position );
                )
            }
            else
            {
                abort();
            }
        }

        i_left_prev    = i_left;
        i_right_prev   = i_right;
        idx_left_prev  = idx_left;
        idx_right_prev = idx_right;

        if (i_left  == part_left.size() - 1 &&
            i_right == part_right.size() - 1)
            break;
    }
}

} // namespace treeface
