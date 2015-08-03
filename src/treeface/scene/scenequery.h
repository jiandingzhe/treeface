#ifndef TREEFACE_SCENE_QUERY_H
#define TREEFACE_SCENE_QUERY_H

#include "treeface/common.h"

#include <treecore/Object.h>

namespace treecore {
class Result;
} // namespace treecore

namespace treeface {
class SceneNode;

class SceneQuery: public treecore::Object
{
public:
    SceneQuery();

    JUCE_DECLARE_NON_COPYABLE(SceneQuery);

    virtual ~SceneQuery();

    treecore::Result traverse(SceneNode* root) NOEXCEPT;

protected:
    virtual treecore::Result traverse_begin() NOEXCEPT = 0;
    virtual treecore::Result traverse_one_node(SceneNode* node) NOEXCEPT = 0;
    virtual treecore::Result traverse_end() NOEXCEPT = 0;

private:
    treecore::Result recur_part(SceneNode* node) NOEXCEPT;
};

} // namespace treeface

#endif // TREEFACE_SCENE_QUERY_H
