#ifndef TREEFACE_MATERIAL_H
#define TREEFACE_MATERIAL_H

#include "treeface/common.h"

#include <treecore/Holder.h>
#include <treecore/IntTypes.h>
#include <treecore/Object.h>

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

class Material: public treecore::Object
{
    friend class MaterialManager;
    friend class SceneRenderer;

public:
    Material();

    virtual ~Material();

    Program* get_program() NOEXCEPT;
    treecore::int32 get_num_textures() const NOEXCEPT;
    Texture* get_texture(treecore::int32 layer_idx) NOEXCEPT;
    Texture* get_texture(treecore::StringRef name) NOEXCEPT;

    void use() NOEXCEPT;
    void unuse() NOEXCEPT;

    JUCE_DECLARE_NON_COPYABLE(Material);
    JUCE_DECLARE_NON_MOVABLE(Material);

protected:
    treecore::Holder<Program> m_program;

private:
    struct Impl;

    Impl* m_impl = nullptr;
}; // class Material

} // namespace treeface

#endif // TREEFACE_MATERIAL_H
