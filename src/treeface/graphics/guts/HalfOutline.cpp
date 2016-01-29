#include "treeface/graphics/guts/HalfOutline.h"

#include "treeface/math/Constants.h"
#include "treeface/math/Mat2.h"

using namespace treecore;

namespace treeface
{

int HalfOutline::find_cross_from_head( const Vec2f& p1, const Vec2f& p2, Vec2f& p_cross, int step_limit ) const
{
    jassert( outline.size() > 1 );
    jassert( outline.size() == outline_bounds.size() + 1 );

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "find cross from tail" );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    );

    const BBox2f bound_input( p1, p2 );

    for (int i = 0; i < outline.size() && i < step_limit; i++)
    {

        if (bound_input ^ outline_bounds[i])
        {
            const Vec2f& p3 = outline[i];
            const Vec2f& p4 = outline[i + 1];

            if ( cross_test_inc( p1, p2, p3, p4, p_cross ) )
            {
                SUCK_GEOM_BLK( OutlineSucker sucker( *this, "got cross" );
                               sucker.draw_vtx( p1 );
                               sucker.draw_vtx( p2 );
                               sucker.draw_vector( p1, p2 );
                               sucker.rgb( SUCKER_GREEN );
                               sucker.draw_vtx( i );
                               sucker.draw_vtx( i + 1 );
                               sucker.draw_vtx( p_cross );
                )
                return i;
            }

            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "not cross" );
                           sucker.draw_vtx( p1 );
                           sucker.draw_vtx( p2 );
                           sucker.rgb( SUCKER_RED );
                           sucker.draw_vtx( i );
                           sucker.draw_vtx( i + 1 );
                           sucker.draw_vtx( p_cross );
            )
        }
        else
        {
            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "not cross by bbox" );
                           sucker.draw_vtx( p1 );
                           sucker.draw_vtx( p2 );
                           sucker.rgb( SUCKER_RED );
                           sucker.draw_vtx( i );
                           sucker.draw_vtx( i + 1 );
            )
        }
    }

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "failed to find cross" );
                   sucker.rgb( SUCKER_RED );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    )

    return -1;
}

int HalfOutline::find_cross_from_tail( const Vec2f& p1, const Vec2f& p2, Vec2f& p_cross, int step_limit ) const
{
    jassert( outline.size() > 1 );
    jassert( outline.size() == outline_bounds.size() + 1 );

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "find cross from tail" );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    );

    const BBox2f bound_input( p1, p2 );

    for (int i = outline.size() - 2; i >= 0; i--)
    {
        if (outline.size() - i > step_limit) break;

        if (bound_input ^ outline_bounds[i])
        {
            const Vec2f& p3 = outline[i];
            const Vec2f& p4 = outline[i + 1];

            if ( cross_test_inc( p1, p2, p3, p4, p_cross ) )
            {
                SUCK_GEOM_BLK( OutlineSucker sucker( *this, "got cross" );
                               sucker.draw_vtx( p1 );
                               sucker.draw_vtx( p2 );
                               sucker.draw_vector( p1, p2 );
                               sucker.rgb( SUCKER_GREEN );
                               sucker.draw_vtx( i );
                               sucker.draw_vtx( i + 1 );
                               sucker.draw_vtx( p_cross );
                )
                return i;
            }

            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "not cross" );
                           sucker.draw_vtx( p1 );
                           sucker.draw_vtx( p2 );
                           sucker.rgb( SUCKER_RED );
                           sucker.draw_vtx( i );
                           sucker.draw_vtx( i + 1 );
                           sucker.draw_vtx( p_cross );
            )
        }
        else
        {
            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "not cross by bbox" );
                           sucker.draw_vtx( p1 );
                           sucker.draw_vtx( p2 );
                           sucker.rgb( SUCKER_RED );
                           sucker.draw_vtx( i );
                           sucker.draw_vtx( i + 1 );
            )
        }
    }

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "failed to find cross" );
                   sucker.rgb( SUCKER_RED );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    )

    return -1;
}

void HalfOutline::add_miter_point( const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style )
{
    jassert( std::abs( ortho_prev.length2() - 1.0f ) < 0.0001f );
    jassert( std::abs( ortho_curr.length2() - 1.0f ) < 0.0001f );

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "add miter point" );
                   sucker.draw_vtx( skeleton1 );
                   sucker.draw_unit_vector( skeleton1, ortho_prev * -1.0f );
                   sucker.draw_unit_vector( skeleton1, ortho_curr );
    )

    // only do miter join if angle is not too sharp
    float turn_cosine = ortho_prev * ortho_curr;
    if (turn_cosine > style.miter_cutoff_cosine)
    {
        Vec2f ortho_mid = ortho_prev + ortho_curr;
        ortho_mid.normalize();

        float half_cosine = ortho_mid * ortho_prev;
        jassert( 0.0f < half_cosine && half_cosine < 1.0f );
        Vec2f r_mid = ortho_mid * (style.half_width * side / half_cosine);

        add( skeleton1 + r_mid, id );

        SUCK_GEOM_BLK( OutlineSucker sucker( *this, "miter point is added" );
                       sucker.draw_vtx( skeleton1 );
                       sucker.draw_unit_vector( skeleton1, ortho_prev * -1.0f );
                       sucker.draw_unit_vector( skeleton1, ortho_curr );
        )
    }
}

