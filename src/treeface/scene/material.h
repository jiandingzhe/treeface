#ifndef TREEFACE_SCENE_MATERIAL_H
#define TREEFACE_SCENE_MATERIAL_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_NAMESPACE_BEGIN

class Material: public treejuce::Object
{
public:
    Material();
    virtual ~Material();

    JUCE_DECLARE_NON_COPYABLE(Material);
    JUCE_DECLARE_NON_MOVABLE(Material);

protected:
    struct Impl;

    Impl* m_impl = nullptr;
}; // class Material

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_MATERIAL_H
