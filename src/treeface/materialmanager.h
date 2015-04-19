#ifndef TREEFACE_MATERIAL_MANAGER_H
#define TREEFACE_MATERIAL_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Result.h>
#include <treejuce/Singleton.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN
class Material;

class MaterialManager
{
public:
    JUCE_DECLARE_NON_COPYABLE(MaterialManager);
    JUCE_DECLARE_NON_MOVABLE(MaterialManager);
    juce_DeclareSingleton(MaterialManager, false);

    treejuce::Result get_material(const treejuce::String& name, Material** mat);
    bool material_is_cached(const treejuce::String& name);
    bool release_material_hold(const treejuce::String& name);

protected:
    struct Impl;

    MaterialManager();
    ~MaterialManager();

    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MATERIAL_MANAGER_H
