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
        edge_helper_edge_map.resize(edges.size());
        for (int i = 0; i < edge_helper_edge_map.size(); i++)
            edge_helper_edge_map[i] = -1;
    }

    void add(IndexType edge_idx)
    {
        SUCK_GEOM(GeomSucker sucker(vertices, edges, "add edge to search store"); )
        SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.5);                                 )
        SUCK_GEOM(for (IndexType idx : i_left_edges)                              )
        SUCK_GEOM(    sucker.draw_edge(idx);                                      )
        SUCK_GEOM(sucker.rgba(SUCKER_GREEN, 0.7);                                 )
        SUCK_GEOM(sucker.draw_edge(edge_idx, 2.0f);                               )

        i_left_edges.add(edge_idx);
        edge_helper_edge_map[edge_idx] = edge_idx;
    }

    void remove(IndexType edge_idx)
    {
        i_left_edges.removeFirstMatchingValue(edge_idx);

        SUCK_GEOM(GeomSucker sucker(vertices, edges, "remove edge from search store"); )
        SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.5);                                      )
        SUCK_GEOM(for (IndexType idx : i_left_edges)                                   )
        SUCK_GEOM(    sucker.draw_edge(idx);                                           )
        SUCK_GEOM(sucker.rgba(SUCKER_RED, 0.7);                                        )
        SUCK_GEOM(sucker.draw_edge(edge_idx, 2.0f);                                    )
    }

    IndexType get_edge_helper_edge(IndexType edge_idx)
    {
        IndexType helper_idx = edge_helper_edge_map[edge_idx];
        jassert(0 <= helper_idx && helper_idx < edges.size());
        return helper_idx;
    }

    ///
    /// \brief find_nearest_left_edge
    ///
    /// If no left edge is found, it will search for right edge!
    ///
    /// \param position
    /// \return half edge index
    ///
    IndexType find_nearest_left_edge(const Vec2f& position)
    {
        IndexType result = std::numeric_limits<IndexType>::max();
        float min_x_dist = std::numeric_limits<float>::max();

        IndexType result_rev = std::numeric_limits<IndexType>::max();
        float min_x_dist_rev = std::numeric_limits<float>::max();


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

            if (x_dist > 0)
            {
                if (x_dist < min_x_dist)
                {
                    result = i_edge;
                    min_x_dist = x_dist;
                }
            }
            else
            {
                x_dist *= -1;
                if (x_dist < min_x_dist_rev)
                {
                    result_rev = i_edge;
                    min_x_dist_rev = x_dist;
                }
            }
        }

        if (result != std::numeric_limits<IndexType>::max())
        {
            jassert(min_x_dist != std::numeric_limits<float>::max());
            return result;
        }
        else
        {
            jassert(result_rev != std::numeric_limits<IndexType>::max());
            jassert(min_x_dist_rev != std::numeric_limits<float>::max());
            return result_rev;
        }
    }

    Array<IndexType> edge_helper_edge_map; // edge idx => helper edge idx
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

bool _vec_are_ccw_(const Vec2f& v1, const Vec2f& v2, const Vec2f& v3)
{
    jassert(v1.length2() - 1.0f < 0.00001f);
    jassert(v2.length2() - 1.0f < 0.00001f);
    jassert(v3.length2() - 1.0f < 0.00001f);

    Vec2f v12 = v2 - v1;
    Vec2f v13 = v3 - v1;

    float orient = v12 ^ v13;

    return orient >= 0;
}

IndexType _iter_next_edge_diff_vtx_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, const IndexType i_edge_from)
{
    const HalfEdge& edge_from = edges[i_edge_from];
    const Vec2f& vtx_from = edge_from.get_vertex(vertices);

    for (IndexType i_edge = edge_from.idx_next_edge; i_edge != i_edge_from;)
    {
        const HalfEdge& edge = edges[i_edge];
        const Vec2f& vtx = edge.get_vertex(vertices);

        if (vtx != vtx_from)
        {
            return i_edge;
        }

        i_edge = edge.idx_next_edge;
    }

    jassertfalse;
    return std::numeric_limits<IndexType>::max();
}

