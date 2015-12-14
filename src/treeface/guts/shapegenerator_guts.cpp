#include "treeface/guts/shapegenerator_guts.h"

#include "treecore/IntUtils.h"

#include <list>

#include "treeface/guts/geomsucker.h"

using namespace treecore;

namespace treeface
{

typedef std::list<IndexType> IndexList;

bool _is_below_(const Vec2f& a, const Vec2f& b)
{
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

struct HalfEdgeIndexVerticalSorter
{
    HalfEdgeIndexVerticalSorter(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges)
        : vertices(vertices)
        , edges(edges)
    {
    }

    int compareElements (IndexType a, IndexType b) const noexcept
    {
        const Vec2f& p1 = vertices[edges[a].idx_vertex];
        const Vec2f& p2 = vertices[edges[b].idx_vertex];

        if (p1.y < p2.y)
        {
            return 1;
        }
        else if (p1.y == p2.y)
        {
            if (p1.x < p2.x)
                return 1;
            else if (p1.x == p2.x)
                return 0;
            else
                return -1;
        }
        else
        {
            return -1;
        }
    }

    const Array<Vec2f>& vertices;
    const Array<HalfEdge>& edges;
};

struct HelpEdgeStore
{
    HelpEdgeStore(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges)
        : vertices(vertices)
        , edges(edges)
    {
        jassert(vertices.size() == edges.size());
        edge_helper_map.resize(edges.size());
        for (int i = 0; i < edge_helper_map.size(); i++)
            edge_helper_map[i] = -1;
    }

    void add(IndexType edge_idx)
    {
        SUCK_GEOM(GeomSucker sucker(vertices, edges, "add edge to search store"); )
        SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.5);                                 )
        SUCK_GEOM(for (IndexType idx : i_left_edges)                              )
        SUCK_GEOM(    sucker.draw_edge(idx);                                      )
        SUCK_GEOM(sucker.rgba(SUCKER_GREEN, 0.7);                                 )
        SUCK_GEOM(sucker.draw_edge(edge_idx, 4.0f);                               )

        i_left_edges.add(edge_idx);
    }

    void remove(IndexType edge_idx)
    {
        i_left_edges.removeFirstMatchingValue(edge_idx);

        SUCK_GEOM(GeomSucker sucker(vertices, edges, "remove edge from search store"); )
        SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.5);                                      )
        SUCK_GEOM(for (IndexType idx : i_left_edges)                                   )
        SUCK_GEOM(    sucker.draw_edge(idx);                                           )
        SUCK_GEOM(sucker.rgba(SUCKER_RED, 0.7);                                        )
        SUCK_GEOM(sucker.draw_edge(edge_idx, 4.0f);                                    )
    }

    IndexType get_edge_helper_edge(IndexType edge_idx)
    {
        IndexType helper_idx = edge_helper_map[edge_idx];
        return helper_idx;
    }

    IndexType find_nearest_left_edge(const Vec2f& position)
    {
        IndexType result = std::numeric_limits<IndexType>::max();
        float min_x_dist = std::numeric_limits<float>::max();

        for (IndexType i_edge : i_left_edges)
        {
            const HalfEdge& edge = edges[i_edge];
            const Vec2f& p1 = edge.get_vertex(vertices);
            const Vec2f& p2 = edge.get_next(edges).get_vertex(vertices);

            jassert(p2.y <= p1.y);

            if (p1.y < position.y || p2.y > position.y) continue;

            Vec2f edge_v = p2 - p1;
            if (edge_v.y == 0)
                continue;

            float slope_inv = edge_v.x / edge_v.y;
            float dy = position.y - p1.y;
            float dx = dy * slope_inv;
            float cross_point_x = p1.x + dx;

            float x_dist = position.x - cross_point_x;
            if (x_dist <= 0)
                continue;

            if (x_dist < min_x_dist)
            {
                result = i_edge;
                min_x_dist = x_dist;
            }
        }

        jassert(min_x_dist > 0);
        jassert(std::isfinite(min_x_dist));
        jassert(result != std::numeric_limits<IndexType>::max());
        return result;
    }

    Array<IndexType> edge_helper_map; // edge idx => helper edge idx
    Array<IndexType> i_left_edges;
    const Array<Vec2f>& vertices;
    const Array<HalfEdge>& edges;
};

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

