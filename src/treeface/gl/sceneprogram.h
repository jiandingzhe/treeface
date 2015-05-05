#ifndef TREEFACE_SCENE_PROGRAM_H
#define TREEFACE_SCENE_PROGRAM_H

#include "treeface/gl/program.h"

TREEFACE_NAMESPACE_BEGIN

class SceneProgram: public Program
{
public:
    SceneProgram();

    JUCE_DECLARE_NON_COPYABLE(SceneProgram);
    JUCE_DECLARE_NON_MOVABLE(SceneProgram);

    virtual ~SceneProgram();

protected:
    virtual void preprocess_shader_source(treejuce::String& src_vertex, treejuce::String& src_fragment) const override;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_SCENE_PROGRAM_H
