#ifndef TREEFACE_GRAPHICS_GUTS_UTILS_H
#define TREEFACE_GRAPHICS_GUTS_UTILS_H

#include "treeface/enums.h"
#include "treeface/gl/type.h"
#include "treeface/math/vec2.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

#define TAIL_FIND_LIMIT 32
#define STROKE_ROUNDNESS 32

namespace treeface
{

typedef treecore::int16 JointID;

struct InternalStrokeStyle
{
    LineCap  cap;
    LineJoin join;
    float    miter_cutoff_cosine;
    float    half_width;
};

double clockwise_accum( const Geometry::HostVertexCache& vertices, IndexType i_begin, IndexType i_end ) noexcept;

inline float calc_step( float total, float step_size, int min_num_step )
{
    int num_step_use = int(total / step_size);
    if (num_step_use < min_num_step) num_step_use = min_num_step;
    return total / num_step_use;
}

inline bool points_are_in_line( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3 )
{
    Vec2f v12 = p2 - p1;
    Vec2f v13 = p3 - p1;

    float l12_2 = v12.length2();
    float l13_2 = v13.length2();

    float sine_l12_l13 = v12 % v13;
    if (sine_l12_l13 * sine_l12_l13 > l12_2 * l13_2 / 10000)
        return false;

    float cosine_l12_l13 = v12 * v13;
    if (0.0f <= cosine_l12_l13 &&  l12_2 <= l13_2)
        return true;
    else
        return false;
}

///
/// \brief determine whether two line segments intersect each other, and
///        calculate the cross point
///
/// if the two line segments are parallel and overlap, the middle point of the
/// overlapping part will be treated as crossing point.
///
/// \param p1      start point of line segment 1
/// \param p2      end point of line segment 1
/// \param p3      start point of line segment 2
/// \param p4      end point of line segment 2
/// \param result  cross point
///
/// \return whether intersection is occurred
///
inline bool cross_test_inc( const Vec2f& p1, const Vec2f& p2,
                            const Vec2f& p3, const Vec2f& p4,
                            Vec2f& result )
{
    const Vec2f v12 = p2 - p1;
    const Vec2f v34 = p4 - p3;

    // process zero-length line segments
    if (p1 == p2)
    {
        if (p3 == p4)
        {
            if (p1 == p3)
            {
                result = p1;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if ( points_are_in_line( p3, p1, p4 ) )
            {
                result = p1;
                return true;
            }
            else { return false; }
        }
    }
    else
    {
        if (p3 == p4)
        {
            if ( points_are_in_line( p1, p3, p2 ) )
            {
                result = p3;
                return true;
            }

            else { return false; }
        }
    }

    float denom = v12 % v34;

    // direction are parallel
    // do 1-dimensional overlap test
    if (denom == 0.0f)
    {
        Vec2f       v12_norm = v12;
        const float len12    = v12_norm.normalize();
        const float len34    = v34.length();

        const float line_dist = (p3 - p1) % v12_norm;

        // two segments (almostly) on same line
        if (line_dist * line_dist < len12 * len34 / 10000)
        {
            float proj1 = 0.0f;
            float proj2 = len12;
            float proj3 = (p3 - p1) * v12_norm;
            float proj4 = (p4 - p1) * v12_norm;

            if (proj4 < proj3)
                std::swap( proj3, proj4 );

            float proj_cross = std::numeric_limits<float>::quiet_NaN();

            if (proj1 <= proj3)
            {
                if (proj3 <= proj2)
                {
                    if (proj2 <= proj4) proj_cross = (proj2 + proj3) / 2; // 1-3-2-4
                    else                proj_cross = (proj3 + proj4) / 2; // 1-3-4-2
                }
                else { return false; } // 1-2 3-4
            }
            else
            {
                if (proj1 <= proj4)
                {
                    if (proj4 <= proj2) proj_cross = (proj1 + proj4) / 2; // 3-1-4-2
                    else                proj_cross = (proj1 + proj2) / 2; // 3-1-2-4
                }
                else { return false; } // 3-4 1-2
            }

            result = p1 + v12_norm * proj_cross;
            return true;
        }
        // not on same line
        else
        {
            return false;
        }
    }

    // not parallel, do testing
    float cross12 = p1 % p2;
    float cross34 = p3 % p4;

    result.set( (v12.x * cross34 - v34.x * cross12) / denom,
                (v12.y * cross34 - v34.y * cross12) / denom );

    float rule_34_on_12 = ( (p3 - p1) % v12 ) * ( (p4 - p1) % v12 );
    float rule_12_on_34 = ( (p1 - p3) % v34 ) * ( (p2 - p3) % v34 );

    if (rule_34_on_12 <= 0 && rule_12_on_34 <= 0)
        return true;
    else
        return false;
}

} // namespace treeface

#endif // TREEFACE_GRAPHICS_GUTS_UTILS_H
