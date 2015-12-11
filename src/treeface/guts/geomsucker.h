#ifndef TREEFACE_GEOM_SUCKER_H
#define TREEFACE_GEOM_SUCKER_H

#ifdef SUCK_TREECORE_GEOMETRY

#include <cairo.h>

#include <treecore/Array.h>

#include "treeface/graphics/halfedge.h"

#define SUCKER_RED      1.0f, 0.0f, 0.0f
#define SUCKER_ORANGE   1.0f, 0.5f, 0.0f
#define SUCKER_YELLOW   1.0f, 1.0f, 0.0f
#define SUCKER_GREEN    0.0f, 1.0f, 0.0f
#define SUCKER_CYAN     0.0f, 1.0f, 1.0f
#define SUCKER_BLUE     0.0f, 0.0f, 1.0f
#define SUCKER_MAGENTA  1.0f, 0.0f, 1.0f
#define SUCKER_BLACK    0.0f, 0.0f, 0.0f
#define SUCKER_WHITE    1.0f, 1.0f, 1.0f

namespace treeface
{

struct GeomSucker
{
    GeomSucker(const treecore::Array<Vec2f>& vertices, const treecore::Array<HalfEdge>& skeleton);
    ~GeomSucker();

    void rgba(float r, float g, float b, float a) { cairo_set_source_rgba(context, r, g, b, a); }
    void rgb (float r, float g, float b)          { cairo_set_source_rgb (context, r, g, b);    }

    void draw_edge(const treecore::Array<HalfEdge>& edges, const IndexType i_edge, float offset_rate = 2.0f);

    const treecore::Array<Vec2f>& vertices;

    float line_w = std::numeric_limits<float>::signaling_NaN();

    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
};

}

#else
#  define SUCK_GEOM(...) __VA_ARGS__
#endif // SUCK_TREECORE_GEOMETRY

#endif // TREEFACE_GEOM_SUCKER_H
