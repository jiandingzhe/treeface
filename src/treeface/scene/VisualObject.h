#ifndef TREEFACE_VISUAL_OBJECT_H
#define TREEFACE_VISUAL_OBJECT_H

#include "treeface/scene/SceneObject.h"

#include <treecore/HashMap.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class Identifier;
class Result;
class var;
} // namespace treecore

namespace treeface {

class SceneGraphMaterial;
class Geometry;
class UniversalValue;
class VertexArray;

class VisualObject: public SceneObject
{
    friend class SceneRenderer;

public:
    VisualObject( Geometry* geom, SceneGraphMaterial* mat );
    virtual ~VisualObject();

    TREECORE_DECLARE_NON_COPYABLE( VisualObject )
    TREECORE_DECLARE_NON_MOVABLE( VisualObject )

    void  set_uniform_value( const treecore::Identifier& name, const UniversalValue& value );
    bool  get_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept;
    bool  has_uniform( const treecore::Identifier& name ) const noexcept;
    int32 collect_uniforms( treecore::HashMap<treecore::Identifier, UniversalValue>& store ) const;

    SceneGraphMaterial* get_material() const noexcept;
    Geometry*           get_geometry() const noexcept;
    VertexArray*        get_vertex_array() const noexcept;
    void                render() noexcept;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_VISUAL_OBJECT_H
