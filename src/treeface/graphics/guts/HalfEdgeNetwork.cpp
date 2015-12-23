#include "treeface/graphics/guts/HalfEdgeNetwork.h"
#include "treeface/graphics/guts/geomsucker.h"
#include "treeface/graphics/utils.h"

using namespace treecore;

namespace treeface
{

struct HelpEdgeStore
{
    HelpEdgeStore(const HalfEdgeNetwork& network)
        : network(network)
    {
        jassert(network.vertices.size() == network.edges.size());
        edge_helper_edge_map.resize(network.edges.size());
        for (int i = 0; i < edge_helper_edge_map.size(); i++)
            edge_helper_edge_map[i] = -1;
    }

    void add(IndexType edge_idx)
    {
        SUCK_GEOM(GeomSucker sucker(network, "add edge to search store"); )
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

        SUCK_GEOM(GeomSucker sucker(network, "remove edge from search store"); )
        SUCK_GEOM(sucker.rgba(SUCKER_BLACK, 0.5);                                      )
        SUCK_GEOM(for (IndexType idx : i_left_edges)                                   )
        SUCK_GEOM(    sucker.draw_edge(idx);                                           )
        SUCK_GEOM(sucker.rgba(SUCKER_RED, 0.7);                                        )
        SUCK_GEOM(sucker.draw_edge(edge_idx, 2.0f);                                    )
    }