IndexType _iter_prev_edge_diff_vtx_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, const IndexType i_edge_from)
{
    const HalfEdge& edge_from = edges[i_edge_from];
    const Vec2f& vtx_from = edge_from.get_vertex(vertices);


    for (IndexType i_edge = edge_from.idx_prev_edge; i_edge != i_edge_from;)
    {
        const HalfEdge& edge = edges[i_edge];
        const Vec2f& vtx = edge.get_vertex(vertices);
        if (vtx != vtx_from)
        {
            return i_edge;
        }

        i_edge = edge.idx_prev_edge;
    }

    jassertfalse;
    return std::numeric_limits<IndexType>::max();

}

IndexType _search_connectable_edge_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges, const IndexType i_edge_ref, const IndexType i_edge_search)
{
    const Vec2f& vtx_target = edges[i_edge_search].get_vertex(vertices);

    // center vector
    // orients from target vertex
    Vec2f v_connect;

    {
        Vec2f vtx_ref_use = edges[i_edge_ref].get_vertex(vertices);
        if (vtx_ref_use == vtx_target)
        {
            IndexType i_edge_ref_next = _iter_next_edge_diff_vtx_(vertices, edges, i_edge_ref);
            IndexType i_edge_ref_prev = _iter_prev_edge_diff_vtx_(vertices, edges, i_edge_ref);
            vtx_ref_use = edges[i_edge_ref_next].get_vertex(vertices) + edges[i_edge_ref_prev].get_vertex(vertices);
        }

        v_connect = vtx_ref_use - vtx_target;
        jassert(v_connect.length2() > 0.0f);
        v_connect.normalize();
    }

    // search forward
    IndexType i_edge_target = i_edge_search;
    bool got_target = false;

    for (;;)
    {
        const HalfEdge& edge_target = edges[i_edge_target];

        IndexType i_edge_target_next = _iter_next_edge_diff_vtx_(vertices, edges, i_edge_target);
        IndexType i_edge_target_prev = _iter_prev_edge_diff_vtx_(vertices, edges, i_edge_target);

        const Vec2f& vtx_target_next = edges[i_edge_target_next].get_vertex(vertices);
        const Vec2f& vtx_target_prev = edges[i_edge_target_prev].get_vertex(vertices);

        // test if link edge is contained inside
        Vec2f v_next = vtx_target_next - vtx_target;
        Vec2f v_prev = vtx_target_prev - vtx_target;
        jassert(v_next.length2() > 0.0f);
        jassert(v_prev.length2() > 0.0f);
        v_next.normalize();
        v_prev.normalize();

        if (_vec_are_ccw_(v_next, v_connect, v_prev))
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges, "search edge clockwise: success");  )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_next);                       )
            SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7f);                                       )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_prev);                       )
            SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                              )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, edges[i_edge_ref].get_vertex(vertices));)
            SUCK_GEOM(})

            got_target = true;
            break;
        }

        // iter to peer's next
        SUCK_GEOM({)
        SUCK_GEOM(    GeomSucker sucker(vertices, edges, "search edge clockwise: fail");     )
        SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_next);                       )
        SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7f);                                       )
        SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_prev);                       )
        SUCK_GEOM(    sucker.rgb(SUCKER_RED);                                                )
        SUCK_GEOM(    sucker.draw_vector(vtx_target, edges[i_edge_ref].get_vertex(vertices));)
        SUCK_GEOM(})

        if (edge_target.idx_peer_edge == std::numeric_limits<IndexType>::max())
            break;

        IndexType i_next_to_search = edge_target.get_peer(edges).idx_next_edge;
        if (i_next_to_search == i_edge_search)
            break;

        i_edge_target = i_next_to_search;
    }

    // search revert
    if (!got_target)
    {
        i_edge_target = i_edge_search;

        for (;;)
        {
            const HalfEdge& edge_target = edges[i_edge_target];

            IndexType i_edge_target_next = _iter_next_edge_diff_vtx_(vertices, edges, i_edge_target);
            IndexType i_edge_target_prev = _iter_prev_edge_diff_vtx_(vertices, edges, i_edge_target);

            const Vec2f& vtx_target_next = edges[i_edge_target_next].get_vertex(vertices);
            const Vec2f& vtx_target_prev = edges[i_edge_target_prev].get_vertex(vertices);

            // test if link edge is contained inside
            Vec2f v_next = vtx_target_next - vtx_target;
            Vec2f v_prev = vtx_target_prev - vtx_target;
            jassert(v_next.length2() > 0.0f);
            jassert(v_prev.length2() > 0.0f);
            v_next.normalize();
            v_prev.normalize();

            if (_vec_are_ccw_(v_next, v_connect, v_prev))
            {
                SUCK_GEOM({)
                SUCK_GEOM(    GeomSucker sucker(vertices, edges, "search edge clockwise: success");  )
                SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_next);                       )
                SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7f);                                       )
                SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_prev);                       )
                SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                              )
                SUCK_GEOM(    sucker.draw_vector(vtx_target, edges[i_edge_ref].get_vertex(vertices));)
                SUCK_GEOM(})

                got_target = true;
                break;
            }

            // iter to prev's peer
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges, "search edge clockwise: fail");     )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_next);                       )
            SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7f);                                       )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, vtx_target_prev);                       )
            SUCK_GEOM(    sucker.rgb(SUCKER_RED);                                                )
            SUCK_GEOM(    sucker.draw_vector(vtx_target, edges[i_edge_ref].get_vertex(vertices));)
            SUCK_GEOM(})

            IndexType i_next_to_search = edge_target.get_prev(edges).idx_peer_edge;
            if (i_next_to_search == std::numeric_limits<IndexType>::max() || i_next_to_search == i_edge_search)
                break;

            i_edge_target = i_next_to_search;
        }
    }

    jassert(got_target);
    return i_edge_target;
}