bool is_counter_clockwise(const Array<Vec2f>& vertices) noexcept
{
    jassert(vertices.size() > 2);

    float sum = 0.0f;

    for (int i = 1; i < vertices.size(); i++)
    {
        const Vec2f& p_curr = vertices[i];
        const Vec2f& p_prev = vertices[i - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    {
        const Vec2f& p_curr = vertices.getFirst();
        const Vec2f& p_prev = vertices.getLast();
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    return sum < 0;
}

bool _edge_is_merge_vertex_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, IndexType i_edge)
{
    const HalfEdge& edge = edges[i_edge];

    const Vec2f& vtx_prev = edge.get_prev(edges).get_vertex(vertices);
    const Vec2f& vtx_curr = edge.get_vertex(vertices);
    const Vec2f& vtx_next = edge.get_next(edges).get_vertex(vertices);

    Vec2f helper_v1 = vtx_curr - vtx_prev;
    Vec2f helper_v2 = vtx_next - vtx_curr;

    return
            ! _is_below_(vtx_prev, vtx_curr) &&
            _is_below_(vtx_curr, vtx_next) &&
            ! is_convex(helper_v1, helper_v2);
}

void _make_connect_(const Array<Vec2f>& vertices, Array<HalfEdge>& edges, IndexType i_edge1, IndexType i_edge2)
{
    IndexType i_edge_1_2 = edges.size();
    IndexType i_edge_2_1 = i_edge_1_2 + 1;

    HalfEdge& edge1 = edges[i_edge1];
    IndexType i_edge1_prev = edge1.idx_prev_edge;
    HalfEdge& edge1_prev = edges[i_edge1_prev];

    HalfEdge& edge2 = edges[i_edge2];
    IndexType i_edge2_prev = edge2.idx_prev_edge;
    HalfEdge& edge2_prev = edges[i_edge2_prev];

    SUCK_GEOM({)
    SUCK_GEOM(    GeomSucker sucker(vertices, edges, "going to link");   )
    SUCK_GEOM(    sucker.draw_edge(i_edge1);           )
    SUCK_GEOM(    sucker.draw_edge(i_edge2);           )
    SUCK_GEOM(    sucker.rgb(SUCKER_BLUE);             )
    SUCK_GEOM(    sucker.draw_edge(i_edge1_prev, 3.0f);)
    SUCK_GEOM(    sucker.draw_edge(i_edge2_prev, 3.0f);)
    SUCK_GEOM(})

    // pre-calculate the index of newly added half edges
    // modify existing edges' connection
    jassert(edge1_prev.idx_next_edge == i_edge1);
    jassert(edge2_prev.idx_next_edge == i_edge2);
    edge1.idx_prev_edge = i_edge_2_1;
    edge2.idx_prev_edge = i_edge_1_2;
    edge1_prev.idx_next_edge = i_edge_1_2;
    edge2_prev.idx_next_edge = i_edge_2_1;

    // create new edge
    edges.add(HalfEdge{edge1.idx_vertex,
                       i_edge1_prev,
                       i_edge2,
                       i_edge_2_1});

    edges.add(HalfEdge{edge2.idx_vertex,
                       i_edge2_prev,
                       i_edge1,
                       i_edge_1_2});

    SUCK_GEOM({)
    SUCK_GEOM(    GeomSucker sucker(vertices, edges, "linked");   )
    SUCK_GEOM(    sucker.draw_edge(i_edge1);            )
    SUCK_GEOM(    sucker.draw_edge(i_edge2);            )
    SUCK_GEOM(    sucker.rgb(SUCKER_BLUE);              )
    SUCK_GEOM(    sucker.draw_edge(i_edge1_prev, 3.0f); )
    SUCK_GEOM(    sucker.draw_edge(i_edge2_prev, 3.0f); )
    SUCK_GEOM(    sucker.rgb(SUCKER_RED);               )
    SUCK_GEOM(    sucker.draw_edge(i_edge_1_2, 4.0f);   )
    SUCK_GEOM(    sucker.draw_edge(i_edge_2_1, 4.0f);   )
    SUCK_GEOM(})
}

void partition_polygon_monotone(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges_input, Array<HalfEdge>& edges_result)
{
    edges_result = edges_input;

    int num_edge_orig = edges_input.size();

    SUCK_GEOM({                                     );
    SUCK_GEOM(    GeomSucker(vertices, edges_input););
    SUCK_GEOM(}                                     );

    // sort by vertical position
    Array<IndexType> edge_idx_by_y;
    edge_idx_by_y.resize(edges_input.size());
    for (int i = 0; i < num_edge_orig; i++)
        edge_idx_by_y[i] = i;

    {
        HalfEdgeIndexVerticalSorter sorter(vertices, edges_input);
        edge_idx_by_y.sort(sorter);
    }

    SUCK_GEOM(for (int i = 0; i < edge_idx_by_y.size(); i++)                                   )
    SUCK_GEOM({                                                                   )
    SUCK_GEOM(    IndexType idx = edge_idx_by_y[i];                               )
    SUCK_GEOM(    GeomSucker sucker(vertices, edges_input);                       )
    SUCK_GEOM(    sucker.draw_edge(idx);                               )
    SUCK_GEOM(    const Vec2f& pos = edges_input[idx].get_vertex(vertices);         )
    SUCK_GEOM(    sucker.text("sorted #"+String(i)+" at "+String(pos.y), pos);     )
    SUCK_GEOM(}                                                                   )

    // traverse all vertices
    HelpEdgeStore helper_store(vertices, edges_input);

    for (IndexType i_edge_curr : edge_idx_by_y)
    {
        const HalfEdge& edge_curr = edges_input[i_edge_curr];
        const Vec2f& vtx_curr = edge_curr.get_vertex(vertices);
        const Vec2f& vtx_prev = edge_curr.get_prev(edges_input).get_vertex(vertices);
        const Vec2f& vtx_next = edge_curr.get_next(edges_input).get_vertex(vertices);
        Vec2f v1 = vtx_curr - vtx_prev;
        Vec2f v2 = vtx_next - vtx_curr;

        if (_is_below_(vtx_prev, vtx_curr))
        {
            //
            // regular vertex of "right" part
            //
            if (_is_below_(vtx_curr, vtx_next))
            {
                SUCK_GEOM({)
                SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);                     )
                SUCK_GEOM(    sucker.text("regular right vtx", edge_curr.idx_vertex);        )
                SUCK_GEOM(    sucker.draw_regular_right_vtx(edge_curr.idx_vertex);           )
                SUCK_GEOM(})

                //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i) .
                IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
                IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);

                SUCK_GEOM({)
                SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
                SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_edge_of_left_edge_helper);       )
                SUCK_GEOM(})

                //PSEUDOCODE if helper(e(j)) is a merge vertex
                //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
                if (_edge_is_merge_vertex_(vertices, edges_input, i_edge_of_left_edge_helper))
                {
                    _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
                }

                //PSEUDOCODE helper(e(j)) = v(i)
                helper_store.edge_helper_map[i_left_edge] = i_edge_curr;
                SUCK_GEOM({)
                SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "set helper to current vtx");          )
                SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr); )
                SUCK_GEOM(})
            }
            else
            {
                //
                // start vertex
                //
                if (is_convex(v1, v2))
                {
                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
                    SUCK_GEOM(    sucker.text("start vtx", edge_curr.idx_vertex); )
                    SUCK_GEOM(    sucker.draw_start_vtx(edge_curr.idx_vertex);    )
                    SUCK_GEOM(})

                    //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i)
                    helper_store.add(i_edge_curr);
                    helper_store.edge_helper_map[i_edge_curr] = i_edge_curr;
                }

                //
                // split vertex
                //
                else
                {
                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
                    SUCK_GEOM(    sucker.text("split vtx", edge_curr.idx_vertex); )
                    SUCK_GEOM(    sucker.draw_split_vtx(edge_curr.idx_vertex);    )
                    SUCK_GEOM(})

                    //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i)
                    IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
                    IndexType i_helper_edge = helper_store.get_edge_helper_edge(i_left_edge);

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
                    SUCK_GEOM(    sucker.draw_edge(i_left_edge);                                     )
                    SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                    )
                    SUCK_GEOM(    sucker.draw_edge(i_helper_edge, 3.0f);                      )
                    SUCK_GEOM(})

                    //PSEUDOCODE Insert the diagonal connecting v(i) to helper(e(j)) in D
                    _make_connect_(vertices, edges_result, i_edge_curr, i_helper_edge);

                    //PSEUDOCODE helper(e(j)) = v(i)
                    helper_store.edge_helper_map[i_left_edge] = i_edge_curr;

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "set helper to current vtx"); )
                    SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_helper_edge, i_edge_curr);     )
                    SUCK_GEOM(})

                    //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i).
                    helper_store.add(i_edge_curr);
                    helper_store.edge_helper_map[i_edge_curr] = i_edge_curr;
                }
            }
        }
        else
        {
            if (_is_below_(vtx_curr, vtx_next))
            {
                //
                // end vertex
                //
                if (is_convex(v1, v2))
                {
                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);    )
                    SUCK_GEOM(    sucker.text("end vtx", edge_curr.idx_vertex); )
                    SUCK_GEOM(    sucker.draw_end_vtx(edge_curr.idx_vertex);    )
                    SUCK_GEOM(})

                    //PSEUDOCODE if helper(e(i-1)) is a merge vertex
                    //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper( e(i-1) ) in D
                    const  IndexType i_edge_of_prev_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
                    jassert(i_edge_of_prev_helper < edges_input.size());

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "helper of prev"); )
                    SUCK_GEOM(    sucker.draw_edge(edge_curr.idx_prev_edge);)
                    SUCK_GEOM(    sucker.rgba(SUCKER_GREEN, 0.5);)
                    SUCK_GEOM(    sucker.draw_edge(i_edge_of_prev_helper);)
                    SUCK_GEOM(})

                    if (_edge_is_merge_vertex_(vertices, edges_input, i_edge_of_prev_helper))
                    {
                        _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_helper);
                    }

                    //PSEUDOCODE Delete e(i-1) from T
                    helper_store.remove(edge_curr.idx_prev_edge);
                }

                //
                // merge vertex
                //
                else
                {
                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
                    SUCK_GEOM(    sucker.text("merge vtx", edge_curr.idx_vertex); )
                    SUCK_GEOM(    sucker.draw_merge_vtx(edge_curr.idx_vertex);    )
                    SUCK_GEOM(})

                    //PSEUDOCODE if helper(e(i-1)) is a merge vertex
                    //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
                    const IndexType i_edge_of_prev_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
                    jassert(i_edge_of_prev_helper < edges_input.size());

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "prev edge and helper");  )
                    SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_edge_of_prev_helper);  )
                    SUCK_GEOM(})

                    if (_edge_is_merge_vertex_(vertices, edges_input, i_edge_of_prev_helper))
                    {
                        _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_helper);
                    }

                    //PSEUDOCODE Delete e(i-1) from T.
                    helper_store.remove(edge_curr.idx_prev_edge);

                    //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i).
                    IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
                    IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
                    SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_edge_of_left_edge_helper);       )
                    SUCK_GEOM(})

                    //PSEUDOCODE if helper(e(j)) is a merge vertex
                    //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
                    if (_edge_is_merge_vertex_(vertices, edges_input, i_edge_of_left_edge_helper))
                    {
                        _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
                    }

                    //PSEUDOCODE helper(e(j)) = v(i)
                    helper_store.edge_helper_map[i_left_edge] = i_edge_curr;

                    SUCK_GEOM({)
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "change helper edge"); )
                    SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr);)
                    SUCK_GEOM(})
                }
            }

            //
            // regular vertex of "left" part
            //
            else
            {
                SUCK_GEOM({)
                SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);             )
                SUCK_GEOM(    sucker.text("regular left vtx", edge_curr.idx_vertex); )
                SUCK_GEOM(    sucker.draw_regular_left_vtx(edge_curr.idx_vertex);    )
                SUCK_GEOM(})

                //PSEUDOCODE if helper(e(i-1)) is a merge vertex
                //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
                const IndexType i_edge_of_prev_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
                jassert(i_edge_of_prev_helper < edges_input.size());

                if (_edge_is_merge_vertex_(vertices, edges_input, i_edge_of_prev_helper))
                {
                    _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_helper);
                }

                //PSEUDOCODE Delete e(i-1) from T.
                helper_store.remove(edge_curr.idx_prev_edge);

                //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i).
                helper_store.add(i_edge_curr);
                helper_store.edge_helper_map[i_edge_curr] = i_edge_curr;
            }
        }
    }
}

