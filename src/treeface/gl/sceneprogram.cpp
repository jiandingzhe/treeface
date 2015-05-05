#include "treeface/gl/sceneprogram.h"

#include <treejuce/String.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

SceneProgram::SceneProgram() {}

SceneProgram::~SceneProgram() {}

const char* SCENE_PROGRAM_SRC_ADDITION =
        "#version 300 es\n"
        "uniform highp mat4 matrix_model_view;\n"
        "uniform highp mat4 matrix_project;\n"
        "uniform highp mat4 matrix_normal;\n"
        "uniform lowp vec3 main_light_direction;\n"
        "uniform lowp vec3 main_light_color;\n"
        "\n"
        ;

void SceneProgram::preprocess_shader_source(treejuce::String& src_vertex, treejuce::String& src_fragment) const
{
    src_vertex = SCENE_PROGRAM_SRC_ADDITION + src_vertex;
    src_fragment = SCENE_PROGRAM_SRC_ADDITION + src_fragment;
}

TREEFACE_NAMESPACE_END
