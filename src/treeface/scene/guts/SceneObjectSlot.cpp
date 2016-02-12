#include "treeface/scene/guts/SceneObjectSlot.h"

#include "treeface/gl/Program.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/VisualObject.h"

using namespace treecore;

namespace treeface
{

SceneObjectSlot::SceneObjectSlot( SceneObjectSlot&& peer )
    : cached_uniforms( std::move( peer.cached_uniforms ) )
    , uniform_cache_dirty( peer.uniform_cache_dirty )
    , obj( std::move( peer.obj ) )
{}

SceneObjectSlot& SceneObjectSlot::operator =( SceneObjectSlot&& peer )
{
    cached_uniforms     = std::move( peer.cached_uniforms );
    uniform_cache_dirty = peer.uniform_cache_dirty;
    obj = std::move( peer.obj );
    return *this;
}

void SceneObjectSlot::update_uniform_cache( SceneNode* node )
{
    jassert( uniform_cache_dirty );

    // collect uniforms from VisualObject and SceneNode
    // uniforms in geometry is collected via VisualObject's collect method
    UniformMap collect;

    const VisualObject* vis_obj = dynamic_cast<VisualObject*>( obj.get() );
    if (vis_obj != nullptr)
        vis_obj->collect_uniforms( collect );

    node->collect_uniforms( collect );

    // store result
    cached_uniforms.clear();
    for (UniformMap::ConstIterator it( collect ); it.next(); )
    {
        GLint loc = vis_obj->get_material()->get_program()->get_uniform_location( it.key() );
        if (loc > 0) cached_uniforms.add( UniformKV( loc, it.value() ) );
    }
}

bool _is_sorted_( const Array<SceneObjectSlot>& array ) noexcept
{
    for (int i = 1; i < array.size(); i++)
    {
        if ( pointer_sized_uint( array[i - 1].obj.get() ) >= pointer_sized_uint( array[i].obj.get() ) )
            return false;
    }
    return true;
}

bool _bin_search_( const Array<SceneObjectSlot>& array, const SceneObject* ptr, int32& result ) noexcept
{
    if (array.size() == 0)
    {
        result = 0;
        return false;
    }

    int32 bound_lo = 0;
    int32 bound_hi = array.size() - 1;
    int32 idx      = (bound_lo + bound_hi) / 2;

    for (;; )
    {
        jassert( 0 <= idx && idx < array.size() );

        if ( ptr == array[idx].obj.get() )
        {
            result = idx;
            return true;
        }
        else if ( ptr < array[idx].obj.get() )
        {
            if (idx == bound_lo)
            {
                if (0 < idx)
                    jassert( array[idx - 1].obj < ptr );
                result = idx;
                return false;
            }
            else
            {
                jassert( bound_lo < idx );
                bound_hi = idx - 1;
            }
        }
        else if ( ptr > array[idx].obj.get() )
        {
            if (idx == bound_hi)
            {
                if (idx < array.size() - 1)
                    jassert( ptr < array[idx + 1].obj );
                result = idx + 1;
                return false;
            }
            else
            {
                jassert( idx < bound_hi );
                bound_lo = idx + 1;
            }
        }

        idx = (bound_lo + bound_hi) / 2;
    }
}

bool SceneObjectSlotArray::add( SceneObject* obj )
{
    int32 idx = -1;
    if ( _bin_search_( m_data, obj, idx ) )
        return false;

    m_data.insert( idx, SceneObjectSlot( obj ) );
    jassert( _is_sorted_( m_data ) );
    return true;
}

bool SceneObjectSlotArray::remove( SceneObject* obj )
{
    int32 idx = -1;
    if ( _bin_search_( m_data, obj, idx ) )
    {
        m_data.removeRange( idx, 1 );
        jassert( !contains( obj ) );
        jassert( _is_sorted_( m_data ) );
        return true;
    }
    else
    {
        return false;
    }
}

bool SceneObjectSlotArray::contains( const SceneObject* obj ) const noexcept
{
    int32 idx = -1;
    return _bin_search_( m_data, obj, idx );
}

} // namespace treeface