void _trangulate_monotone_polygons_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, Array<IndexType>& result_indices)
{

}

///
/// \brief do triangulate
/// \param idx_list  a list of all indices
/// \param vertices  vertex list that all index values should be valid in it
/// \param indices   result triangle indices will be filled to here
///
void _triangulate_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, Array<IndexType>& result_indices)
{
    Array<HalfEdge> edges_monotone;
    partition_polygon_monotone(vertices, edges, edges_monotone);
    _trangulate_monotone_polygons_(vertices, edges_monotone, result_indices);
}

void SubPath::triangulate_simple(Array<Vec2f>& result_vertices, Array<IndexType>& result_indices) const
{
    if (vertices.size() < 3) return;
    const IndexType subpath_idx_first = IndexType(result_vertices.size());
    const IndexType subpath_idx_last = subpath_idx_first + vertices.size() - 1;

    // assign global vertex index
    // generate vertex connection map
    Array<HalfEdge> edges;
    IndexType total_num_edge = vertices.size();
    bool ccw = is_counter_clockwise(vertices);

    for (int i = 0; i < vertices.size(); i++)
    {
        IndexType i_vtx = IndexType(result_vertices.size());
        result_vertices.add(vertices[i]);

        IndexType i_edge_curr = IndexType(edges.size());

        IndexType i_prev = (i == 0
                            ? IndexType(total_num_edge - 1)
                            : i_edge_curr - 1);

        IndexType i_next = (i == vertices.size() - 1
                            ? 0
                            : i_edge_curr + 1);

        if (ccw)
            edges.add(HalfEdge{i_vtx, i_prev, i_next, std::numeric_limits<IndexType>::max()});
        else
            edges.add(HalfEdge{i_vtx, i_next, i_prev, std::numeric_limits<IndexType>::max()});
    }

    jassert(edges.size() == vertices.size());

    // do triangulate
    _triangulate_(result_vertices, edges, result_indices);
}

