#ifndef TREEFACE_MATERIAL_MANAGER_H
#define TREEFACE_MATERIAL_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Holder.h>
#include <treejuce/Object.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN
class Material;

class MaterialManager: public treejuce::Object
{
public:
    MaterialManager();

    JUCE_DECLARE_NON_COPYABLE(MaterialManager);
    JUCE_DECLARE_NON_MOVABLE(MaterialManager);

    virtual ~MaterialManager();

    treejuce::Result build_material(const treejuce::var& data, treejuce::Holder<Material>& mat);

    treejuce::Result get_material(const treejuce::String& name, Material** mat_pp);

    treejuce::Result get_material(const treejuce::String& name, treejuce::Holder<Material>& mat);

    bool material_is_cached(const treejuce::String& name);
    bool release_material_hold(const treejuce::String& name);

protected:
    struct Impl;

    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_MATERIAL_MANAGER_H