//IndexType _search_edge_in_same_ring_(const Array<HalfEdge>& edges, IndexType i_edge_start, IndexType i_target_vtx)
//{
//    for (IndexType i_edge = i_edge_start;;)
//    {
//        const HalfEdge& edge = edges[i_edge];
//        if (edge.idx_vertex == i_target_vtx)
//            return i_edge;
//
//        i_edge = edge.idx_next_edge;
//        if (i_edge == i_edge_start)
//            break;
//    }
//
//    jassertfalse;
//    return std::numeric_limits<IndexType>::max();
//}

void partition_polygon_monotone(const Array<Vec2f>& vertices,
                                const Array<HalfEdge>& edges_input,
                                Array<HalfEdge>& edges_result)
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


    // determine edge role
    Array<VertexRole> vtx_roles;
    vtx_roles.resize(vertices.size());
    for (const HalfEdge& edge_curr : edges_input)
    {
        const Vec2f& vtx_curr = edge_curr.get_vertex(vertices);
        const Vec2f& vtx_prev = edge_curr.get_prev(edges_input).get_vertex(vertices);
        const Vec2f& vtx_next = edge_curr.get_next(edges_input).get_vertex(vertices);
        Vec2f v1 = vtx_curr - vtx_prev;
        Vec2f v2 = vtx_next - vtx_curr;

        if (_is_below_(vtx_prev, vtx_curr))
        {
            if (_is_below_(vtx_curr, vtx_next))
            {
                vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_RIGHT;
            }
            else
            {
                if (is_convex(v1, v2))
                    vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_START;
                else
                    vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_SPLIT;
            }
        }
        else
        {
            if (_is_below_(vtx_curr, vtx_next))
            {
                if (is_convex(v1, v2))
                    vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_END;
                else
                    vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_MERGE;
            }
            else
            {
                vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_LEFT;
            }
        }
    }

    // process all edges by decreasing Y corrd
    HelpEdgeStore helper_store(vertices, edges_input);

    for (IndexType i_edge_curr : edge_idx_by_y)
    {
        const HalfEdge& edge_curr = edges_input[i_edge_curr];
        const Vec2f& vtx_curr = edge_curr.get_vertex(vertices);
        const Vec2f& vtx_prev = edge_curr.get_prev(edges_input).get_vertex(vertices);
        const Vec2f& vtx_next = edge_curr.get_next(edges_input).get_vertex(vertices);
        Vec2f v1 = vtx_curr - vtx_prev;
        Vec2f v2 = vtx_next - vtx_curr;

        switch (vtx_roles[edge_curr.idx_vertex])
        {
        case VTX_ROLE_START:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
            SUCK_GEOM(    sucker.text("start vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_start_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i)
            helper_store.add(i_edge_curr);
        }
            break;

        case VTX_ROLE_END  :
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);    )
            SUCK_GEOM(    sucker.text("end vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_end_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper( e(i-1) ) in D
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = edges_result[i_edge_of_prev_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "helper of prev"); )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);)
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                i_edge_of_prev_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
                _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
            }

            //PSEUDOCODE Delete e(i-1) from T
            helper_store.remove(edge_curr.idx_prev_edge);
        }
            break;

        case VTX_ROLE_LEFT :
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);             )
            SUCK_GEOM(    sucker.text("regular left vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_regular_left_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = edges_result[i_edge_of_prev_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "prev edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);       )
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                i_edge_of_prev_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
                _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
            }

            //PSEUDOCODE Delete e(i-1) from T.
            helper_store.remove(edge_curr.idx_prev_edge);

            //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i).
            helper_store.add(i_edge_curr);
        }
            break;

        case VTX_ROLE_RIGHT:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);                     )
            SUCK_GEOM(    sucker.text("regular right vtx", edge_curr.idx_vertex);        )
            SUCK_GEOM(    sucker.draw_regular_right_vtx(edge_curr.idx_vertex);           )
            SUCK_GEOM(})

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i) .
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges_result[i_edge_of_left_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(j)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
            if (vtx_roles[i_left_edge_helper] == VTX_ROLE_MERGE)
            {
                i_edge_of_left_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
                _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
            }

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "set helper to current vtx");          )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr); )
            SUCK_GEOM(})
        }
            break;

        case VTX_ROLE_SPLIT:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
            SUCK_GEOM(    sucker.text("split vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_split_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i)
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges_result[i_edge_of_left_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE Insert the diagonal connecting v(i) to helper(e(j)) in D
            i_edge_of_left_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
            _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "set helper to current vtx");          )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr); )
            SUCK_GEOM(})

            //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i).
            helper_store.add(i_edge_curr);
        }
            break;

        case VTX_ROLE_MERGE:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result);      )
            SUCK_GEOM(    sucker.text("merge vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_merge_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = edges_result[i_edge_of_prev_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "prev edge and helper");  )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);  )
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                i_edge_of_prev_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
                _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_prev_edge_helper);
            }

            //PSEUDOCODE Delete e(i-1) from T.
            helper_store.remove(edge_curr.idx_prev_edge);

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i).
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges_result[i_edge_of_left_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(j)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
            if (vtx_roles[i_left_edge_helper] == VTX_ROLE_MERGE)
            {
                i_edge_of_left_edge_helper = _search_connectable_edge_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
                _make_connect_(vertices, edges_result, i_edge_curr, i_edge_of_left_edge_helper);
            }

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(vertices, edges_result, "change helper edge"); )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr);)
            SUCK_GEOM(})
        }
            break;

        default:
            abort();
        }
    }
}

