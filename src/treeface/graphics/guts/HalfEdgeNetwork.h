#ifndef TREEFACE_HALF_EDGE_NETWORK_H
#define TREEFACE_HALF_EDGE_NETWORK_H

#include "treeface/graphics/HalfEdge.h"
#include "treeface/graphics/guts/Enums.h"

#include "treeface/math/Vec2.h"

#include "treeface/scene/Geometry.h"

#include <treecore/Array.h>

namespace treeface
{

struct HalfEdgeNetwork
{
    explicit HalfEdgeNetwork( const Geometry::HostVertexCache& vertices ): vertices( vertices ) {}

    void build_half_edges( const treecore::Array<IndexType>& subpath_begin, bool is_cclw );

    void connect( IndexType i_edge1, IndexType i_edge2 );

    bool fan_is_facing( const Vec2f& vec_ref, IndexType i_edge ) const;

    void get_edge_vertical_order( const treecore::Array<VertexRole>& roles, treecore::Array<IndexType>& result ) const;

    void get_edge_role( treecore::Array<VertexRole>& result_roles ) const;

    IndexType get_next_edge_diff_vtx( const IndexType i_edge_search_base ) const;
    IndexType get_prev_edge_diff_vtx( const IndexType i_edge_search_base ) const;

    void iter_edge_to_facing_fan( const IndexType i_edge_ref, IndexType& i_edge_search ) const;

    void partition_polygon_monotone( HalfEdgeNetwork& result_network ) const;

    const Vec2f& get_edge_vertex( IndexType i_edge ) const noexcept
    {
        return vertices.get<Vec2f>( edges[i_edge].idx_vertex );
    }

    treecore::int16 mark_polygons( treecore::Array<treecore::int16>& edge_polygon_map ) const;

    void triangulate_monotone_polygons( const treecore::Array<IndexType>&       edge_indices_by_y,
                                        const treecore::Array<treecore::int16>& edge_polygon_map,
                                        treecore::int16 num_polygon,
                                        const treecore::Array<VertexRole>&      edge_roles,
                                        treecore::Array<IndexType>&             result_indices,
                                        Vec2f& result_skeleton_min,
                                        Vec2f& result_skeleton_max ) const;

    const Geometry::HostVertexCache& vertices;
    treecore::Array<HalfEdge>        edges;
};

} // namespace treeface

#endif // TREEFACE_HALF_EDGE_NETWORK_H
