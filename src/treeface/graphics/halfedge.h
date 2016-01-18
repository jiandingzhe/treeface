#ifndef TREEFACE_HALF_EDGE_H
#define TREEFACE_HALF_EDGE_H

#include "treeface/gl/type.h"
#include "treeface/math/vec2.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{

struct HalfEdge
{
    IndexType idx_vertex;
    IndexType idx_prev_edge;
    IndexType idx_next_edge;
    IndexType idx_peer_edge;

    const Vec2f&    get_vertex( const Geometry::HostVertexCache& store ) const { return store.get<Vec2f>( idx_vertex ); }
    const HalfEdge& get_prev( const treecore::Array<HalfEdge>& store ) const   { return store[idx_prev_edge]; }
    const HalfEdge& get_next( const treecore::Array<HalfEdge>& store ) const   { return store[idx_next_edge]; }
    const HalfEdge& get_peer( const treecore::Array<HalfEdge>& store ) const   { return store[idx_peer_edge]; }
};

} // namespace treeface

#endif // TREEFACE_HALF_EDGE_H