///
/// \brief distinguish single-connected polygons
///
/// Find single-connected polygons from a mess of input edges, assign ID for
/// them, and find the edge's roles
///
/// \param edges              input edge soup
/// \param edge_indices_by_y  edge indices sorted by decreasing Y coord
/// \param edge_polygon_map   edge ID => polygon ID
/// \param edge_site_map      edge ID => role of edge's vertex
///
/// \return number of polygons
///
int16 _separate_monotone_polygons_(const Array<Vec2f>& vertices,
                                   const Array<HalfEdge>& edges,
                                   Array<IndexType>& edge_indices_by_y,
                                   Array<int16>& edge_polygon_map,
                                   Array<VertexRole>& edge_role_map)
{
    int num_edge = edges.size();

    // sort edge by decreasing Y coord
    edge_indices_by_y.resize(num_edge);
    for (int i = 0; i < num_edge; i++)
        edge_indices_by_y[i] = IndexType(i);

    {
        HalfEdgeIndexVerticalSorter sorter(vertices, edges);
        edge_indices_by_y.sort(sorter);
    }

    // initialize polygon map and role map
    edge_polygon_map.resize(num_edge);
    edge_role_map.resize(num_edge);
    for (int i = 0; i < num_edge; i++)
    {
        edge_polygon_map[i] = -1;
        edge_role_map[i] = VTX_ROLE_INVALID;
    }

    // traverse all edges
    int16 num_polygon = 0;
    for (IndexType i_edge_first : edge_indices_by_y)
    {
        // skip processed edge
        if (edge_polygon_map[i_edge_first] >= 0) continue;

        // mark all edges of this polygon
        bool reached_bottom = false;
        for (IndexType i_edge = i_edge_first;;)
        {
            // mark polygon index
            jassert(edge_polygon_map[i_edge] == -1);
            edge_polygon_map[i_edge] = num_polygon;

            // mark vertex role
            const HalfEdge& edge = edges[i_edge];

            if (i_edge == i_edge_first)
            {
                edge_role_map[i_edge] = VTX_ROLE_START;
            }
            else
            {
                const Vec2f& vtx_prev = edge.get_prev(edges).get_vertex(vertices);
                const Vec2f& vtx_curr = edge.get_vertex(vertices);
                const Vec2f& vtx_next = edge.get_next(edges).get_vertex(vertices);

                if (_is_below_(vtx_curr, vtx_prev) && _is_below_(vtx_curr, vtx_next))
                {
                    reached_bottom = true;
                    edge_role_map[i_edge] = VTX_ROLE_END;
                }
                else
                {
                    if (reached_bottom)
                        edge_role_map[i_edge] = VTX_ROLE_RIGHT;
                    else
                        edge_role_map[i_edge] = VTX_ROLE_LEFT;
                }
            }

            // move to next
            if (edge.idx_next_edge == i_edge_first) break;
            i_edge = edge.idx_next_edge;
        }

        num_polygon++;
    }

    SUCK_GEOM(for (int i_poly = 0; i_poly < num_polygon; i_poly++)                         )
    SUCK_GEOM({                                                                            )
    SUCK_GEOM(    GeomSucker sucker(vertices, edges, "monotone polygon #"+String(i_poly)); )
    SUCK_GEOM(    int idx = 0;                                                             )
    SUCK_GEOM(    for (IndexType i_edge : edge_indices_by_y)                               )
    SUCK_GEOM(    {                                                                        )
    SUCK_GEOM(        if (edge_polygon_map[i_edge] != i_poly) continue;                    )
    SUCK_GEOM(        IndexType i_vtx = edges[i_edge].idx_vertex;                          )
    SUCK_GEOM(        sucker.rgba(SUCKER_BLACK, 0.5);                                      )
    SUCK_GEOM(        sucker.draw_edge(i_edge);                                            )
    //SUCK_GEOM(        switch (edge_role_map[i_edge])                                       )
    //SUCK_GEOM(        {                                                                    )
    //SUCK_GEOM(        case VTX_ROLE_START: sucker.draw_start_vtx(i_vtx); break;            )
    //SUCK_GEOM(        case VTX_ROLE_END: sucker.draw_end_vtx(i_vtx); break;                )
    //SUCK_GEOM(        case VTX_ROLE_LEFT: sucker.draw_regular_left_vtx(i_vtx); break;      )
    //SUCK_GEOM(        case VTX_ROLE_RIGHT: sucker.draw_regular_right_vtx(i_vtx); break;    )
    //SUCK_GEOM(        default: jassertfalse;                                               )
    //SUCK_GEOM(        }                                                                    )
    SUCK_GEOM(        sucker.rgb(SUCKER_BLACK);                                            )
    SUCK_GEOM(        sucker.text(String(idx), i_vtx);                                     )
    SUCK_GEOM(        idx++;                                                               )
    SUCK_GEOM(    }                                                                        )
    SUCK_GEOM(})

    return num_polygon;
}

