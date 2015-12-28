#include "treeface/graphics/guts/shapegenerator_guts.h"

#include <treecore/IntUtils.h>

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"
#include "treeface/graphics/guts/geomsucker.h"

using namespace treecore;

namespace treeface
{

struct EdgeRebuildTmp
{
    EdgeRebuildTmp(IndexType idx_begin, IndexType idx_end, const Array<Vec2f>& vertices)
        : idx_begin(idx_begin)
        , idx_end(idx_end)
        , bound(vertices[idx_begin], vertices[idx_end])
    {
        direct = vertices[idx_end] - vertices[idx_begin];
        length = direct.length();
        direct /= length;
    }

    IndexType idx_begin;
    IndexType idx_end;
    float length;
    Vec2f direct;

    BBox2f bound;

    Array<IndexType> idx_interm;
};

struct IntermVtxSorter
{
    IntermVtxSorter(const EdgeRebuildTmp& edge, const Array<Vec2f>& vertices)
        : edge(edge)
        , vertices(vertices)
    {
    }

    int compareElements (IndexType a, IndexType b) const noexcept
    {
        const Vec2f& start = vertices[edge.idx_begin];
        float proj_a = (vertices[a] - start) * edge.direct;
        float proj_b = (vertices[b] - start) * edge.direct;

        if (proj_a < proj_b)
            return -1;
        else if (proj_a == proj_b)
            return 0;
        else
            return 1;
    }

