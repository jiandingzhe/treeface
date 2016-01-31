#ifndef TREEFACE_SCENE_NODE_H
#define TREEFACE_SCENE_NODE_H

#include "treeface/base/Common.h"

#include "treeface/math/Mat4.h"

#include <treecore/ArrayRef.h>
#include <treecore/HashMap.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class Identifier;
class Result;
class var;
} // namespace treecore

namespace treeface {

class SceneObject;
class SceneNodeManager;
class UniversalValue;

class SceneNode: public treecore::RefCountObject
{
    friend class SceneNodeManager;
    friend class SceneRenderer;

public:
    SceneNode();

    TREECORE_DECLARE_NON_COPYABLE( SceneNode )
    TREECORE_DECLARE_NON_MOVABLE( SceneNode )

    virtual ~SceneNode();

    const Mat4f& get_transform() noexcept;
    const Mat4f& get_global_transform() noexcept;
    const Mat4f& get_transform_inv() noexcept;
    const Mat4f& get_global_transform_inv() noexcept;

    void set_transform( const Mat4f& value ) noexcept;

    bool         add_item( SceneObject* obj );
    bool         has_item( SceneObject* obj ) const noexcept;
    bool         remove_item( SceneObject* obj );
    int32        get_num_items() const noexcept;
    SceneObject* get_item_at( int idx ) noexcept;

    bool get_self_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept;
    bool get_uniform_value( const treecore::Identifier& name, UniversalValue& result );
    void set_uniform_value( const treecore::Identifier& name, const UniversalValue& value );
    bool has_self_uniform( const treecore::Identifier& name ) const noexcept;
    bool has_uniform( const treecore::Identifier& name );

    int32 collect_uniforms( treecore::HashMap<treecore::Identifier, UniversalValue>& result );

    bool       add_child( SceneNode* child );
    bool       has_child( SceneNode* child ) const noexcept;
    bool       remove_child( SceneNode* child ) noexcept;
    int32      get_num_children() const noexcept;
    SceneNode* get_child_at( int idx ) noexcept;
    SceneNode* get_parent() noexcept;

private:
    struct Guts;
    Guts* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_NODE_H
