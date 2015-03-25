#ifndef TREEFACE_GL_PROGRAM_H
#define TREEFACE_GL_PROGRAM_H

#include "treeface/common.h"
#include "treejuce/Object.h"
#include "GL/glew.h"

TREEFACE_NAMESPACE_BEGIN

class Program: public treejuce::Object
{
public:
    Program();

    virtual ~Program();

    virtual const char* get_shader_source_vert();
    virtual const char* get_shader_source_geom();
    virtual const char* get_shader_source_frag();

protected:
    GLuint ms_program = 0;
    GLuint ms_shader_vert = 0;
    GLuint ms_shader_geom = 0;
    GLuint ms_shader_frag = 0;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_PROGRAM_H