    const EdgeRebuildTmp& edge;
    const Array<Vec2f>& vertices;
};

double clockwise_accum(const Array<Vec2f>& vertices, IndexType i_begin, IndexType i_end) noexcept
{
    jassert(i_end - i_begin > 2);
    jassert(i_end <= vertices.size());

    double sum = 0.0;

    for (int i = i_begin + 1; i != i_end; i++)
    {
        const Vec2f& p_curr = vertices[i];
        const Vec2f& p_prev = vertices[i - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    {
        const Vec2f& p_curr = vertices[i_begin];
        const Vec2f& p_prev = vertices[i_end - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    return sum;
}


///
/// \brief do triangulate
/// \param idx_list  a list of all indices
/// \param vertices  vertex list that all index values should be valid in it
/// \param indices   result triangle indices will be filled to here
///
void _triangulate_(const HalfEdgeNetwork& network, Array<IndexType>& result_indices)
{
    HalfEdgeNetwork network_monotone(network.vertices);
    network.partition_polygon_monotone(network_monotone);

    Array<VertexRole> monotone_edge_roles;
    network_monotone.get_edge_role(monotone_edge_roles);

    Array<IndexType> edge_monotone_by_y;
    network_monotone.get_edge_vertical_order(monotone_edge_roles, edge_monotone_by_y);

    Array<int16> edge_polygon_map;
    int16 num_polygon = network_monotone.mark_polygons(edge_polygon_map);

    SUCK_GEOM(for (int i_poly = 0; i_poly < num_polygon; i_poly++)               )
    SUCK_GEOM({                                                                  )
    SUCK_GEOM(    GeomSucker sucker(network_monotone, "monotone polygon #"+String(i_poly)); )
    SUCK_GEOM(    int idx = 0;                                                   )
    SUCK_GEOM(    for (IndexType i_edge = 0; i_edge < network_monotone.edges.size(); i_edge++)    )
    SUCK_GEOM(    {                                                              )
    SUCK_GEOM(        if (edge_polygon_map[i_edge] != i_poly) continue;          )
    SUCK_GEOM(        IndexType i_vtx = network_monotone.edges[i_edge].idx_vertex;                )
    SUCK_GEOM(        sucker.rgba(SUCKER_BLACK, 0.4);                            )
    SUCK_GEOM(        sucker.draw_edge(i_edge);                                  )
    SUCK_GEOM(        sucker.rgb(SUCKER_BLACK);                                  )
    SUCK_GEOM(        sucker.draw_roled_vtx(i_vtx, monotone_edge_roles[i_edge]); )
    SUCK_GEOM(        idx++;                                                     )
    SUCK_GEOM(    }                                                              )
    SUCK_GEOM(}                                                                  )

    network_monotone.triangulate_monotone_polygons(edge_monotone_by_y, edge_polygon_map, num_polygon, monotone_edge_roles, result_indices);
}



void SubPath::stroke_complex(treecore::Array<ShapeGenerator::StrokeVertex>& result_vertices,
                                  treecore::Array<IndexType>& result_indices,
                                  ShapeGenerator::StrokeStyle style) const
{

}

void ShapeGenerator::Guts::triangulate(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices)
{
    // do segment on all subpath
    Array<IndexType> subpath_begin_indices;
    for (const SubPath& subpath : subpaths)
    {
        IndexType idx_begin = IndexType(result_vertices.size());
        subpath_begin_indices.add(idx_begin);

        jassert(subpath.glyphs.size() > 1);

        for (int i_glyph = 0; i_glyph < subpath.glyphs.size(); i_glyph++)
        {
            const PathGlyph& glyph = subpath.glyphs[i_glyph];

            if (i_glyph == 0)
            {
                jassert(glyph.type == GLYPH_TYPE_LINE);
                result_vertices.add(glyph.end);
            }
            else
            {
                switch(glyph.type)
                {
                case GLYPH_TYPE_LINE:
                    result_vertices.add(glyph.end);
                    break;
                case GLYPH_TYPE_ARC:
                    //segment_arc(result_vertices, glyph);
                    break;
                case GLYPH_TYPE_BESSEL3:
                    //segment_bessel3(result_vertices, glyph);
                    break;
                case GLYPH_TYPE_BESSEL4:
                    //segment_bessel4(result_vertices, glyph);
                    break;
                default:
                    jassertfalse; abort();
                }
            }
        }
    }

    jassert(subpath_begin_indices.size() == subpaths.size());

    // determine global clockwise
    double clw_accum_global = 0.0;
    for (int i_subpath = 0; i_subpath < subpaths.size(); i_subpath++)
    {
        IndexType i_begin = subpath_begin_indices[i_subpath];
        IndexType i_end = i_subpath == subpaths.size() - 1
                ? result_vertices.size()
                : subpath_begin_indices[i_subpath + 1];
        clw_accum_global += clockwise_accum(result_vertices, i_begin, i_end);
    }

    // generate half edges
    HalfEdgeNetwork network(result_vertices);
    network.build_half_edges(subpath_begin_indices, clw_accum_global < 0.0);

    // do triangulation
    _triangulate_(network, result_indices);
}

int segment_arc(const PathGlyph& glyph, treecore::Array<Vec2f>& result_vertices)
{
    jassert(result_vertices.size() > 0);
    const int n_vtx_old = result_vertices.size();

    // roll to 0-360 degree
    float angle_use = glyph.arc.angle;
    while (angle_use < 0.0f) angle_use += 2.0f * PI;
    while (angle_use > 2.0f * PI) angle_use -= 2.0f * PI;

    // determine step
    int num_step = std::round(angle_use / PI * 32);
    if (num_step < 5 && angle_use > 0.0f) num_step = 5;
    float step = angle_use / num_step;

    // rotate from end point
    Mat2f rot;
    if (glyph.arc.is_cclw) rot.set_rotate(step);
    else                   rot.set_rotate(-step);

    // generate points
    Vec2f v = result_vertices.getLast() - glyph.arc.center;

    for (int i = 1; i < num_step; i--)
    {
        v = rot * v;
        result_vertices.add(glyph.arc.center + v);
    }

    result_vertices.add(glyph.end);

    return result_vertices.size() - n_vtx_old;
}

#define INTERPO(_a_, _frac_, _b_) _a_ * _frac_ + _b_ * (1.0f - _frac_)

inline Vec2f _bessel3_interpo_(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, float frac)
{
    Vec2f tmp1 = INTERPO(p1, frac, p2);
    Vec2f tmp2 = INTERPO(p2, frac, p3);
    return INTERPO(tmp1, frac, tmp2);
}

inline Vec2f _bessel4_interpo_(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3, const Vec2f& p4, float frac)
{
    Vec2f tmp_a1 = INTERPO(p1, frac, p2);
    Vec2f tmp_a2 = INTERPO(p2, frac, p3);
    Vec2f tmp_a3 = INTERPO(p3, frac, p4);

    Vec2f tmp_b1 = INTERPO(tmp_a1, frac, tmp_a2);
    Vec2f tmp_b2 = INTERPO(tmp_a2, frac, tmp_a3);

    return INTERPO(tmp_b1, frac, tmp_b2);
}

int segment_bessel3(const PathGlyph& glyph, treecore::Array<Vec2f>& result_vertices)
{
    jassert(result_vertices.size() > 0);
    const int n_vtx_old = result_vertices.size();

    float step = 1.0f / 32;

    const Vec2f& prev_end = result_vertices.getLast();

    for (float frac = 0.0f; frac < 1.0f;)
    {
        const Vec2f& vtx_prev = result_vertices.getLast();
        float step_use = step;

        for (;;)
        {
            Vec2f vtx_estimator = _bessel3_interpo_(prev_end, glyph.bessel3.ctrl, glyph.end, frac + step_use / 2);
            Vec2f vtx_curr      = _bessel3_interpo_(prev_end, glyph.bessel3.ctrl, glyph.end, frac + step_use);
            jassert(vtx_prev != vtx_estimator);
            jassert(vtx_estimator != vtx_curr);
            jassert(vtx_curr != vtx_prev);

            float cosine = (vtx_curr - vtx_estimator) * (vtx_estimator - vtx_prev);
            if (cosine > 0.6f)
            {
                result_vertices.add(vtx_curr);
                break;
            }

            step_use *= 0.67f;
        }
    }

    result_vertices.add(glyph.end);

    return result_vertices.size() - n_vtx_old;
}

int segment_bessel4(const PathGlyph& glyph, treecore::Array<Vec2f>& result_vertices)
{
    jassert(result_vertices.size() > 0);
    const int n_vtx_old = result_vertices.size();

    float step = 1.0f / 32;

    const Vec2f& prev_end = result_vertices.getLast();

    for (float frac = 0.0f; frac < 1.0f;)
    {
        const Vec2f& vtx_prev = result_vertices.getLast();
        float step_use = step;

        for (;;)
        {
            Vec2f vtx_estimator = _bessel4_interpo_(prev_end, glyph.bessel4.ctrl1, glyph.bessel4.ctrl2, glyph.end, frac + step_use / 2);
            Vec2f vtx_curr      = _bessel4_interpo_(prev_end, glyph.bessel4.ctrl1, glyph.bessel4.ctrl2, glyph.end, frac + step_use);
            jassert(vtx_prev != vtx_estimator);
            jassert(vtx_estimator != vtx_curr);
            jassert(vtx_curr != vtx_prev);

            float cosine = (vtx_curr - vtx_estimator) * (vtx_estimator - vtx_prev);
            if (cosine > 0.6f)
            {
                result_vertices.add(vtx_curr);
                break;
            }

            step_use *= 0.67f;
        }
    }

    result_vertices.add(glyph.end);

    return result_vertices.size() - n_vtx_old;
}


} // namespace treeface
