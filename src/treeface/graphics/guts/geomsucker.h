#ifndef TREEFACE_GEOM_SUCKER_H
#define TREEFACE_GEOM_SUCKER_H

#ifdef SUCK_TREECORE_GEOMETRY
#    define SUCK_GEOM( ... ) __VA_ARGS__
#    define SUCK_GEOM_BLK( ... ) {__VA_ARGS__}
#    include <cairo.h>

#    include <treecore/Array.h>

#    include "treeface/gl/type.h"
#    include "treeface/math/vec2.h"
#    include "treeface/graphics/guts/enums.h"

#    define SUCKER_RED      1.0f, 0.0f, 0.0f
#    define SUCKER_ORANGE   1.0f, 0.5f, 0.0f
#    define SUCKER_YELLOW   0.75f, 0.75f, 0.0f
#    define SUCKER_GREEN    0.0f, 0.75f, 0.0f
#    define SUCKER_CYAN     0.0f, 0.75f, 0.75f
#    define SUCKER_BLUE     0.0f, 0.0f, 1.0f
#    define SUCKER_MAGENTA  1.0f, 0.0f, 1.0f
#    define SUCKER_BLACK    0.0f, 0.0f, 0.0f
#    define SUCKER_WHITE    1.0f, 1.0f, 1.0f

namespace treeface
{

struct HalfEdgeNetwork;
struct HalfOutline;

struct GeomSucker
{
    GeomSucker( const HalfEdgeNetwork&  network,
                const treecore::String& title = treecore::String::empty );
    ~GeomSucker();

    void rgba( float r, float g, float b, float a ) {
        cairo_set_source_rgba( context, r, g, b, a );
    }
    void rgb( float r, float g, float b )          {
        cairo_set_source_rgb( context, r, g, b );
    }

    void draw_vtx( IndexType vtx_idx ) const;
    void draw_vtx( const Vec2f& vtx ) const;

    void draw_vector( const Vec2f& start, const Vec2f& end ) const;

    void draw_roled_vtx( IndexType vtx_idx, VertexRole role ) const;
    void draw_merge_vtx( IndexType vtx_idx ) const;
    void draw_split_vtx( IndexType vtx_idx ) const;
    void draw_start_vtx( IndexType vtx_idx ) const;
    void draw_end_vtx( IndexType vtx_idx ) const;
    void draw_regular_left_vtx( IndexType vtx_idx ) const;
    void draw_regular_right_vtx( IndexType vtx_idx ) const;

    void draw_edge( const IndexType i_edge, float offset_rate = 1.0f ) const;
    void draw_edge_stack( const treecore::Array<IndexType>& edges ) const;
    void draw_trig_by_edge( IndexType i_edge1, IndexType i_edge2, IndexType i_edge3 ) const;

    void draw_helper( IndexType i_edge, IndexType i_helper ) const;
    void draw_helper_change( IndexType i_edge, IndexType i_helper_old, IndexType i_helper_new ) const;

    void text( const treecore::String& content, const Vec2f& position ) const;
    void text( const treecore::String& content, IndexType i_vtx ) const;

    const HalfEdgeNetwork& network;

    float width  = std::numeric_limits<float>::signaling_NaN();
    float height = std::numeric_limits<float>::signaling_NaN();
    float line_w = std::numeric_limits<float>::signaling_NaN();

    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
};

struct OutlineSucker
{
    OutlineSucker( const HalfOutline&      outline,
                   const treecore::String& title = treecore::String::empty );
    ~OutlineSucker();

    void rgba( float r, float g, float b, float a ) {
        cairo_set_source_rgba( context, r, g, b, a );
    }
    void rgb( float r, float g, float b )          {
        cairo_set_source_rgb( context, r, g, b );
    }

    void draw_vtx( int i_outline ) const;
    void draw_vtx( const Vec2f& vtx ) const;

    void draw_vector( const Vec2f& start, const Vec2f& end ) const;
    void draw_unit_vector( const Vec2f& start, const Vec2f& v ) const;

    void text( const treecore::String& content, int i_outline ) const;
    void text( const treecore::String& content, const Vec2f& position ) const;

    void draw_outline( const HalfOutline& content ) const;

    void draw_trig( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3 ) const;

    const HalfOutline& outline;
    float width  = std::numeric_limits<float>::signaling_NaN();
    float height = std::numeric_limits<float>::signaling_NaN();
    float line_w = std::numeric_limits<float>::signaling_NaN();
    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
};

}

#else
#    define SUCK_GEOM( ... )
#    define SUCK_GEOM_BLK( ... )
#endif // SUCK_TREECORE_GEOMETRY

#endif // TREEFACE_GEOM_SUCKER_H
