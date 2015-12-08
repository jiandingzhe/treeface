#include "treeface/guts/shapegenerator_guts.h"

#include "treecore/IntUtils.h"

#include <list>

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


inline bool _is_convex_(const Vec2f& edge1, const Vec2f& edge2) noexcept
{
    float cross = edge1 ^ edge2;
    return cross > 0;
}

inline bool _is_counter_clockwise_(const Array<Vec2f>& vertices) noexcept
{
    jassert(vertices.size() > 2);

    float sum = 0.0f;

    for (int i = 1; i < vertices.size(); i++)
    {
        const Vec2f& p_curr = vertices[i];
        const Vec2f& p_prev = vertices[i - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    return sum < 0;
}

///
/// \brief do triangulate
/// \param idx_list  a list of all indices
/// \param vertices  vertex list that all index values should be valid in it
/// \param indices   result triangle indices will be filled to here
///
void _triangulate_(IndexList& idx_list, const Array<Vec2f>& vertices, Array<IndexType>& result_indices)
{
    typedef typename treecore::larger_int<IndexType>::type IndexLarger;

    size_t idx_list_size = idx_list.size();

    // process and reduce vertices one by one
    while (idx_list_size > 3)
    {
        jassert(idx_list_size == idx_list.size());

        printf("to be processed:\n");
        for (IndexType idx : idx_list)
            printf("  %u\n", idx);

        // find sharpest convex vertex
        IndexList::iterator it_use;
        IndexType idx_use_prev = std::numeric_limits<IndexType>::max();
        IndexType idx_use_curr = std::numeric_limits<IndexType>::max();
        IndexType idx_use_next = std::numeric_limits<IndexType>::max();

        {
            IndexList::iterator convex_sharp_it;
            IndexLarger         convex_sharp_idx_prev = -1;
            IndexLarger         convex_sharp_idx_curr = -1;
            IndexLarger         convex_sharp_idx_next = -1;
            float               convex_sharp_cosine = 1.0f;

            IndexList::iterator rconvex_sharp_it;
            IndexLarger         rconvex_sharp_idx_prev = -1;
            IndexLarger         rconvex_sharp_idx_curr = -1;
            IndexLarger         rconvex_sharp_idx_next = -1;
            float               rconvex_sharp_cosine = 1.0f;

            bool got_spine = false;

            {
                IndexList::iterator it_prev;
                IndexList::iterator it_curr = idx_list.begin();
                IndexList::iterator it_next = it_curr; it_next++;

                while (it_curr != idx_list.end())
                {
                    // get indices flanking current position
                    IndexType idx_curr = *it_curr;

                    IndexType idx_prev = it_curr == idx_list.begin()
                            ? idx_list.back()
                            : *it_prev;

                    IndexType idx_next = it_next == idx_list.end()
                            ? idx_list.front()
                            : *it_next;

                    jassert(idx_prev < vertices.size());
                    jassert(idx_curr < vertices.size());
                    jassert(idx_next < vertices.size());

                    const Vec2f& vtx_prev = vertices[idx_prev];
                    const Vec2f& vtx_curr = vertices[idx_curr];
                    const Vec2f& vtx_next = vertices[idx_next];

                    Vec2f edge1 = vtx_curr - vtx_prev;
                    Vec2f edge2 = vtx_next - vtx_curr;
                    edge1.normalize();
                    edge2.normalize();

                    float cosine = edge1 * edge2;

                    // we got a spine
                    // use it for highest priority
                    if (idx_prev == idx_next || idx_prev == idx_curr || idx_curr == idx_next)
                    {
                        got_spine = true;
                        convex_sharp_idx_prev = idx_prev;
                        convex_sharp_idx_curr = idx_curr;
                        convex_sharp_idx_next = idx_next;
                        convex_sharp_it = it_curr;
                        break;
                    }

                    bool convex = _is_convex_(edge1, edge2);

                    // current point is sharp
                    if (convex && cosine < convex_sharp_cosine)
                    {
                        convex_sharp_cosine = cosine;
                        convex_sharp_idx_prev = idx_prev;
                        convex_sharp_idx_curr = idx_curr;
                        convex_sharp_idx_next = idx_next;
                        convex_sharp_it = it_curr;
                    }
                    else if (!convex && cosine < rconvex_sharp_cosine)
                    {
                        rconvex_sharp_cosine = cosine;
                        rconvex_sharp_idx_prev = idx_prev;
                        rconvex_sharp_idx_curr = idx_curr;
                        rconvex_sharp_idx_next = idx_next;
                        rconvex_sharp_it = it_curr;
                    }

                    // move to next vertex
                    it_prev = it_curr;
                    it_curr = it_next;
                    it_next++;
                }
            }

            if (got_spine)
            {
                it_use = convex_sharp_it;
                idx_use_prev = IndexType(convex_sharp_idx_prev);
                idx_use_curr = IndexType(convex_sharp_idx_curr);
                idx_use_next = IndexType(convex_sharp_idx_next);
            }
            else if (convex_sharp_cosine < 1.0f)
            {
                it_use = convex_sharp_it;
                idx_use_prev = IndexType(convex_sharp_idx_prev);
                idx_use_curr = IndexType(convex_sharp_idx_curr);
                idx_use_next = IndexType(convex_sharp_idx_next);
            }
            else if (rconvex_sharp_cosine < 1.0f)
            {
                it_use = rconvex_sharp_it;
                idx_use_prev = IndexType(rconvex_sharp_idx_prev);
                idx_use_curr = IndexType(rconvex_sharp_idx_curr);
                idx_use_next = IndexType(rconvex_sharp_idx_next);
            }
            else
            {
                jassertfalse;
            }
        }

        // remove current point, record triangle
        idx_list.erase(it_use);
        idx_list_size--;
        if (idx_use_prev != idx_use_next)
        {
            printf("triangle %u %u %u\n", idx_use_prev, idx_use_curr, idx_use_next);
            result_indices.add(idx_use_prev);
            result_indices.add(idx_use_curr);
            result_indices.add(idx_use_next);
        }
    }

    jassert(idx_list_size == 3);
    jassert(idx_list.size() == 3);

    // add last triangle
    {
        IndexList::iterator it = idx_list.begin();
        IndexType idx1 = *it;
        it++;
        IndexType idx2 = *it;
        it++;
        IndexType idx3 = *it;

        if (idx1 != idx2 && idx2 != idx3 && idx3 != idx1)
        {
            printf("last triangle %u %u %u\n", idx1, idx2, idx3);
            result_indices.add(idx1);
            result_indices.add(idx2);
            result_indices.add(idx3);
        }
    }
}

void SubPath::triangulate_simple(Array<Vec2f>& result_vertices, Array<IndexType>& result_indices) const
{
    if (vertices.size() < 3) return;
    const IndexType subpath_idx_begin = IndexType(result_vertices.size());

    // assign global vertex index
    // generate vertex connection map
    IndexList idx_list;

    if (_is_counter_clockwise_(vertices))
    {
        // subpath is counter-clockwise, use it in order
        for (int i = 0; i < vertices.size(); i++)
        {
            result_vertices.add(vertices[i]);
            idx_list.push_back(subpath_idx_begin + i);
        }
    }
    else
    {
        // subpath is clockwise, use it in reversed order
        for (int i = vertices.size() - 1; i >= 0; i--)
        {
            result_vertices.add(vertices[i]);
            idx_list.push_back(subpath_idx_begin + i);
        }
    }

    jassert(idx_list.size() == vertices.size());

    // do triangulate
    _triangulate_(idx_list, result_vertices, result_indices);
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

    //if (_is_counter_clockwise_(vertices))
    if (true)
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
    IndexList idx_list;

    for (int i_edge = 0; i_edge < orig_edges.size(); i_edge++)
    {
        EdgeRebuildTmp& edge = orig_edges[i_edge];

        // do sort
        if (edge.idx_interm.size() > 2)
        {
            IntermVtxSorter sorter(edge, result_vertices);
            edge.idx_interm.sort(sorter);
        }

        // recored vertex connection
        idx_list.push_back(edge.idx_begin);
        for (int i_idx = 0; i_idx < edge.idx_interm.size(); i_idx++)
        {
            idx_list.push_back(edge.idx_interm[i_idx]);
        }

        // the last point is the begin point of next edge or first edge
        // so we don't need to append it
    }

    // do triangulate
    _triangulate_(idx_list, result_vertices, result_indices);
}


} // namespace treeface