void HalfOutline::add_round_points( const Vec2f& skeleton1, JointID id, const Vec2f& ortho_prev, const Vec2f& ortho_curr, const InternalStrokeStyle& style )
{
    jassert( std::abs( ortho_prev.length2() - 1.0f ) < 0.0001f );
    jassert( std::abs( ortho_curr.length2() - 1.0f ) < 0.0001f );

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "add round points" );
                   sucker.draw_vtx( skeleton1 );
                   sucker.draw_unit_vector( skeleton1, ortho_prev * -1.0f );
                   sucker.draw_unit_vector( skeleton1, ortho_curr );
    );

    // calculate step
    float turn_angle = std::acos( ortho_prev * ortho_curr );
    int   num_step   = turn_angle / PI * STROKE_ROUNDNESS / 2;
    if (num_step < 5) num_step = 5;
    float step_angle = turn_angle / num_step;

    Mat2f step_mat;
    step_mat.set_rotate( -side * step_angle );

    // do rotation
    Vec2f r_tmp = ortho_prev * side * style.half_width;
    for (int i = 1; i < num_step; i++)
    {
        r_tmp = step_mat * r_tmp;
        add( skeleton1 + r_tmp, id );
    }

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "round points added" );
                   sucker.draw_vtx( skeleton1 );
                   sucker.draw_unit_vector( skeleton1, ortho_prev * -1.0f );
                   sucker.draw_unit_vector( skeleton1, ortho_curr );
    )
}

void HalfOutline::process_inner( const HalfOutline&         outer_peer,
                                 const Vec2f&               skeleton1,
                                 const JointID              id1,
                                 const Vec2f&               skeleton2,
                                 const Vec2f&               ortho_curr,
                                 const InternalStrokeStyle& style )
{
    const JointID id2 = id1 + 1;
    Vec2f p_cross;

    Vec2f r_curr = ortho_curr * (side * style.half_width);
    Vec2f p1 = skeleton1 + r_curr;
    Vec2f p2 = skeleton2 + r_curr;

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "process inner" );
                   sucker.draw_outline( outer_peer );
                   sucker.rgb( SUCKER_BLACK );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    )

    {
        int i_self = find_cross_from_tail( p1, p2, p_cross, TAIL_FIND_LIMIT );
        if (i_self >= 0)
        {
            resize( i_self + 1 );
            add( p_cross, id1 );
            add( p2,      id2 );
            sunken = false;

            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "inner solved in same edge" );
                           sucker.draw_outline( outer_peer );
                           sucker.rgb( SUCKER_GREEN );
                           sucker.draw_vtx( p_cross );
                           sucker.draw_vtx( p2 );
            )

            return;
        }
    }

    {
        int i_peer = outer_peer.find_cross_from_tail( p1, p2, p_cross, TAIL_FIND_LIMIT );
        if (i_peer >= 0)
        {
            add( p_cross, id1 );
            add( p2,      id2 );
            sunken = false;

            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "inner solved in peer edge" );
                           sucker.draw_outline( outer_peer );
                           sucker.rgb( SUCKER_GREEN );
                           sucker.draw_vtx( p_cross );
                           sucker.draw_vtx( p2 );
            )

            return;
        }
    }

    {
        jassert( outline.size() > 0 );
        jassert( outer_peer.size() > 0 );

        if ( cross_test_inc( outline.getFirst(), outer_peer.outline.getFirst(), p1, p2, p_cross ) )
        {
            add( p_cross, id1 );
            add( p2,      id2 );
            sunken = false;

            SUCK_GEOM_BLK( OutlineSucker sucker( *this, "inner solved in between inner edge and peer edge" );
                           sucker.draw_outline( outer_peer );
                           sucker.rgb( SUCKER_GREEN );
                           sucker.draw_vtx( p_cross );
                           sucker.draw_vtx( p2 );
            )

            return;
        }
    }

    SUCK_GEOM_BLK( OutlineSucker sucker( *this, "inner unsolved" );
                   sucker.draw_outline( outer_peer );
                   sucker.rgb( SUCKER_RED );
                   sucker.draw_vtx( p1 );
                   sucker.draw_vtx( p2 );
    )

    sunken = true;
}


void HalfOutline::accum_trip( treecore::Array<float>& results ) const
{
    results.add( 0.0f );

    for (int i = 1; i < outline.size(); i++)
        results.add( (outline[i] - outline[i - 1]).length() + results[i - 1] );
}

} // namespace treeface
