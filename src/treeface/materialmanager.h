#ifndef TREEFACE_MATERIAL_MANAGER_H
#define TREEFACE_MATERIAL_MANAGER_H

#include "treeface/common.h"

#include <treecore/RefCountHolder.h>
#include <treecore/RefCountObject.h>
#include <treecore/Result.h>
#include <treecore/String.h>

namespace treecore {
class var;
} // namespace treecore

namespace treeface {
class Material;

class MaterialManager: public treecore::RefCountObject
{
public:
    MaterialManager();

    TREECORE_DECLARE_NON_COPYABLE(MaterialManager);
    TREECORE_DECLARE_NON_MOVABLE(MaterialManager);

    virtual ~MaterialManager();

    Material* build_material(const treecore::String& name, const treecore::var& data);

    Material* get_material(const treecore::String& name);

    bool material_is_cached(const treecore::String& name);
    bool release_material_hold(const treecore::String& name);

protected:
    struct Impl;

    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_MATERIAL_MANAGER_H
