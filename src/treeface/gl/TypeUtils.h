#ifndef TREEFACE_GL_TYPE_H
#define TREEFACE_GL_TYPE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/base/Common.h"

#include <treecore/IntTypes.h>

namespace treeface {

/**
 * @brief helper class to get type at compile time from OpenGL type enums.
 */
template<GLenum GL_TYPE_ENUM_VALUE>
struct GLTypeHelper;

template<typename T>
struct GLTypeEnumHelper;

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

int size_of_gl_type( GLenum type );

template<>
struct GLTypeEnumHelper<GLbyte>
{
    static const GLenum value = GL_BYTE;
};

template<>
struct GLTypeEnumHelper<GLubyte>
{
    static const GLenum value = GL_UNSIGNED_BYTE;
};

template<>
struct GLTypeEnumHelper<GLshort>
{
    static const GLenum value = GL_SHORT;
};

template<>
struct GLTypeEnumHelper<GLushort>
{
    static const GLenum value = GL_UNSIGNED_SHORT;
};

template<>
struct GLTypeEnumHelper<GLint>
{
    static const GLenum value = GL_INT;
};

template<>
struct GLTypeEnumHelper<GLuint>
{
    static const GLenum value = GL_UNSIGNED_INT;
};

template<>
struct GLTypeEnumHelper<GLfloat>
{
    static const GLenum value = GL_FLOAT;
};

template<>
struct GLTypeEnumHelper<GLdouble>
{
    static const GLenum value = GL_DOUBLE;
};

} // namespace treeface

#endif // TREEFACE_GL_TYPE_H
