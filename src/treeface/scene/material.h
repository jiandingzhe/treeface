#ifndef TREEFACE_SCENE_MATERIAL_H
#define TREEFACE_SCENE_MATERIAL_H

#include "treeface/common.h"

#include <treejuce/IntTypes.h>
#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class StringRef;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class Program;
class Texture;
class Renderer;

class Material: public treejuce::Object
{
    friend class Renderer;

public:
    Material();

    virtual ~Material();

    treejuce::Result build(const treejuce::var& root_node);

    Program* get_program() NOEXCEPT;
    treejuce::int32 get_num_textures() const NOEXCEPT;
    Texture* get_texture(treejuce::int32 layer_idx) NOEXCEPT;
    Texture* get_texture(treejuce::StringRef name) NOEXCEPT;

    JUCE_DECLARE_NON_COPYABLE(Material);
    JUCE_DECLARE_NON_MOVABLE(Material);

protected:
    struct Impl;

    Impl* m_impl = nullptr;
}; // class Material

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_MATERIAL_H
