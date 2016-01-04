#ifndef TREEFACE_LINE_STROKER_H
#define TREEFACE_LINE_STROKER_H

#include "treeface/gl/type.h"
#include "treeface/graphics/guts/HalfOutline.h"
#include "treeface/graphics/utils.h"
#include "treeface/graphics/shapegenerator.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"

#include <treecore/Array.h>

namespace treeface
{

struct LineStroker
{
    LineStroker( const StrokeStyle& pub_style )
        : style{ pub_style.cap, pub_style.join, std::cos( pub_style.miter_cutoff ), pub_style.width / 2 }
        , part_left( 1.0f )
        , part_right( -1.0f )
    {}

    void cap_begin( const Vec2f& skeleton, const Vec2f& direction );
    void cap_end( const Vec2f& skeleton, const Vec2f& direction );

    Vec2f extend_stroke( const Vec2f& v_prev, const Vec2f& p1, const Vec2f& p2, bool use_line_join );

    void close_stroke( const Vec2f& v_prev, const Vec2f& p, const Vec2f& v_next );

    void triangulate( treecore::Array<StrokeVertex>& result_vertices, treecore::Array<IndexType>& result_indices, bool path_is_closed ) const;

    HalfOutline part_left;
    HalfOutline part_right;
    bool stroke_done = false;
    InternalStrokeStyle style;
};

} // namespace treeface

#endif // TREEFACE_LINE_STROKER_H