    IndexType get_edge_helper_edge(IndexType edge_idx)
    {
        IndexType helper_idx = edge_helper_edge_map[edge_idx];
        jassert(0 <= helper_idx && helper_idx < network.edges.size());
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
            const HalfEdge& edge = network.edges[i_edge];
            const Vec2f& p1 = edge.get_vertex(network.vertices);
            const Vec2f& p2 = edge.get_next(network.edges).get_vertex(network.vertices);

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
    const HalfEdgeNetwork& network;
};


void HalfEdgeNetwork::build_half_edges(const treecore::Array<IndexType>& subpath_begin, bool is_cclw)
{
    for (int i_subpath = 0; i_subpath < subpath_begin.size(); i_subpath++)
    {
        IndexType i_vtx_begin = subpath_begin[i_subpath];
        IndexType i_vtx_end = i_subpath == subpath_begin.size() - 1
                ? vertices.size()
                : subpath_begin[i_subpath + 1];

        IndexType i_vtx_last = i_vtx_end - 1;

        for (IndexType i_vtx = i_vtx_begin; i_vtx != i_vtx_end; i_vtx++)
        {
            jassert(i_vtx == edges.size());
            IndexType i_edge_prev;
            IndexType i_edge_next;

            if (is_cclw)
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
}

void HalfEdgeNetwork::connect(IndexType i_edge1, IndexType i_edge2)
{
    // pre-calculate the index of newly added half edges
    IndexType i_edge_1_2 = edges.size();
    IndexType i_edge_2_1 = i_edge_1_2 + 1;

    HalfEdge& edge1 = edges[i_edge1];
    IndexType i_edge1_prev = edge1.idx_prev_edge;
    HalfEdge& edge1_prev = edges[i_edge1_prev];

    HalfEdge& edge2 = edges[i_edge2];
    IndexType i_edge2_prev = edge2.idx_prev_edge;
    HalfEdge& edge2_prev = edges[i_edge2_prev];

    SUCK_GEOM({)
    SUCK_GEOM(    GeomSucker sucker(*this, "going to link");   )
    SUCK_GEOM(    sucker.draw_edge(i_edge1);           )
    SUCK_GEOM(    sucker.draw_edge(i_edge2);           )
    SUCK_GEOM(    sucker.rgb(SUCKER_BLUE);             )
    SUCK_GEOM(    sucker.draw_edge(i_edge1_prev, 3.0f);)
    SUCK_GEOM(    sucker.draw_edge(i_edge2_prev, 3.0f);)
    SUCK_GEOM(})


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
    SUCK_GEOM(    GeomSucker sucker(*this, "linked");   )
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

IndexType HalfEdgeNetwork::get_next_edge_diff_vtx(const IndexType i_edge_search_base) const
{
    const HalfEdge& edge_from = edges[i_edge_search_base];
    const Vec2f& vtx_from = edge_from.get_vertex(vertices);

    for (IndexType i_edge = edge_from.idx_next_edge; i_edge != i_edge_search_base;)
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

IndexType HalfEdgeNetwork::get_prev_edge_diff_vtx(const IndexType i_edge_search_base) const
{
    const HalfEdge& edge_from = edges[i_edge_search_base];
    const Vec2f& vtx_from = edge_from.get_vertex(vertices);


    for (IndexType i_edge = edge_from.idx_prev_edge; i_edge != i_edge_search_base;)
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

bool HalfEdgeNetwork::fan_is_facing(const Vec2f& vec_ref, IndexType i_edge) const
{
    const Vec2f& vtx_fan_base = edges[i_edge].get_vertex(vertices);
    const Vec2f& vtx_next = edges[get_next_edge_diff_vtx(i_edge)].get_vertex(vertices);
    const Vec2f& vtx_prev = edges[get_prev_edge_diff_vtx(i_edge)].get_vertex(vertices);

    Vec2f v_next = vtx_next - vtx_fan_base;
    Vec2f v_prev = vtx_prev - vtx_fan_base;

    return vec_are_cclw(v_next, vec_ref, v_prev);
}

void HalfEdgeNetwork::get_edge_vertical_order(treecore::Array<IndexType>& result) const
{
    int num_edge = edges.size();
    result.resize(num_edge);

    for (int i = 0; i < num_edge; i++)
        result[i] = IndexType(i);

    HalfEdgeIndexVerticalSorter sorter(vertices, edges);
    result.sort(sorter);
}

void HalfEdgeNetwork::iter_edge_to_facing_fan(const IndexType i_edge_ref, IndexType& i_edge_iter) const
{
    const Vec2f& vtx_fan_base = edges[i_edge_iter].get_vertex(vertices);

    // center vector
    // orients from target vertex
    Vec2f v_ref;

    {
        Vec2f vtx_ref_use = edges[i_edge_ref].get_vertex(vertices);
        if (vtx_ref_use == vtx_fan_base)
        {
            IndexType i_edge_ref_next = get_next_edge_diff_vtx(i_edge_ref);
            IndexType i_edge_ref_prev = get_prev_edge_diff_vtx(i_edge_ref);
            vtx_ref_use = edges[i_edge_ref_next].get_vertex(vertices) + edges[i_edge_ref_prev].get_vertex(vertices);
            vtx_ref_use /= 2;
        }

        v_ref = vtx_ref_use - vtx_fan_base;
        jassert(v_ref.length2() > 0.0f);
        v_ref.normalize();
    }

    // search self
    const HalfEdge& edge_begin = edges[i_edge_iter];
    if (fan_is_facing(v_ref, i_edge_iter))
    {
        SUCK_GEOM({)
        SUCK_GEOM(    GeomSucker sucker(*this, "search target facing: success");                )
        SUCK_GEOM(    sucker.draw_edge(i_edge_iter);                                            )
        SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                                 )
        SUCK_GEOM(    sucker.draw_vector(vtx_fan_base, edges[i_edge_ref].get_vertex(vertices)); )
        SUCK_GEOM(})
        return;
    }

    // search forward
    for (IndexType i_edge_search = edge_begin.get_next(edges).idx_peer_edge;
         i_edge_search != std::numeric_limits<IndexType>::max() && i_edge_search != i_edge_iter;
         i_edge_search = edges[i_edge_search].get_next(edges).idx_peer_edge)
    {
        if (fan_is_facing(v_ref, i_edge_search))
        {
            i_edge_iter = i_edge_search;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this, "edge is facing");                     )
            SUCK_GEOM(    sucker.draw_edge(i_edge_iter);                                            )
            SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                                 )
            SUCK_GEOM(    sucker.draw_vector(vtx_fan_base, edges[i_edge_ref].get_vertex(vertices)); )
            SUCK_GEOM(})
            return;
        }

        SUCK_GEOM({)
        SUCK_GEOM(    GeomSucker sucker(*this, "edge is not facing");                 )
        SUCK_GEOM(    sucker.draw_edge(i_edge_iter);                                            )
        SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                                 )
        SUCK_GEOM(    sucker.draw_vector(vtx_fan_base, edges[i_edge_ref].get_vertex(vertices)); )
        SUCK_GEOM(})
    }

