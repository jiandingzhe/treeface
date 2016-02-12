#ifndef TREEFACE_SCENE_OBJECT_SLOT_H
#define TREEFACE_SCENE_OBJECT_SLOT_H

#include "treeface/scene/guts/Utils.h"
#include "treeface/scene/SceneObject.h"

#include <treecore/Array.h>

namespace treeface
{

struct SceneObjectSlot
{
    SceneObjectSlot( SceneObject* obj ): obj( obj ) {}
    SceneObjectSlot( SceneObjectSlot&& peer );

    SceneObjectSlot& operator =( SceneObjectSlot&& peer );

    TREECORE_DECLARE_NON_COPYABLE( SceneObjectSlot )

    void update_uniform_cache( SceneNode* node );

    // cached uniforms are stored in a continuous list, which would make faster
    // traversion in rendering progress
    treecore::Array<UniformKV> cached_uniforms;
    bool uniform_cache_dirty = true;
    treecore::RefCountHolder<SceneObject> obj;
};

class SceneObjectSlotArray
{
public:
    bool add( SceneObject* obj );
    bool remove( SceneObject* obj );
    bool contains( const SceneObject* obj ) const noexcept;

    SceneObjectSlot*       begin() noexcept       { return m_data.begin(); }
    const SceneObjectSlot* begin() const noexcept { return m_data.begin(); }
    SceneObjectSlot*       end() noexcept         { return m_data.end(); }
    const SceneObjectSlot* end() const noexcept   { return m_data.end(); }

    int32                  size() const noexcept                   { return m_data.size(); }
    SceneObjectSlot&       operator []( int32 idx ) noexcept       { return m_data[idx]; }
    const SceneObjectSlot& operator []( int32 idx ) const noexcept { return m_data[idx]; }
private:
    treecore::Array<SceneObjectSlot> m_data;
};

} // namespace treeface

#endif // TREEFACE_SCENE_OBJECT_SLOT_H
