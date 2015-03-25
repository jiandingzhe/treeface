#include "treeface/gl/program.h"

TREEFACE_NAMESPACE_BEGIN

Program::Program() {}

Program::~Program() {}

const char* Program::get_shader_source_vert()
{
    return nullptr;
}

const char* Program::get_shader_source_geom()
{
    return nullptr;
}

const char* Program::get_shader_source_frag()
{
    return nullptr;
}

TREEFACE_NAMESPACE_END
