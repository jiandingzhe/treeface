#include "treeface/graphics/guts/shapegenerator_guts.h"

#include <treecore/IntUtils.h>

#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"
#include "treeface/graphics/guts/geomsucker.h"
#include "treeface/graphics/guts/utils.h"

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
    for (int i_glyph = 0; i_glyph < glyphs.size(); i_glyph++)
    {
        const PathGlyph& glyph = glyphs[i_glyph];

        // first
        if (i_glyph == 0)
        {
            jassert(glyph.type == GLYPH_TYPE_LINE);
        }
        else
        {

        }
    }
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



} // namespace treeface
