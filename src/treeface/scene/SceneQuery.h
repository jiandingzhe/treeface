#ifndef TREEFACE_SCENE_QUERY_H
#define TREEFACE_SCENE_QUERY_H

#include "treeface/base/Common.h"

#include <treecore/RefCountObject.h>

namespace treecore {
class Result;
} // namespace treecore

namespace treeface {
class SceneNode;

class SceneQuery: public treecore::RefCountObject
{
public:
    SceneQuery();

    TREECORE_DECLARE_NON_COPYABLE(SceneQuery);

    virtual ~SceneQuery();

    treecore::Result traverse(SceneNode* root) noexcept;

protected:
    virtual treecore::Result traverse_begin() noexcept = 0;
    virtual treecore::Result traverse_one_node(SceneNode* node) noexcept = 0;
    virtual treecore::Result traverse_end() noexcept = 0;

private:
    treecore::Result recur_part(SceneNode* node) noexcept;
};

} // namespace treeface

#endif // TREEFACE_SCENE_QUERY_H
