#ifdef SUCK_TREECORE_GEOMETRY

#include "treeface/graphics/guts/geomsucker.h"

#include "cairo-svg.h"

#include <treecore/Process.h>
#include <treecore/File.h>
#include <treecore/StringRef.h>

#include "treeface/graphics/guts/HalfOutline.h"
#include "treeface/graphics/guts/HalfEdgeNetwork.h"
#include "treeface/math/constants.h"

using namespace treecore;

namespace treeface
{

GeomSucker::GeomSucker( const HalfEdgeNetwork& network, const treecore::String& title )
    : network( network )
{
    // get X and Y boundary
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();

    for (const Vec2f& vtx : network.vertices)
    {
        if (x_min > vtx.x) x_min = vtx.x;
        if (y_min > vtx.y) y_min = vtx.y;
        if (x_max < vtx.x) x_max = vtx.x;
        if (y_max < vtx.y) y_max = vtx.y;
    }

    // get scales
    width  = x_max - x_min;
    height = y_max - y_min;

    line_w = std::sqrt( width * height ) / 200;

    // decide output file name
    String file_out;
    for (int i = 0;; i++)
    {
        file_out = String( pointer_sized_int( Process::getProcessID() ) ) + "_" + String( i ) + ".svg";

        if ( !File::getCurrentWorkingDirectory().getChildFile( file_out ).exists() )
            break;
    }

    // create cairo stuffs
    float border = std::min( width * 0.5, height * 0.5 );
    surface = cairo_svg_surface_create( file_out.toRawUTF8(), (width + border * 2) * 5, (height + border * 2) * 5 );
    context = cairo_create( surface );

    // set to initial state
    cairo_scale( context, 5, 5 );
    cairo_translate( context, border, border );
    cairo_translate( context, -x_min, height + y_min );

    cairo_set_line_width( context, line_w );
    cairo_set_line_cap( context, CAIRO_LINE_CAP_ROUND );
    cairo_set_line_join( context, CAIRO_LINE_JOIN_ROUND );

    cairo_save( context );

    {
        //draw axis
        cairo_move_to( context, x_min, 0 );
        cairo_line_to( context, x_max, 0 );
        cairo_move_to( context, 0.0, -y_min );
        cairo_line_to( context, 0.0, -y_max );

        cairo_set_line_width( context, 2 * line_w );
        cairo_set_source_rgba( context, SUCKER_BLACK, 0.4 );
        cairo_stroke( context );

        // draw skeleton
        cairo_set_line_width( context, line_w );

        Array<bool> rendered;
        rendered.resize( network.edges.size() );
        for (int i = 0; i < network.edges.size(); i++) rendered[i] = false;

        for (int i_begin = 0; i_begin < network.edges.size(); i_begin++)
        {
            if (rendered[i_begin]) continue;

            cairo_new_path( context );

            for (int i_edge = i_begin;; )
            {
                const HalfEdge& edge = network.edges[i_edge];

                const Vec2f& vtx = edge.get_vertex( network.vertices );
                cairo_line_to( context, vtx.x, -vtx.y );
                rendered[i_edge] = true;

                i_edge = edge.idx_next_edge;
                if (i_edge == i_begin) break;
            }
            cairo_close_path( context );
            cairo_stroke( context );
        }

        // draw title
        cairo_set_font_size( context, line_w * 8 );
        cairo_text_extents_t ext;
        cairo_text_extents( context, title.toRawUTF8(), &ext );

        cairo_move_to( context, (width - ext.width) / 2, -(height - ext.height) / 2 );
        cairo_set_source_rgba( context, 0.0, 0.0, 0.0, 1.0 );
        cairo_show_text( context, title.toRawUTF8() );
    }

    cairo_restore( context );
}

GeomSucker::~GeomSucker()
{
    if (context)
        cairo_destroy( context );

    if (surface)
        cairo_surface_destroy( surface );
}

void GeomSucker::draw_vtx( IndexType vtx_idx ) const
{
    const Vec2f& vtx = network.vertices[vtx_idx];
    draw_vtx( vtx );
}

void GeomSucker::draw_vtx( const Vec2f& vtx ) const
{
    cairo_new_path( context );
    cairo_arc( context, vtx.x, -vtx.y, line_w * 2, 0, 3.14159265 * 2 );
    cairo_fill( context );
}

void GeomSucker::draw_vector( const Vec2f& start, const Vec2f& end ) const
{
    cairo_save( context );
    cairo_new_path( context );

    Vec2f v = end - start;
    v.normalize();

    float arrow_sz = line_w * 2;
    Vec2f ortho( -v.y, v.x );
    Vec2f p_arrow_root  = end - v * (arrow_sz * 3.0f);
    Vec2f p_arrow_root2 = end - v * (arrow_sz * 2.0f);
    Vec2f p_arrow1      = p_arrow_root + ortho * arrow_sz;
    Vec2f p_arrow2      = p_arrow_root + ortho * -arrow_sz;

    cairo_move_to( context, start.x, -start.y );
    cairo_line_to( context, end.x, -end.y );
    cairo_set_line_width( context, line_w );
    cairo_stroke( context );

    cairo_move_to( context, end.x, -end.y );
    cairo_line_to( context, p_arrow1.x,      -p_arrow1.y );
    cairo_line_to( context, p_arrow_root2.x, -p_arrow_root2.y );
    cairo_line_to( context, p_arrow2.x,      -p_arrow2.y );
    cairo_fill( context );

    cairo_restore( context );
}

void GeomSucker::draw_roled_vtx( IndexType vtx_idx, VertexRole role ) const
{
    switch (role)
    {
    case VTX_ROLE_START: draw_start_vtx( vtx_idx ); break;
    case VTX_ROLE_END: draw_end_vtx( vtx_idx ); break;
    case VTX_ROLE_LEFT: draw_regular_left_vtx( vtx_idx ); break;
    case VTX_ROLE_RIGHT: draw_regular_right_vtx( vtx_idx ); break;
    case VTX_ROLE_MERGE: draw_merge_vtx( vtx_idx ); break;
    case VTX_ROLE_SPLIT: draw_split_vtx( vtx_idx ); break;
    default: abort();
    }
}

void GeomSucker::draw_merge_vtx( IndexType vtx_idx ) const
{
    const Vec2f& vtx = network.vertices[vtx_idx];
    Vec2f        p1  = vtx - Vec2f( 0.0f, line_w * 2 );
    Vec2f        p2  = vtx + Vec2f( line_w * 2, line_w * 2 );
    Vec2f        p3  = vtx + Vec2f( -line_w * 2, line_w * 2 );

    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_fill( context );
}

void GeomSucker::draw_split_vtx( IndexType vtx_idx ) const
{
    const Vec2f& vtx = network.vertices[vtx_idx];
    Vec2f        p1  = vtx + Vec2f( 0.0f, line_w * 2 );
    Vec2f        p2  = vtx + Vec2f( line_w * 2, -line_w * 2 );
    Vec2f        p3  = vtx + Vec2f( -line_w * 2, -line_w * 2 );

    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_fill( context );
}

void GeomSucker::draw_start_vtx( IndexType vtx_idx ) const
{
    const Vec2f& vtx = network.vertices[vtx_idx];
    Vec2f        p1  = vtx + Vec2f( line_w * 1.5, line_w * 1.5 );
    Vec2f        p2  = vtx + Vec2f( -line_w * 1.5, line_w * 1.5 );
    Vec2f        p3  = vtx + Vec2f( -line_w * 1.5, -line_w * 1.5 );
    Vec2f        p4  = vtx + Vec2f( line_w * 1.5, -line_w * 1.5 );

    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_line_to( context, p4.x, -p4.y );
    cairo_close_path( context );
    cairo_stroke( context );
}

void GeomSucker::draw_end_vtx( IndexType vtx_idx ) const
{
    const Vec2f& vtx = network.vertices[vtx_idx];
    Vec2f        p1  = vtx + Vec2f( line_w * 1.5, line_w * 1.5 );
    Vec2f        p2  = vtx + Vec2f( -line_w * 1.5, line_w * 1.5 );
    Vec2f        p3  = vtx + Vec2f( -line_w * 1.5, -line_w * 1.5 );
    Vec2f        p4  = vtx + Vec2f( line_w * 1.5, -line_w * 1.5 );

    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_line_to( context, p4.x, -p4.y );
    cairo_fill( context );
}

void GeomSucker::draw_regular_left_vtx( IndexType vtx_idx ) const
{
    draw_vtx( vtx_idx );

    const Vec2f& vtx = network.vertices[vtx_idx];
    cairo_new_path( context );
    cairo_move_to( context, vtx.x - line_w * 2, -vtx.y - line_w * 5 );
    cairo_line_to( context, vtx.x - line_w * 2, -vtx.y + line_w * 5 );
    cairo_stroke( context );
}

void GeomSucker::draw_regular_right_vtx( IndexType vtx_idx ) const
{
    draw_vtx( vtx_idx );

    const Vec2f& vtx = network.vertices[vtx_idx];
    cairo_new_path( context );
    cairo_move_to( context, vtx.x + line_w * 2, -vtx.y - line_w * 5 );
    cairo_line_to( context, vtx.x + line_w * 2, -vtx.y + line_w * 5 );
    cairo_stroke( context );
}

void GeomSucker::draw_edge( const IndexType i_edge, float offset_rate ) const
{
    const HalfEdge& edge    = network.edges[i_edge];
    const Vec2f&    p_start = edge.get_vertex( network.vertices );
    const Vec2f&    p_end   = edge.get_next( network.edges ).get_vertex( network.vertices );
    const Vec2f&    p_prev  = edge.get_prev( network.edges ).get_vertex( network.vertices );
    const Vec2f&    p_next  = edge.get_next( network.edges ).get_next( network.edges ).get_vertex( network.vertices );

    Vec2f v_prev = p_start - p_prev;
    Vec2f v_curr = p_end - p_start;
    Vec2f v_next = p_next - p_end;

    float l_prev = v_prev.normalize();
    float l_curr = v_curr.normalize();
    float l_next = v_next.normalize();

    float offset = line_w * offset_rate;
    Vec2f p1     = p_start - v_prev * (l_prev / 3.0f) + v_curr * offset;
    Vec2f p2     = p_start + (v_curr - v_prev) * offset;
    Vec2f p3     = p_end   + (v_next - v_curr) * offset;
    Vec2f p4     = p_end   + v_next * (l_next / 3.0f) - v_curr * offset;

    // arrow
    float arrow_sz = line_w * 2;
    Vec2f ortho_next( -v_next.y, v_next.x );
    Vec2f p_arrow_root  = p4 - v_next * (arrow_sz * 3.0f);
    Vec2f p_arrow_root2 = p4 - v_next * (arrow_sz * 2.0f);
    Vec2f p_arrow1      = p_arrow_root + ortho_next * arrow_sz;
    Vec2f p_arrow2      = p_arrow_root + ortho_next * -arrow_sz;

    // do_drawing
    cairo_new_path( context );

    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_line_to( context, p4.x, -p4.y );

    cairo_set_line_width( context, line_w );
    cairo_stroke( context );

    cairo_move_to( context, p_arrow1.x, -p_arrow1.y );
    cairo_line_to( context, p4.x,            -p4.y );
    cairo_line_to( context, p_arrow2.x,      -p_arrow2.y );
    cairo_line_to( context, p_arrow_root2.x, -p_arrow_root2.y );
    cairo_fill( context );

    draw_vtx( edge.idx_vertex );
}

void GeomSucker::draw_edge_stack( const treecore::Array<IndexType>& edges ) const
{
    cairo_save( context );
    cairo_set_source_rgba( context, SUCKER_BLACK, 0.3 );
    for (IndexType edge_i : edges)
    {
        draw_edge( edge_i );
    }
    cairo_restore( context );
}

void GeomSucker::draw_trig_by_edge( IndexType i_edge1, IndexType i_edge2, IndexType i_edge3 ) const
{
    const Vec2f& p1 = network.get_edge_vertex( i_edge1 );
    const Vec2f& p2 = network.get_edge_vertex( i_edge2 );
    const Vec2f& p3 = network.get_edge_vertex( i_edge3 );

    cairo_new_path( context );
    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_fill( context );
}

void GeomSucker::draw_helper( IndexType i_edge, IndexType i_helper ) const
{
    cairo_save( context );

    draw_edge( i_edge, 1.0f );

    cairo_set_source_rgb( context, SUCKER_GREEN );
    draw_vtx( i_helper );

    cairo_restore( context );
}

void GeomSucker::draw_helper_change( IndexType i_edge, IndexType i_helper_old, IndexType i_helper_new ) const
{
    cairo_save( context );

    draw_edge( i_edge, 0.5f );

    cairo_set_source_rgba( context, SUCKER_BLUE, 0.33 );
    draw_edge( i_helper_old, 1.0f );

    cairo_set_source_rgba( context, SUCKER_GREEN, 0.66 );
    draw_edge( i_helper_new, 2.0f );

    cairo_restore( context );
}

void GeomSucker::text( const treecore::String& text, const Vec2f& position ) const
{
    cairo_move_to( context, position.x, -position.y );
    cairo_set_font_size( context, line_w * 5 );
    cairo_show_text( context, text.toRawUTF8() );
}

void GeomSucker::text( const treecore::String& text, IndexType i_vtx ) const
{
    this->text( text, network.vertices[i_vtx] );
}

OutlineSucker::OutlineSucker( const HalfOutline&      outline,
                              const treecore::String& title )
    : outline( outline )
{
    // get X and Y boundary
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();

    if (outline.outline.size() > 0)
    {
        for (const Vec2f& vtx : outline.outline)
        {
            if (x_min > vtx.x) x_min = vtx.x;
            if (y_min > vtx.y) y_min = vtx.y;
            if (x_max < vtx.x) x_max = vtx.x;
            if (y_max < vtx.y) y_max = vtx.y;
        }
    }
    else
    {
        x_min = -1.0f;
        x_max = 1.0f;
        y_min = -1.0f;
        y_max = 1.0f;
    }

    // get scales
    {
        float content_w = x_max - x_min;
        float content_h = y_max - y_min;

        if (content_w <= 0.0f) content_w = 2.0f;
        if (content_h <= 0.0f) content_h = 2.0f;

        if (x_min > -content_w / 5) x_min = -content_w / 5;
        if (x_max < content_w / 5) x_max = content_w / 5;
        if (y_min > -content_h / 5) y_min = -content_h / 5;
        if (y_max < content_h / 5) y_max = content_h / 5;

        width  = x_max - x_min;
        height = y_max - y_min;
    }

    line_w = std::sqrt( width * height ) / 200;

    // decide output file name
    String file_out;
    for (int i = 0;; i++)
    {
        file_out = String( pointer_sized_int( Process::getProcessID() ) ) + "_" + String( i ) + ".svg";

        if ( !File::getCurrentWorkingDirectory().getChildFile( file_out ).exists() )
            break;
    }

    // create cairo stuffs
    float scale = 1.0f;
    if (width * height < 40000.0f)
    {
        scale = sqrt( 40000.0f / width / height );
        printf( "too small, enlarge %f\n", scale );
    }

    float border = std::max( width / 2, height / 2 );
    surface = cairo_svg_surface_create( file_out.toRawUTF8(), (width + border * 2) * scale, (height + border * 2) * scale );
    context = cairo_create( surface );

    // set to initial state
    cairo_scale( context, scale, scale );
    cairo_translate( context, border, border );
    cairo_translate( context, -x_min, height + y_min );

    cairo_set_line_width( context, line_w );
    cairo_set_line_cap( context, CAIRO_LINE_CAP_ROUND );
    cairo_set_line_join( context, CAIRO_LINE_JOIN_ROUND );

    cairo_save( context );

    {
        //draw axis
        cairo_move_to( context, x_min, 0 );
        cairo_line_to( context, x_max, 0 );
        cairo_move_to( context, 0.0, -y_min );
        cairo_line_to( context, 0.0, -y_max );

        cairo_set_line_width( context, 2 * line_w );
        cairo_set_source_rgba( context, SUCKER_BLACK, 0.4 );
        cairo_stroke( context );

        // draw skeleton
        draw_outline( outline );

        // draw title
        cairo_set_font_size( context, line_w * 8 );
        cairo_text_extents_t ext;
        cairo_text_extents( context, title.toRawUTF8(), &ext );

        cairo_move_to( context, (x_min + x_max - ext.width) / 2, -(y_min + y_max - ext.height) / 2 );
        cairo_set_source_rgba( context, 0.0, 0.0, 0.0, 1.0 );
        cairo_show_text( context, title.toRawUTF8() );
    }

    cairo_restore( context );
}

OutlineSucker::~OutlineSucker()
{
    if (context)
        cairo_destroy( context );
    if (surface)
        cairo_surface_destroy( surface );
}

void OutlineSucker::draw_vtx( int i_outline ) const
{
    draw_vtx( outline.outline[i_outline] );
}

void OutlineSucker::draw_vtx( const Vec2f& vtx ) const
{
    cairo_new_path( context );
    cairo_arc( context, vtx.x, -vtx.y, line_w * 2, 0, 3.14159265 * 2 );
    cairo_fill( context );
}

void OutlineSucker::draw_vector( const Vec2f& start, const Vec2f& end ) const
{
    cairo_save( context );
    cairo_new_path( context );

    Vec2f v = end - start;
    v.normalize();

    float arrow_sz = line_w * 2;
    Vec2f ortho( -v.y, v.x );
    Vec2f p_arrow_root  = end - v * (arrow_sz * 3.0f);
    Vec2f p_arrow_root2 = end - v * (arrow_sz * 2.0f);
    Vec2f p_arrow1      = p_arrow_root + ortho * arrow_sz;
    Vec2f p_arrow2      = p_arrow_root + ortho * -arrow_sz;

    cairo_move_to( context, start.x, -start.y );
    cairo_line_to( context, end.x, -end.y );
    cairo_set_line_width( context, line_w );
    cairo_stroke( context );

    cairo_move_to( context, end.x, -end.y );
    cairo_line_to( context, p_arrow1.x,      -p_arrow1.y );
    cairo_line_to( context, p_arrow_root2.x, -p_arrow_root2.y );
    cairo_line_to( context, p_arrow2.x,      -p_arrow2.y );
    cairo_fill( context );

    cairo_restore( context );
}

void OutlineSucker::draw_unit_vector( const Vec2f& start, const Vec2f& v ) const
{
    Vec2f end = start + v * line_w * 20.0f;
    draw_vector( start, end );
}

void OutlineSucker::text( const treecore::String& content, int i_outline ) const
{
    draw_vtx( i_outline );
    text( content, outline.outline[i_outline] );
}

void OutlineSucker::text( const treecore::String& content, const Vec2f& position ) const
{
    cairo_move_to( context, position.x, -position.y );
    cairo_set_font_size( context, line_w * 5 );
    cairo_show_text( context, content.toRawUTF8() );
}

void OutlineSucker::draw_outline( const HalfOutline& content ) const
{
    if (content.outline.size() == 0)
        return;

    cairo_save( context );

    if (content.side > 0) cairo_set_source_rgb( context, SUCKER_ORANGE );
    else cairo_set_source_rgb( context, SUCKER_CYAN );

    // half outline skeleton
    cairo_new_path( context );

    cairo_set_line_width( context, line_w );

    for (int i = 0; i < content.outline.size(); i++)
    {
        const Vec2f& p = content.outline[i];
        cairo_line_to( context, p.x, -p.y );
    }

    cairo_stroke( context );

    // half outline joint IDs
    cairo_save( context );
    cairo_set_source_rgba( context, SUCKER_BLACK, 0.5f );
    cairo_set_font_size( context, line_w * 5 );
    for (int i = 0; i < content.outline.size(); i++)
    {
        const Vec2f& p = content.outline[i];
        cairo_move_to( context, p.x, -p.y );
        cairo_show_text( context, String( content.joint_ids[i] ).toRawUTF8() );
    }
    cairo_restore( context );

    // end mark
    const Vec2f& last    = content.outline.getLast();
    float        mark_sz = line_w * 5;

    cairo_new_path( context );
    if (content.sunken)
    {
        cairo_move_to( context, last.x - mark_sz, -(last.y - mark_sz) );
        cairo_line_to( context, last.x + mark_sz, -(last.y + mark_sz) );
        cairo_move_to( context, last.x - mark_sz, -(last.y + mark_sz) );
        cairo_line_to( context, last.x + mark_sz, -(last.y - mark_sz) );
    }
    else
    {
        cairo_arc( context, last.x, -last.y, line_w * 5, 0, treeface::PI * 2 );
        cairo_close_path( context );
    }

    cairo_stroke( context );

    cairo_restore( context );
}

void OutlineSucker::draw_trig( const Vec2f& p1, const Vec2f& p2, const Vec2f& p3 ) const
{
    cairo_new_path( context );
    cairo_move_to( context, p1.x, -p1.y );
    cairo_line_to( context, p2.x, -p2.y );
    cairo_line_to( context, p3.x, -p3.y );
    cairo_fill( context );
}

} // namespace treeface

#endif // SUCK_TREECORE_GEOMETRY
