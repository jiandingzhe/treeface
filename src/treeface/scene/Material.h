#ifndef TREEFACE_MATERIAL_H
#define TREEFACE_MATERIAL_H

#include "treeface/base/Common.h"

#include <treecore/ClassUtils.h>
#include <treecore/RefCountHolder.h>
#include <treecore/Identifier.h>
#include <treecore/RefCountObject.h>

namespace treecore {
class Identifier;
class Result;
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

    ///
    /// \brief create an empty material object
    ///
    Material();

    virtual ~Material();

    ///
    /// \brief init
    /// \param program
    ///
    virtual void    init( Program* program );
    Program*        get_program() const noexcept;
    treecore::int32 get_num_textures() const noexcept;
    bool            add_texture( const treecore::Identifier& name, Texture* tex );
    Texture*        get_texture( treecore::int32 layer_idx ) const noexcept;
    Texture*        get_texture( const treecore::Identifier& uniform_name ) const noexcept;
    bool            remove_texture( const treecore::Identifier& uniform_sname );

    void bind() noexcept;
    void unbind() noexcept;

    TREECORE_DECLARE_NON_COPYABLE( Material )
    TREECORE_DECLARE_NON_MOVABLE( Material )

protected:
    virtual treecore::String get_shader_source_addition() const noexcept;

    treecore::RefCountHolder<Program> m_program;

private:
    struct Impl;

    Impl* m_impl = nullptr;
};  // class Material

} // namespace treeface

#endif // TREEFACE_MATERIAL_H