    // search revert
    for (IndexType i_edge_search = edge_begin.get_prev(edges).idx_peer_edge;
         i_edge_search != std::numeric_limits<IndexType>::max() && i_edge_search != i_edge_iter;
         i_edge_search = edges[i_edge_search].get_prev(edges).idx_peer_edge)
    {
        if (fan_is_facing(v_ref, i_edge_search))
        {
            i_edge_iter = i_edge_search;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this, "edge is facing");                     )
            SUCK_GEOM(    sucker.draw_edge(i_edge_iter);                                            )
            SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                                 )
            SUCK_GEOM(    sucker.draw_vector(vtx_fan_base, edges[i_edge_ref].get_vertex(vertices)); )
            SUCK_GEOM(})
            return;
        }

        SUCK_GEOM({)
        SUCK_GEOM(    GeomSucker sucker(*this, "edge is not facing");                 )
        SUCK_GEOM(    sucker.draw_edge(i_edge_iter);                                            )
        SUCK_GEOM(    sucker.rgb(SUCKER_GREEN);                                                 )
        SUCK_GEOM(    sucker.draw_vector(vtx_fan_base, edges[i_edge_ref].get_vertex(vertices)); )
        SUCK_GEOM(})
    }

    jassertfalse;
}

///
/// \brief distinguish single-connected polygons
///
/// Find single-connected polygons from a mess of input edges, assign ID for
/// them, and find the edge's roles
///
/// \param edge_indices_by_y  edge indices sorted by decreasing Y coord
/// \param edge_polygon_map   edge ID => polygon ID
/// \param edge_site_map      edge ID => role of edge's vertex
///
/// \return number of polygons
///
int16 HalfEdgeNetwork::mark_monotone_polygons(Array<IndexType>& edge_indices_by_y,
                                              Array<int16>& edge_polygon_map,
                                              Array<VertexRole>& edge_role_map) const
{
    int num_edge = edges.size();

    // sort edge by decreasing Y coord
    get_edge_vertical_order(edge_indices_by_y);

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

                if (is_below(vtx_curr, vtx_prev) && is_below(vtx_curr, vtx_next))
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
    SUCK_GEOM(    GeomSucker sucker(*this, "monotone polygon #"+String(i_poly)); )
    SUCK_GEOM(    int idx = 0;                                                             )
    SUCK_GEOM(    for (IndexType i_edge : edge_indices_by_y)                               )
    SUCK_GEOM(    {                                                                        )
    SUCK_GEOM(        if (edge_polygon_map[i_edge] != i_poly) continue;                    )
    SUCK_GEOM(        IndexType i_vtx = edges[i_edge].idx_vertex;                          )
    SUCK_GEOM(        sucker.rgba(SUCKER_BLACK, 0.5);                                      )
    SUCK_GEOM(        sucker.draw_edge(i_edge);                                            )
    SUCK_GEOM(        sucker.rgb(SUCKER_BLACK);                                            )
    SUCK_GEOM(        sucker.text(String(idx), i_vtx);                                     )
    SUCK_GEOM(        idx++;                                                               )
    SUCK_GEOM(    }                                                                        )
    SUCK_GEOM(})

    return num_polygon;
}

