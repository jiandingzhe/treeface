#ifndef TREEFACE_GEOM_SUCKER_H
#define TREEFACE_GEOM_SUCKER_H

#ifdef SUCK_TREECORE_GEOMETRY
#define SUCK_GEOM(...) __VA_ARGS__

#include <cairo.h>

#include <treecore/Array.h>

#include "treeface/graphics/halfedge.h"

#define SUCKER_RED      1.0f, 0.0f, 0.0f
#define SUCKER_ORANGE   1.0f, 0.5f, 0.0f
#define SUCKER_YELLOW   0.75f, 0.75f, 0.0f
#define SUCKER_GREEN    0.0f, 0.75f, 0.0f
#define SUCKER_CYAN     0.0f, 0.75f, 0.75f
#define SUCKER_BLUE     0.0f, 0.0f, 1.0f
#define SUCKER_MAGENTA  1.0f, 0.0f, 1.0f
#define SUCKER_BLACK    0.0f, 0.0f, 0.0f
#define SUCKER_WHITE    1.0f, 1.0f, 1.0f

namespace treeface
{

struct GeomSucker
{
    GeomSucker(const treecore::Array<Vec2f>& vertices,
               const treecore::Array<HalfEdge>& edges,
               const treecore::String& title = treecore::String::empty);
    ~GeomSucker();

    void rgba(float r, float g, float b, float a) { cairo_set_source_rgba(context, r, g, b, a); }
    void rgb (float r, float g, float b)          { cairo_set_source_rgb (context, r, g, b);    }

    void draw_vtx(IndexType vtx_idx) const;
    void draw_merge_vtx(IndexType vtx_idx) const;
    void draw_split_vtx(IndexType vtx_idx) const;
    void draw_start_vtx(IndexType vtx_idx) const;
    void draw_end_vtx(IndexType vtx_idx) const;
    void draw_regular_left_vtx(IndexType vtx_idx) const;
    void draw_regular_right_vtx(IndexType vtx_idx) const;
    void draw_edge(const IndexType i_edge, float offset_rate = 1.0f);
    void draw_edge_stack(const treecore::Array<IndexType>& edges);
    void draw_trig_by_edge(IndexType i_edge1, IndexType i_edge2, IndexType i_edge3);
    void draw_helper(IndexType i_edge, IndexType i_helper);
    void draw_helper_change(IndexType i_edge, IndexType i_helper_old, IndexType i_helper_new);
    void text(const treecore::String& text, const Vec2f& position);
    void text(const treecore::String& text, IndexType i_vtx);

    const treecore::Array<Vec2f>& vertices;
    const treecore::Array<HalfEdge>& edges;

    float width = std::numeric_limits<float>::signaling_NaN();
    float height = std::numeric_limits<float>::signaling_NaN();
    float line_w = std::numeric_limits<float>::signaling_NaN();

    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
};

}

#else
#  define SUCK_GEOM(...)
#endif // SUCK_TREECORE_GEOMETRY

#endif // TREEFACE_GEOM_SUCKER_H
