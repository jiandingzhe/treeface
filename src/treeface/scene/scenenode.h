#ifndef TREEFACE_SCENE_NODE_H
#define TREEFACE_SCENE_NODE_H

#include "treeface/common.h"

#include "treeface/math.h"

#include <treecore/ArrayRef.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class Identifier;
class Result;
class var;
} // namespace treecore

namespace treeface {

class SceneNodeManager;
class SceneItem;
class UniversalValue;

class SceneNode: public treecore::RefCountObject
{
    friend class SceneNodeManager;

public:
    SceneNode();

    TREECORE_DECLARE_NON_COPYABLE( SceneNode );
    TREECORE_DECLARE_NON_MOVABLE( SceneNode );

    virtual ~SceneNode();

    const Mat4f& get_transform() noexcept;
    const Mat4f& get_global_transform() noexcept;
    const Mat4f& get_transform_inv() noexcept;
    const Mat4f& get_global_transform_inv() noexcept;

    void set_transform( const Mat4f& value ) noexcept;

    bool       add_item( SceneItem* obj );
    bool       has_item( SceneItem* obj ) const noexcept;
    bool       remove_item( SceneItem* obj );
    int        get_num_items() const noexcept;
    SceneItem* get_item_at( int idx ) noexcept;

    bool get_self_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept;
    bool get_uniform_value( const treecore::Identifier& name, UniversalValue& result );
    void set_uniform_value( const treecore::Identifier& name, const UniversalValue& value );
    bool has_self_uniform( const treecore::Identifier& name ) const noexcept;
    bool has_uniform( const treecore::Identifier& name );

    bool       add_child( SceneNode* child );
    bool       has_child( SceneNode* child ) const noexcept;
    bool       remove_child( SceneNode* child ) noexcept;
    int        get_num_children() const noexcept;
    SceneNode* get_child_at( int idx ) noexcept;

    SceneNode* get_parent() noexcept;
private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_NODE_H
