#ifndef TREEFACE_SCENE_RENDERER_H
#define TREEFACE_SCENE_RENDERER_H

#include "treeface/scene/SceneQuery.h"

#include "treeface/math/Mat4.h"

namespace treeface {

class Scene;

class SceneRenderer: public SceneQuery
{
public:
    SceneRenderer();
    virtual ~SceneRenderer();

    TREECORE_DECLARE_NON_COPYABLE(SceneRenderer);
    TREECORE_DECLARE_NON_MOVABLE(SceneRenderer);

    void render(const Mat4f& matrix_proj,
                const Mat4f& matrix_view,
                Scene* scene);

protected:
    virtual treecore::Result traverse_begin() noexcept;
    virtual treecore::Result traverse_one_node(SceneNode* node) noexcept;
    virtual treecore::Result traverse_end() noexcept;

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_SCENE_RENDERER_H