void SubPath::triangulate_complex(Array<Vec2f>& result_vertices, Array<IndexType>& result_indices) const
{
    if (vertices.size() < 3) return;

    const IndexType subpath_idx_begin = IndexType(result_vertices.size());

    //
    // add subpath's vertices to whole list
    // assign indices in whole list
    // generate edges
    //
    Array<EdgeRebuildTmp> orig_edges;

    if (is_counter_clockwise(vertices))
    {
        printf("is counter clockwise\n");
        // subpath is counter-clockwise, add in order
        for (int i = 0; i < vertices.size(); i++)
        {
            result_vertices.add(vertices[i]);

            if (i > 0)
            {
                IndexType idx_to = IndexType(result_vertices.size() - 1);
                IndexType idx_from = idx_to - 1;
                orig_edges.add(EdgeRebuildTmp(idx_from, idx_to, result_vertices));
            }
        }
    }
    else
    {
        printf("is clockwise, use in reverse\n");
        // subpath is clockwise, add in reversed order
        for (int i = vertices.size() - 1; i >= 0; i--)
        {
            result_vertices.add(vertices[i]);

            if (i != vertices.size() - 1)
            {
                IndexType idx_to = IndexType(result_vertices.size() - 1);
                IndexType idx_from = idx_to - 1;
                orig_edges.add(EdgeRebuildTmp(idx_from, idx_to, result_vertices));
            }
        }
    }

    {
        IndexType idx_last = IndexType(result_vertices.size() - 1);
        orig_edges.add(EdgeRebuildTmp(idx_last, subpath_idx_begin, result_vertices));
    }

    jassert(orig_edges.size() == vertices.size());

    // cross test between all edge pairs
    // record cross point to subdivide those edges
    for (int i = 0; i < orig_edges.size(); i++)
    {
        EdgeRebuildTmp& edge1 = orig_edges[i];

        const Vec2f& p1 = result_vertices[edge1.idx_begin];
        const Vec2f& p2 = result_vertices[edge1.idx_end];
        const Vec2f v1 = p2 - p1;
        const float cross12 = p1 ^ p2;

        for (int j = i; j < orig_edges.size(); j++)
        {
            EdgeRebuildTmp& edge2 = orig_edges[j];

            const Vec2f& p3 = result_vertices[edge2.idx_begin];
            const Vec2f& p4 = result_vertices[edge2.idx_end];
            const Vec2f v2 = p4 - p3;
            const float cross34 = p3 ^ p4;

            float epsilon = (edge1.length + edge2.length) / 1000;

            // bbox test
            if (!(edge1.bound ^ edge2.bound))
                continue;

            // line parallel test
            float denm = v1 ^ v2;
            if (denm == 0.0f)
                continue;

            // TODO calculate cross point
            Vec2f cross((cross34 * v1.x - cross12 * v2.x) / denm,
                        (cross34 * v1.y - cross12 * v2.y) / denm);

            // determine cross point is inside two edges
            float proj1 = (cross - p1) * edge1.direct;
            if (proj1 < epsilon || proj1 > edge1.length - epsilon)
                continue;

            float proj2 = (cross - p3) * edge2.direct;
            if (proj2 < epsilon || proj2 > edge2.length - epsilon)
                continue;

            // finally store the cross point
            result_vertices.add(cross);
            IndexType cross_idx = IndexType(result_vertices.size() - 1);
            edge1.idx_interm.add(cross_idx);
            edge2.idx_interm.add(cross_idx);
        }
    }

    //
    // traverse all edges
    // sort cross points in each edge
    // generate connection map of all vertices
    //
    for (int i_edge = 0; i_edge < orig_edges.size(); i_edge++)
    {
        EdgeRebuildTmp& edge = orig_edges[i_edge];

        // do sort
        if (edge.idx_interm.size() > 2)
        {
            IntermVtxSorter sorter(edge, result_vertices);
            edge.idx_interm.sort(sorter);
        }

        // record vertex connection


    }

    // do triangulate
}


} // namespace treeface
