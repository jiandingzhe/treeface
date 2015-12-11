#ifdef SUCK_TREECORE_GEOMETRY

#include "treeface/guts/geomsucker.h"

#include "cairo-svg.h"

#include <treecore/Thread.h>
#include <treecore/File.h>
#include <treecore/StringRef.h>

using namespace treecore;

namespace treeface
{

GeomSucker::GeomSucker(const treecore::Array<Vec2f>& vertices, const treecore::Array<HalfEdge>& skeleton): vertices(vertices)
{
    // get X and Y boundary
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();

    for (const Vec2f& vtx : vertices)
    {
        if (x_min > vtx.x) x_min = vtx.x;
        if (y_min > vtx.y) y_min = vtx.y;
        if (x_max < vtx.x) x_max = vtx.x;
        if (y_max < vtx.y) y_max = vtx.y;
    }

    // get scales
    float width = x_max - x_min;
    float height = y_max - y_min;

    line_w = std::sqrt(width*height) / 100;

    // decide output file name
    String file_out;
    for (int i = 0; ; i++)
    {
        file_out = String(pointer_sized_int(Thread::getCurrentThreadId())) + "_" + String(i) + ".svg";

        if (!File::getCurrentWorkingDirectory().getChildFile(file_out).exists())
            break;
    }

    // create cairo stuffs
    surface = cairo_svg_surface_create(file_out.toRawUTF8(), width * 1.2, height * 1.2);
    context = cairo_create(surface);

    cairo_translate(context, -x_min * 1.2, -y_min * 1.2);

    //draw axis
    cairo_move_to(context, x_min, 0.0);
    cairo_line_to(context, x_max, 0.0);
    cairo_move_to(context, 0.0, y_min);
    cairo_line_to(context, 0.0, y_max);

    cairo_set_line_width(context, 2 * line_w);
    cairo_set_source_rgba(context, SUCKER_BLACK, 0.8);
    cairo_stroke(context);

    // draw skeleton
    // assume skeleton is a single-connected loop
    cairo_new_path(context);
    for (int i = 0;;)
    {
        const HalfEdge& edge = skeleton[i];

        const Vec2f& vtx = edge.get_vertex(vertices);
        cairo_line_to(context, vtx.x, vtx.y);

        i = edge.idx_next_edge;
        if (i == 0) break;
    }

    cairo_set_line_width(context, line_w);
    cairo_stroke(context);

    cairo_set_source_rgba(context, 0.0, 0.0, 0.0, 1.0);
}

GeomSucker::~GeomSucker()
{
    if (context)
        cairo_destroy(context);

    if (surface)
        cairo_surface_destroy(surface);
}

void GeomSucker::draw_edge(const treecore::Array<HalfEdge>& edges, const IndexType i_edge, float offset_rate)
{
    const HalfEdge& edge = edges[i_edge];
    const Vec2f& p_start = edge.get_vertex(vertices);
    const Vec2f& p_end = edge.get_next(edges).get_vertex(vertices);
    const Vec2f& p_prev = edge.get_prev(edges).get_vertex(vertices);
    const Vec2f& p_next = edge.get_next(edges).get_next(edges).get_vertex(vertices);

    Vec2f v_prev = p_start - p_prev;
    Vec2f v_curr = p_end - p_start;
    Vec2f v_next = p_next - p_end;

    float l_prev = v_prev.normalize();
    float l_curr = v_curr.normalize();
    float l_next = v_next.normalize();

    Vec2f ortho_prev(-v_prev.y, v_prev.x);
    Vec2f ortho_curr(-v_curr.y, v_curr.x);
    Vec2f ortho_next(-v_next.y, v_next.x);

    ortho_prev.normalize();
    ortho_curr.normalize();
    ortho_next.normalize();

    float offset = line_w * offset_rate;
    Vec2f p1 = p_start - v_prev * (l_prev / 3.0f) + ortho_prev * offset;
    Vec2f p2 = p_start + (ortho_prev + ortho_curr) * offset;
    Vec2f p3 = p_end   + (ortho_curr + ortho_next) * offset;
    Vec2f p4 = p_end   + v_next * (l_next / 3.0f) + ortho_next * offset;

    // arrow
    float arrow_sz = line_w * 5;
    Vec2f p_arrow_root = p4 - v_next * arrow_sz;
    Vec2f p_arrow1 = p_arrow_root + ortho_next * arrow_sz;
    Vec2f p_arrow2 = p_arrow_root + ortho_next * -arrow_sz;

    // do_drawing
    cairo_new_path(context);

    cairo_move_to(context, p1.x, p1.y);
    cairo_line_to(context, p2.x, p2.y);
    cairo_line_to(context, p3.x, p3.y);
    cairo_line_to(context, p4.x, p4.y);

    cairo_move_to(context, p_arrow1.x, p_arrow1.y);
    cairo_line_to(context, p4.x, p4.y);
    cairo_line_to(context, p_arrow2.x, p_arrow2.y);

    cairo_set_line_width(context, line_w);
    cairo_stroke(context);
}

} // namespace treeface

#endif // SUCK_TREECORE_GEOMETRY
