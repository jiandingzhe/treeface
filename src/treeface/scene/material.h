#ifndef TREEFACE_SCENE_MATERIAL_H
#define TREEFACE_SCENE_MATERIAL_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class Material: public treejuce::Object
{
public:
    Material();
    Material(const treejuce::var& root_node);

    virtual ~Material();

    treejuce::Result init(const treejuce::var& root_node);

    JUCE_DECLARE_NON_COPYABLE(Material);
    JUCE_DECLARE_NON_MOVABLE(Material);

protected:
    struct Impl;

    Impl* m_impl = nullptr;
}; // class Material

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_MATERIAL_H
