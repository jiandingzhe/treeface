#ifndef TREEFACE_HALF_OUTLINE_H
#define TREEFACE_HALF_OUTLINE_H

#include "treeface/math/bbox2.h"
#include "treeface/math/vec2.h"
#include "treeface/graphics/guts/utils.h"

#include "treeface/graphics/guts/geomsucker.h"

#include <treecore/String.h>

namespace treeface
{

struct HalfOutline
{
    void add( const Vec2f& vtx, JointID id )
    {
        jassert( outline.size() == joint_ids.size() );
        jassert( ( outline.size() == 0 ) || ( outline.size() == outline_bounds.size() + 1 ) );

        SUCK_GEOM_BLK(
            OutlineSucker sucker( *this, "add vertex" );
            sucker.rgb( SUCKER_GREEN );
            sucker.draw_vtx( vtx );
        )

        if ( outline.size() )
            outline_bounds.add( {outline.getLast(), vtx} );
        outline.add( vtx );
        joint_ids.add( id );
    }

    void resize( int size )
    {
        jassert( size > 0 );
        outline.resize( size );
        outline_bounds.resize( size - 1 );
        joint_ids.resize( size );
    }

    void salvage( const Vec2f& p1, const Vec2f& r_prev, JointID id )
    {
        if (sunken)
        {
            Vec2f vtx = p1 + r_prev * side;
            SUCK_GEOM_BLK(
                OutlineSucker sucker( *this, "salvage" );
                sucker.rgb( SUCKER_GREEN );
                sucker.draw_vtx( vtx );
            )
            sunken = false;
            add( vtx, id );
        }
    }

    void add_miter_point( const Vec2f&               skeleton1,
                          JointID                    id,
                          const Vec2f&               ortho_prev,
                          const Vec2f&               ortho_curr,
                          const InternalStrokeStyle& style );

    void add_round_points( const Vec2f&               skeleton1,
                           JointID                    id,
                           const Vec2f&               ortho_prev,
                           const Vec2f&               ortho_curr,
                           const InternalStrokeStyle& style );

    int find_cross_from_tail( const Vec2f& p1, const Vec2f& p2, Vec2f& result, int step_limit ) const;

    void process_inner( const HalfOutline&         outer_peer,
                        const Vec2f&               skeleton1,
                        JointID                    id1,
                        const Vec2f&               skeleton2,
                        const Vec2f&               ortho_curr,
                        const InternalStrokeStyle& style );

    void close_inner();

    int size() const noexcept
    {
        jassert( outline.size() == joint_ids.size() );
        jassert( outline.size() == outline_bounds.size() + 1 );
        return outline.size();
    }

    Vec2f get_tangent_unorm( int i, bool path_is_closed ) const
    {
        jassert( outline.size() > 1 );
        Vec2f result;
        if (i == 0)
        {
            if (path_is_closed)
                result = outline[1] - outline[outline.size() - 2];
            else
                result = outline[1] - outline[0];
        }
        else if (i == outline.size() - 1)
        {
            if (path_is_closed)
                result = outline[1] - outline[outline.size() - 2];
            else
                result = outline[i] - outline[i - 1];
        }
        else
        {
            result = outline[i + 1] - outline[i - 1];
        }
        return result;
    }

    void accum_trip( treecore::Array<float>& results ) const;

    treecore::Array<Vec2f> outline;
    treecore::Array<BBox2f> outline_bounds;
    treecore::Array<JointID> joint_ids;
    float side; // 1 for left, -1 for right
    bool sunken;
};

} // namespace treeface

#endif // TREEFACE_HALF_OUTLINE_H
