#ifndef TREEFACE_GL_TYPE_H
#define TREEFACE_GL_TYPE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/common.h"

TREEFACE_NAMESPACE_BEGIN

/**
 * @brief helper class to get type at compile time from OpenGL type enums.
 */
template<GLenum GL_TYPE_ENUM_VALUE>
struct GLTypeHelper
{};

template<>
struct GLTypeHelper<GL_BOOL>
{
    typedef GLboolean Type;
};

template<>
struct GLTypeHelper<GL_BYTE>
{
    typedef GLbyte Type;
};

template<>
struct GLTypeHelper<GL_UNSIGNED_BYTE>
{
    typedef GLubyte Type;
};

template<>
struct GLTypeHelper<GL_SHORT>
{
    typedef GLshort Type;
};

template<>
struct GLTypeHelper<GL_UNSIGNED_SHORT>
{
    typedef GLushort Type;
};

template<>
struct GLTypeHelper<GL_INT>
{
    typedef GLint Type;
};

template<>
struct GLTypeHelper<GL_UNSIGNED_INT>
{
    typedef GLuint Type;
};

template<>
struct GLTypeHelper<GL_FLOAT>
{
    typedef GLfloat Type;
};

template<>
struct GLTypeHelper<GL_DOUBLE>
{
    typedef GLdouble Type;
};

int size_of_gl_type(GLenum type);


TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_TYPE_H
