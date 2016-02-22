#ifndef TREEFACE_MATERIAL_H
#define TREEFACE_MATERIAL_H

#include "treeface/base/Common.h"

#include <treecore/RefCountHolder.h>
#include <treecore/Identifier.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class Result;
class StringRef;
class var;
} // namespace treecore

namespace treeface {

class MaterialManager;
class Program;
class SceneRenderer;
class Texture;

class Material: public treecore::RefCountObject
{
    friend class MaterialManager;
    friend class SceneRenderer;

public:
    Material();

    virtual ~Material();

    void            init( Program* program ) noexcept;
    Program*        get_program() const noexcept;
    treecore::int32 get_num_textures() const noexcept;
    bool            add_texture( const treecore::Identifier& name, Texture* tex );
    Texture*        get_texture( treecore::int32 layer_idx ) const noexcept;
    Texture*        get_texture( const treecore::Identifier& name ) const noexcept;
    bool            remove_texture( const treecore::Identifier& name );

    void bind() noexcept;
    void unbind() noexcept;

    TREECORE_DECLARE_NON_COPYABLE( Material );
    TREECORE_DECLARE_NON_MOVABLE( Material );

protected:
    virtual treecore::String get_shader_source_addition() const noexcept;

    treecore::RefCountHolder<Program> m_program;

private:
    struct Impl;

    Impl* m_impl = nullptr;
};  // class Material

} // namespace treeface

#endif // TREEFACE_MATERIAL_H