void HalfEdgeNetwork::triangulate_monotone_polygons(const Array<IndexType>& edge_indices_by_y,
                                                    const Array<int16>& edge_polygon_map,
                                                    int16 num_polygon,
                                                    const Array<VertexRole>& edge_roles,
                                                    Array<IndexType>& result_indices) const
{
    SUCK_GEOM({)
    SUCK_GEOM(    GeomSucker sucker(*this, "input for triangulation");)
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
                SUCK_GEOM(GeomSucker sucker(*this);          )
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
                        SUCK_GEOM(    GeomSucker sucker(*this);          )
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
                            SUCK_GEOM(    GeomSucker sucker(*this, "do triangulation");)
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
                        SUCK_GEOM(    GeomSucker sucker(*this, "add curr edge and prev edge to stack");)
                        SUCK_GEOM(    sucker.draw_edge_stack(edge_stack);                                             )
                        SUCK_GEOM(})
                    }
                    else
                    {
                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(*this);          )
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
                        SUCK_GEOM(    GeomSucker sucker(*this);                                        )
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

                            SUCK_GEOM(GeomSucker sucker(*this);)
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
                            SUCK_GEOM(    GeomSucker sucker(*this, "do triangulation"); )
                            SUCK_GEOM(    sucker.draw_edge_stack(popped_edges);                   )
                            SUCK_GEOM(    sucker.rgba(SUCKER_GREEN, 0.5);                         )
                            SUCK_GEOM(    sucker.draw_trig_by_edge(i_edge1, i_edge2, i_edge);     )
                            SUCK_GEOM(})
                        }

                        //PSEUDOCODE Push the last vertex that has been popped back onto S.
                        //PSEUDOCODE Push u(j) onto S.
                        IndexType i_popped_last = popped_edges.getLast();

                        SUCK_GEOM({)
                        SUCK_GEOM(    GeomSucker sucker(*this);          )
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
                    SUCK_GEOM(    GeomSucker sucker(*this); )
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
                        SUCK_GEOM(    GeomSucker sucker(*this, "do triangulation"); )
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

void HalfEdgeNetwork::partition_polygon_monotone(HalfEdgeNetwork& result_network) const
{
    jassert(&vertices == &result_network.vertices);
    result_network.edges = edges;

    int num_edge_orig = edges.size();

    SUCK_GEOM({                                     );
    SUCK_GEOM(    GeomSucker(*this););
    SUCK_GEOM(}                                     );

    // sort by vertical position
    Array<IndexType> edge_idx_by_y;
    get_edge_vertical_order(edge_idx_by_y);


    // determine edge role
    Array<VertexRole> vtx_roles;
    vtx_roles.resize(vertices.size());
    for (const HalfEdge& edge_curr : edges)
    {
        const Vec2f& vtx_curr = edge_curr.get_vertex(vertices);
        const Vec2f& vtx_prev = edge_curr.get_prev(edges).get_vertex(vertices);
        const Vec2f& vtx_next = edge_curr.get_next(edges).get_vertex(vertices);
        Vec2f v1 = vtx_curr - vtx_prev;
        Vec2f v2 = vtx_next - vtx_curr;

        if (is_below(vtx_prev, vtx_curr))
        {
            if (is_below(vtx_curr, vtx_next))
            {
                vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_RIGHT;
            }
            else
            {
                if (is_convex(v1, v2)) vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_START;
                else                   vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_SPLIT;
            }
        }
        else
        {
            if (is_below(vtx_curr, vtx_next))
            {
                if (is_convex(v1, v2)) vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_END;
                else                   vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_MERGE;
            }
            else
            {
                vtx_roles[edge_curr.idx_vertex] = VTX_ROLE_LEFT;
            }
        }
    }

    // process all edges by decreasing Y corrd
    HelpEdgeStore helper_store(*this);

    for (IndexType i_edge_curr : edge_idx_by_y)
    {
        const HalfEdge& edge_curr = edges[i_edge_curr];
        const Vec2f& vtx_curr = edge_curr.get_vertex(vertices);
        const Vec2f& vtx_prev = edge_curr.get_prev(edges).get_vertex(vertices);
        const Vec2f& vtx_next = edge_curr.get_next(edges).get_vertex(vertices);
        Vec2f v1 = vtx_curr - vtx_prev;
        Vec2f v2 = vtx_next - vtx_curr;

        switch (vtx_roles[edge_curr.idx_vertex])
        {
        case VTX_ROLE_START:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this);      )
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
            SUCK_GEOM(    GeomSucker sucker(*this);    )
            SUCK_GEOM(    sucker.text("end vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_end_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper( e(i-1) ) in D
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = result_network.edges[i_edge_of_prev_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "helper of prev"); )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);)
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_prev_edge_helper);
                result_network.connect(i_edge_curr, i_edge_of_prev_edge_helper);
            }

            //PSEUDOCODE Delete e(i-1) from T
            helper_store.remove(edge_curr.idx_prev_edge);
        }
            break;

        case VTX_ROLE_LEFT :
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this);             )
            SUCK_GEOM(    sucker.text("regular left vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_regular_left_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = result_network.edges[i_edge_of_prev_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "prev edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);       )
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_prev_edge_helper);
                result_network.connect(i_edge_curr, i_edge_of_prev_edge_helper);
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
            SUCK_GEOM(    GeomSucker sucker(*this);                                      )
            SUCK_GEOM(    sucker.text("regular right vtx", edge_curr.idx_vertex);        )
            SUCK_GEOM(    sucker.draw_regular_right_vtx(edge_curr.idx_vertex);           )
            SUCK_GEOM(})

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i) .
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges[i_edge_of_left_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(j)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
            if (vtx_roles[i_left_edge_helper] == VTX_ROLE_MERGE)
            {
                result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_left_edge_helper);
                result_network.connect(i_edge_curr, i_edge_of_left_edge_helper);
            }

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "set helper to current vtx");          )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr); )
            SUCK_GEOM(})
        }
            break;

        case VTX_ROLE_SPLIT:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this);      )
            SUCK_GEOM(    sucker.text("split vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_split_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i)
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges[i_edge_of_left_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE Insert the diagonal connecting v(i) to helper(e(j)) in D
            result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_left_edge_helper);
            result_network.connect(i_edge_curr, i_edge_of_left_edge_helper);

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "set helper to current vtx");          )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr); )
            SUCK_GEOM(})

            //PSEUDOCODE Insert e(i) in T and set helper(e(i)) to v(i).
            helper_store.add(i_edge_curr);
        }
            break;

        case VTX_ROLE_MERGE:
        {
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(*this);      )
            SUCK_GEOM(    sucker.text("merge vtx", edge_curr.idx_vertex); )
            SUCK_GEOM(    sucker.draw_merge_vtx(edge_curr.idx_vertex);    )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(i-1)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(i-1)) in D.
            IndexType i_edge_of_prev_edge_helper = helper_store.get_edge_helper_edge(edge_curr.idx_prev_edge);
            IndexType i_prev_edge_helper = edges[i_edge_of_prev_edge_helper].idx_vertex;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "prev edge and helper");  )
            SUCK_GEOM(    sucker.draw_helper(edge_curr.idx_prev_edge, i_prev_edge_helper);  )
            SUCK_GEOM(})

            if (vtx_roles[i_prev_edge_helper] == VTX_ROLE_MERGE)
            {
                result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_prev_edge_helper);
                result_network.connect(i_edge_curr, i_edge_of_prev_edge_helper);
            }

            //PSEUDOCODE Delete e(i-1) from T.
            helper_store.remove(edge_curr.idx_prev_edge);

            //PSEUDOCODE Search in T to find the edge e(j) directly left of v(i).
            IndexType i_left_edge = helper_store.find_nearest_left_edge(vtx_curr);
            IndexType i_edge_of_left_edge_helper = helper_store.get_edge_helper_edge(i_left_edge);
            IndexType i_left_edge_helper = edges[i_edge_of_left_edge_helper].idx_vertex;
            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "left edge and helper"); )
            SUCK_GEOM(    sucker.draw_helper(i_left_edge, i_left_edge_helper);       )
            SUCK_GEOM(})

            //PSEUDOCODE if helper(e(j)) is a merge vertex
            //PSEUDOCODE     then Insert the diagonal connecting v(i) to helper(e(j)) in D.
            if (vtx_roles[i_left_edge_helper] == VTX_ROLE_MERGE)
            {
                result_network.iter_edge_to_facing_fan(i_edge_curr, i_edge_of_left_edge_helper);
                result_network.connect(i_edge_curr, i_edge_of_left_edge_helper);
            }

            //PSEUDOCODE helper(e(j)) = v(i)
            helper_store.edge_helper_edge_map[i_left_edge] = i_edge_curr;

            SUCK_GEOM({)
            SUCK_GEOM(    GeomSucker sucker(result_network, "change helper edge"); )
            SUCK_GEOM(    sucker.draw_helper_change(i_left_edge, i_edge_of_left_edge_helper, i_edge_curr);)
            SUCK_GEOM(})
        }
            break;

        default:
            abort();
        }
    }
}

} // namespace treeface
