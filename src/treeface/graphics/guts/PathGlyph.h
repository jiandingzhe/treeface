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
    Vec2f center;
    float angle;
    bool  is_cclw;
};

struct PathGlyphBessel3
{
    Vec2f ctrl;
};

struct PathGlyphBessel4
{
    Vec2f ctrl1;
    Vec2f ctrl2;
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
        , arc{ center, angle, is_cclw }
    {}

    PathGlyph( const Vec2f& ctrl, const Vec2f& end )
        : type( GLYPH_TYPE_BESSEL3 )
        , end( end )
        , bessel3{ ctrl }
    {}

    PathGlyph( const Vec2f& ctrl1, const Vec2f& ctrl2, const Vec2f& end )
        : type( GLYPH_TYPE_BESSEL4 )
        , bessel4{ ctrl1, ctrl2 }
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