void _triangulate_monotone_polygons_(const Array<Vec2f>& vertices,
                                    const Array<HalfEdge>& edges,
                                    const Array<IndexType>& edge_indices_by_y,
                                    const Array<int16>& edge_polygon_map,
                                    int16 num_polygon,
                                    const Array<VertexRole>& edge_roles,
                                    Array<IndexType>& result_indices)
{
    SUCK_GEOM({)
    SUCK_GEOM(    GeomSucker sucker(vertices, edges, "input for triangulation");)
    SUCK_GEOM(})

    Array<IndexType> edge_stack;

    for (int16 i_polygon = 0; i_polygon < num_polygon; i_polygon++)
    {
        int count = 0;

        IndexType i_edge_first = std::numeric_limits<IndexType>::max();
        IndexType i_edge_prev = std::numeric_limits<IndexType>::max();

        //
        // traverse by decreasing Y coord
        //
        for (IndexType i_edge : edge_indices_by_y)
        {
            if (edge_polygon_map[i_edge] != i_polygon) continue;

            const VertexRole role = edge_roles[i_edge];
            const HalfEdge& edge = edges[i_edge];
            const Vec2f& vtx = edge.get_vertex(vertices);

            DBGCODE( if (count == 0) jassert(role == VTX_ROLE_START) );

            if (count < 2)
            {
                //
                // first two edges
                //
                SUCK_GEOM(GeomSucker sucker(vertices, edges);          )
                SUCK_GEOM(sucker.draw_edge_stack(edge_stack);               )
                SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.8);              )
                SUCK_GEOM(sucker.draw_edge(i_edge, 2.0f);                    )
                SUCK_GEOM(sucker.text("enstack first two edges", vtx); )

                if (count == 0)
                {
                    jassert(role == VTX_ROLE_START);
                    edge_stack.clear();
                    i_edge_first = i_edge;
                }
                edge_stack.add(i_edge);
            }
            else
            {
                //
                // middle edges
                //
                if (role != VTX_ROLE_END)
                {
                    //PSEUDOCODE if u(j) and the vertex on top of S are on different chains
                    if (role != edge_roles[edge_stack.getLast()])
                    {
                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges);          )
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);               )
                        SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.8);              )
                        SUCK_GEOM(    sucker.draw_edge(i_edge, 2.0f);                    )
                        SUCK_GEOM(    sucker.text("curr on different chain", vtx); )
                        SUCK_GEOM(})

                        //PSEUDOCODE Pop all vertices from S.
                        //PSEUDOCODE Insert into D a diagonal from u(j) to each popped vertex, except the last one.
                        for (int i_stack = 1; i_stack < edge_stack.size(); i_stack++)
                        {
                            IndexType i_curr = edge_stack[i_stack];
                            IndexType i_prev = edge_stack[i_stack-1];
                            result_indices.add(edge.idx_vertex);
                            result_indices.add(edges[i_curr].idx_vertex);
                            result_indices.add(edges[i_prev].idx_vertex);
                            SUCK_GEOM({)
                            SUCK_GEOM(    GeomSucker sucker(vertices, edges, "do triangulation");)
                            SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);                         )
                            SUCK_GEOM(    sucker.rgba(SUCKER_GREEN, 0.5);                        )
                            SUCK_GEOM(    sucker.draw_trig_by_edge(i_curr, i_prev, i_edge);      )
                            SUCK_GEOM(})
                        }

                        edge_stack.clear();

                        //PSEUDOCODE Push u(j−1) and u(j) onto S
                        edge_stack.add(i_edge_prev);
                        edge_stack.add(i_edge);

                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges, "add curr edge and prev edge to stack");)
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);                                             )
                        SUCK_GEOM(})
                    }
                    else
                    {
                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges);          )
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);               )
                        SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.8);              )
                        SUCK_GEOM(    sucker.draw_edge(i_edge, 2.0f);                    )
                        SUCK_GEOM(    sucker.text("curr on same chain", vtx);      )
                        SUCK_GEOM(})

                        jassert(edge_stack.size() > 1);
                        Array<IndexType> popped_edges;

                        //PSEUDOCODE Pop one vertex from S.
                        popped_edges.add(edge_stack.getLast());
                        edge_stack.removeLast(1);

                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges);                                        )
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);                                        )
                        SUCK_GEOM(    sucker.rgba(SUCKER_RED, 0.2);                                              )
                        SUCK_GEOM(    sucker.draw_edge(popped_edges.getLast(), 2.0f);                                  )
                        SUCK_GEOM(    sucker.rgb(SUCKER_BLACK);                                                  )
                        SUCK_GEOM(    sucker.text("remove stack top", edges[popped_edges.getLast()].idx_vertex); )
                        SUCK_GEOM(})


                        //PSEUDOCODE Pop the other vertices from S as long as the diagonals from u(j) to them are inside P.
                        while (edge_stack.size() > 0)
                        {
                            const IndexType i_edge_stack_top = edge_stack.getLast();
                            const Vec2f& vtx_stack_top = edges[i_edge_stack_top].get_vertex(vertices);
                            const IndexType i_edge_popped = popped_edges.getLast();
                            const Vec2f& vtx_popped = edges[i_edge_popped].get_vertex(vertices);

                            bool diag_is_inside;
                            if (edge_roles[i_edge] == VTX_ROLE_LEFT)
                                diag_is_inside = is_convex(vtx_stack_top, vtx_popped, vtx);
                            else if (edge_roles[i_edge] == VTX_ROLE_RIGHT)
                                diag_is_inside = is_convex(vtx, vtx_popped, vtx_stack_top);
                            else
                                jassertfalse;

                            SUCK_GEOM(GeomSucker sucker(vertices, edges);)
                            SUCK_GEOM(sucker.draw_edge_stack(edge_stack);)

                            if (diag_is_inside)
                            {
                                SUCK_GEOM(sucker.rgba(SUCKER_BLUE, 0.7);                          )
                                SUCK_GEOM(sucker.draw_edge(i_edge_popped, 1.5f);                  )
                                SUCK_GEOM(sucker.rgba(SUCKER_GREEN, 0.7);                         )
                                SUCK_GEOM(sucker.draw_edge(i_edge_stack_top, 1.0f);               )
                                SUCK_GEOM(sucker.text("top is convex, pop stack", vtx_stack_top); )
                                SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.7);                         )
                                SUCK_GEOM(sucker.draw_edge(i_edge, 0.5f);                         )
                                popped_edges.add(i_edge_stack_top);
                                edge_stack.removeLast(1);
                            }
                            else
                            {
                                SUCK_GEOM(sucker.rgba(SUCKER_BLUE, 0.7);                          )
                                SUCK_GEOM(sucker.draw_edge(i_edge_popped, 1.5f);                  )
                                SUCK_GEOM(sucker.rgba(SUCKER_RED, 0.7);                           )
                                SUCK_GEOM(sucker.draw_edge(i_edge_stack_top, 1.0f);               )
                                SUCK_GEOM(sucker.text("top is not convex, break", vtx_stack_top); )
                                SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.7);                         )
                                SUCK_GEOM(sucker.draw_edge(i_edge, 0.5f);                         )
                                break;
                            }
                        }

                        //PSEUDOCODE Insert these diagonals into D.
                        for (int i = 1; i < popped_edges.size(); i++)
                        {
                            // the last one continues with edge stack top
                            IndexType i_edge1 = popped_edges[i];
                            IndexType i_edge2 = popped_edges[i-1];

                            result_indices.add(edge.idx_vertex);
                            result_indices.add(edges[i_edge1].idx_vertex);
                            result_indices.add(edges[i_edge2].idx_vertex);

                            SUCK_GEOM({)
                            SUCK_GEOM(    GeomSucker sucker(vertices, edges, "do triangulation"); )
                            SUCK_GEOM(    sucker.draw_edge_stack(popped_edges);                   )
                            SUCK_GEOM(    sucker.rgba(SUCKER_GREEN, 0.5);                         )
                            SUCK_GEOM(    sucker.draw_trig_by_edge(i_edge1, i_edge2, i_edge);     )
                            SUCK_GEOM(})
                        }

                        //PSEUDOCODE Push the last vertex that has been popped back onto S.
                        //PSEUDOCODE Push u(j) onto S.
                        IndexType i_popped_last = popped_edges.getLast();

                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges);          )
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);          )
                        SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7);              )
                        SUCK_GEOM(    sucker.draw_edge(i_edge);                    )
                        SUCK_GEOM(    sucker.text("enstack current edge", vtx);    )
                        SUCK_GEOM(    sucker.rgba(SUCKER_BLUE, 0.7);               )
                        SUCK_GEOM(    sucker.draw_edge(i_popped_last);             )
                        SUCK_GEOM(    sucker.text("enstack last popped edge", edges[i_popped_last].idx_vertex); )
                        SUCK_GEOM(})

                        edge_stack.add(i_popped_last);
                        edge_stack.add(i_edge);
                    }
                }

                //
                // last edge
                //
                else
                {
                    //PSEUDOCODE Add diagonals from u n to all stack vertices except the first and the last one.
                    SUCK_GEOM(if (edge_stack.size() > 1)              )
                    SUCK_GEOM({                                       )
                    SUCK_GEOM(    GeomSucker sucker(vertices, edges); )
                    SUCK_GEOM(    sucker.draw_edge_stack(edge_stack); )
                    SUCK_GEOM(    sucker.rgba(SUCKER_BLACK, 0.7);     )
                    SUCK_GEOM(    sucker.draw_edge(i_edge);           )
                    SUCK_GEOM(    sucker.text("last one", vtx);       )
                    SUCK_GEOM(}                                       )

                    for (int i = 1; i < edge_stack.size(); i++)
                    {
                        IndexType i_prev = edge_stack[i-1];
                        IndexType i_curr = edge_stack[i];
                        result_indices.add(edge.idx_vertex);
                        result_indices.add(edges[i_prev].idx_vertex);
                        result_indices.add(edges[i_curr].idx_vertex);

                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(vertices, edges, "do triangulation"); )
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);                     )
                        SUCK_GEOM(    sucker.rgba(SUCKER_GREEN, 0.5);                         )
                        SUCK_GEOM(    sucker.draw_trig_by_edge(i_curr, i_prev, i_edge);       )
                        SUCK_GEOM(})
                    }

                    break;
                }
            }

            i_edge_prev = i_edge;
            count++;
        }
    } // polygon cycle
}

