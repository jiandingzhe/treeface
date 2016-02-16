#include "treeface/scene/guts/Geometry_guts.h"

#include <treecore/ArrayRef.h>

using namespace treecore;

namespace treeface
{

Geometry::Guts::Guts( const VertexTemplate& vtx_temp, GLPrimitive primitive, bool is_dynamic )
    : vtx_temp( vtx_temp )
    , primitive( primitive )
    , dynamic( is_dynamic )
    , buf_vtx( new GLBuffer( TFGL_BUFFER_VERTEX, is_dynamic ? TFGL_BUFFER_DYNAMIC_DRAW : TFGL_BUFFER_STATIC_DRAW ) )
    , buf_idx( new GLBuffer( TFGL_BUFFER_INDEX,  is_dynamic ? TFGL_BUFFER_DYNAMIC_DRAW : TFGL_BUFFER_STATIC_DRAW ) )
    , host_data_vtx( vtx_temp.vertex_size() )
{}

Geometry::Guts::~Guts()
{
    jassert( user_head == nullptr );
    jassert( user_tail == nullptr );
}

void Geometry::Guts::upload_data()
{
    jassert( !drawing );
    jassert( buf_vtx->is_bound() );
    jassert( buf_idx->is_bound() );

    if (dirty)
    {
        num_idx = host_data_idx.size();
        buf_vtx->upload_data( host_data_vtx.get_raw_data_ptr(), host_data_vtx.num_byte() );
        buf_idx->upload_data( ArrayRef<IndexType>( host_data_idx ) );

        if (!dynamic)
        {
            host_data_vtx.clear();
            host_data_idx.clear();
        }

        dirty = false;
    }
}

void Geometry::Guts::invalidate_user_uniform_cache()
{
    for (VisualObject::Impl* curr = user_head; curr != nullptr; curr = curr->same_geom_next)
        curr->uniform_cache_dirty = true;

}

} // namespace treeface
