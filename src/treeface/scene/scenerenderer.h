#ifndef TREEFACE_SCENE_RENDERER_H
#define TREEFACE_SCENE_RENDERER_H

#include "treeface/scene/scenequery.h"

#include "treeface/math/mat4.h"

TREEFACE_NAMESPACE_BEGIN

class Scene;

class SceneRenderer: public SceneQuery
{
public:
    SceneRenderer();
    virtual ~SceneRenderer();

    JUCE_DECLARE_NON_COPYABLE(SceneRenderer);
    JUCE_DECLARE_NON_MOVABLE(SceneRenderer);

    void render(const Mat4f& matrix_proj,
                const Mat4f& matrix_view,
                Scene* scene);

protected:
    virtual treejuce::Result traverse_begin() NOEXCEPT;
    virtual treejuce::Result traverse_one_node(SceneNode* node) NOEXCEPT;
    virtual treejuce::Result traverse_end() NOEXCEPT;

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_RENDERER_H
