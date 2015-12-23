#include "treeface/graphics/guts/shapegenerator_guts.h"

#include "treecore/IntUtils.h"

#include <list>

#include "treeface/graphics/guts/geomsucker.h"

using namespace treecore;

namespace treeface
{

typedef std::list<IndexType> IndexList;


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

    Array<IndexType> edge_monotone_by_y;
    Array<int16> edge_polygon_map;
    Array<VertexRole> monotone_edge_roles;
    int16 num_polygon = network_monotone.mark_monotone_polygons(edge_monotone_by_y, edge_polygon_map, monotone_edge_roles);

    network_monotone.triangulate_monotone_polygons(edge_monotone_by_y, edge_polygon_map, num_polygon, monotone_edge_roles, result_indices);
}


void _gen_line_join_round_(float half_w, const Vec2f& p_prev, const Vec2f& p_curr, const Vec2f& p_next,
                           Array<Vec2f>& result_vertices, Array<IndexType>& result_indices,
                           IndexType prev_tail_left, IndexType prev_tail_right,
                           IndexType& tail_left, IndexType& tail_right)
{
    Vec2f v1 = p_curr - p_prev;
    Vec2f v2 = p_next - p_curr;
    float l1 = v1.normalize();
    float l2 = v2.normalize();

    Vec2f ortho1(-v1.y, v1.x);
    Vec2f ortho2(-v2.y, v2.x);

    float sine = v1 ^ v2;

    // straight, just add two points and move on
    if (sine == 0)
    {
        tail_left = result_vertices.size();
        tail_right = tail_left + 1;

        result_vertices.add(p_curr + ortho1 * half_w);
        result_vertices.add(p_curr - ortho1 * half_w);

        result_indices.add(prev_tail_left);
        result_indices.add(prev_tail_right);
        result_indices.add(tail_left);

        result_indices.add(prev_tail_right);
        result_indices.add(tail_right);
        result_indices.add(tail_left);
    }
    else
    {
        float cosine = v1 * v2;

        if (sine > 0)
        {

        }
        // turn right
        else if (sine < 0)
        {

        }
    }
}

void SubPath::stroke_complex(treecore::Array<Vec2f>& result_vertices,
                             treecore::Array<IndexType>& result_indices,
                             LineCapStyle cap_style,
                             LineJoinStyle join_style,
                             float line_width) const
{
    IndexType idx_start_left  = std::numeric_limits<IndexType>::max();
    IndexType idx_start_right = std::numeric_limits<IndexType>::max();
    IndexType idx_prev_left  = std::numeric_limits<IndexType>::max();
    IndexType idx_prev_right = std::numeric_limits<IndexType>::max();

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