///
/// \brief do triangulate
/// \param idx_list  a list of all indices
/// \param vertices  vertex list that all index values should be valid in it
/// \param indices   result triangle indices will be filled to here
///
void _triangulate_(const Array<Vec2f>& vertices, const Array<HalfEdge>& edges_input, Array<IndexType>& result_indices)
{
    Array<HalfEdge> edges_monotone;
    partition_polygon_monotone(vertices, edges_input, edges_monotone);

    Array<IndexType> edge_monotone_by_y;
    Array<int16> edge_polygon_map;
    Array<VertexRole> monotone_edge_roles;
    int16 num_polygon = _separate_monotone_polygons_(vertices, edges_monotone, edge_monotone_by_y, edge_polygon_map, monotone_edge_roles);

    _triangulate_monotone_polygons_(vertices, edges_monotone, edge_monotone_by_y, edge_polygon_map, num_polygon, monotone_edge_roles, result_indices);
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
    treecore::Array<HalfEdge> edges;
    for (int i_subpath = 0; i_subpath < subpaths.size(); i_subpath++)
    {
        IndexType i_vtx_begin = subpath_begin_indices[i_subpath];
        IndexType i_vtx_end = i_subpath == subpaths.size() - 1
                ? result_vertices.size()
                : subpath_begin_indices[i_subpath + 1];

        IndexType i_vtx_last = i_vtx_end - 1;

        for (IndexType i_vtx = i_vtx_begin; i_vtx != i_vtx_end; i_vtx++)
        {
            jassert(i_vtx == edges.size());
            IndexType i_edge_prev;
            IndexType i_edge_next;

            if (clw_accum_global < 0.0)
            {
                i_edge_prev = i_vtx == i_vtx_begin
                        ? i_vtx_last
                        : i_vtx - 1;
                i_edge_next = i_vtx == i_vtx_last
                        ? i_vtx_begin
                        : i_vtx + 1;
            }
            else
            {
                i_edge_prev = i_vtx == i_vtx_last
                        ? i_vtx_begin
                        : i_vtx + 1;
                i_edge_next = i_vtx == i_vtx_begin
                        ? i_vtx_last
                        : i_vtx - 1;
            }

            edges.add(HalfEdge{i_vtx, i_edge_prev, i_edge_next, std::numeric_limits<IndexType>::max()});
        }
    }

    // do triangulation
    _triangulate_(result_vertices, edges, result_indices);
}

} // namespace treeface
