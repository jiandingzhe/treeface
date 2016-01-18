#ifndef TREEFACE_PATH_GLYPH_H
#define TREEFACE_PATH_GLYPH_H

#include "treeface/enums.h"
#include "treeface/graphics/guts/enums.h"
#include "treeface/math/vec2.h"

#include <treecore/Array.h>

namespace treeface
{

struct PathGlyphArc
{
    float center_x;
    float center_y;
    float angle;
    bool  is_cclw;
};

struct PathGlyphBessel3
{
    float ctrl_x;
    float ctrl_y;
};

struct PathGlyphBessel4
{
    float ctrl1_x;
    float ctrl1_y;
    float ctrl2_x;
    float ctrl2_y;
};

struct PathGlyph
{
    explicit PathGlyph( const Vec2f& line_end )
        : type( GLYPH_TYPE_LINE )
        , end( line_end )
    {}

    PathGlyph( const Vec2f& center, const Vec2f& end, float angle, bool is_cclw )
        : type( GLYPH_TYPE_ARC )
        , end( end )
        , arc(PathGlyphArc{ center.x, center.y, angle, is_cclw })
    {}

    PathGlyph( const Vec2f& ctrl, const Vec2f& end )
        : type( GLYPH_TYPE_BESSEL3 )
        , end( end )
        , bessel3(PathGlyphBessel3{ ctrl.x, ctrl.y })
    {}

    PathGlyph( const Vec2f& ctrl1, const Vec2f& ctrl2, const Vec2f& end )
        : type( GLYPH_TYPE_BESSEL4 )
        , bessel4(PathGlyphBessel4{ ctrl1.x, ctrl1.y, ctrl2.x, ctrl2.y })
    {}

    void segment( const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices ) const
    {
        result_vertices.clear();
        switch (type)
        {
        case GLYPH_TYPE_ARC: segment_arc( prev_end, result_vertices ); break;
        case GLYPH_TYPE_BESSEL3:
        case GLYPH_TYPE_BESSEL4: segment_bessel( prev_end, result_vertices ); break;
        case GLYPH_TYPE_LINE: result_vertices.add( end ); break;
        default: abort();
        }
    }

    void segment_arc( const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices ) const;

    void segment_bessel( const Vec2f& prev_end, treecore::Array<Vec2f>& result_vertices ) const;

    GlyphType type;
    Vec2f end;
    union
    {
        PathGlyphArc arc;
        PathGlyphBessel3 bessel3;
        PathGlyphBessel4 bessel4;
    };
};

} // namespace treeface

#endif // TREEFACE_PATH_GLYPH_H
