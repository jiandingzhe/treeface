#include "treeface/scene/guts/VisualObject_guts.h"

#include "treeface/gl/Program.h"
#include "treeface/gl/VertexArray.h"
#include "treeface/scene/guts/Geometry_guts.h"
#include "treeface/scene/SceneGraphMaterial.h"

namespace treeface
{

VisualObject::Impl::Impl( Geometry* geom, SceneGraphMaterial* mat )
    : material( mat )
    , geometry( geom )
    , vertex_array( new VertexArray( geom->get_vertex_buffer(), geom->get_index_buffer(), geom->get_vertex_template(), mat->get_program() ) )
{
    if (geom->m_impl->user_tail == nullptr)
    {
        jassert( geom->m_impl->user_head == nullptr );
        geom->m_impl->user_head = this;
        geom->m_impl->user_tail = this;
    }
    else
    {
        jassert( geom->m_impl->user_head != nullptr );
        Impl* prev_tail = geom->m_impl->user_tail;
        prev_tail->same_geom_next = this;
        geom->m_impl->user_tail   = this;
        same_geom_prev = prev_tail;
    }
}

VisualObject::Impl::~Impl()
{
    if (same_geom_next == nullptr)
    {
        jassert( geometry->m_impl->user_tail == this );
        geometry->m_impl->user_tail = same_geom_prev;
    }
    else
    {
        same_geom_next->same_geom_prev = same_geom_prev;
    }

    if (same_geom_prev == nullptr)
    {
        jassert( geometry->m_impl->user_head == this );
        geometry->m_impl->user_head = same_geom_next;
    }
    else
    {
        same_geom_prev->same_geom_next = same_geom_next;
    }
}

void VisualObject::Impl::update_uniform_cache()
{
    jassert( uniform_cache_dirty );

    // collect uniforms
    UniformMap cache_map = uniforms;
    collect_uniforms( geometry->m_impl->uniforms, cache_map );

    // update cache
    cached_uniforms.clearQuick();
    for (UniformMap::ConstIterator it( cache_map ); it.next(); )
    {
        GLint uni_loc = material->get_program()->get_uniform_location( it.key() );
        cached_uniforms.add( { uni_loc, it.value() } );
    }
    uniform_cache_dirty = false;
}

} // namespace treeface
