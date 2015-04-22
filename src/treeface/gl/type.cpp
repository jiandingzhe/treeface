#include "treeface/gl/type.h"

#include <treejuce/StringRef.h>

TREEFACE_NAMESPACE_BEGIN

int size_of_gl_type(GLenum type)
{
    switch (type)
    {
    case GL_BYTE:
        return sizeof(GLTypeHelper<GL_BYTE>::Type);
    case GL_UNSIGNED_BYTE:
        return sizeof(GLTypeHelper<GL_UNSIGNED_BYTE>::Type);
    case GL_SHORT:
        return sizeof(GLTypeHelper<GL_SHORT>::Type);
    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
        return sizeof(GLTypeHelper<GL_UNSIGNED_SHORT>::Type);
    case GL_INT:
        return sizeof(GLTypeHelper<GL_INT>::Type);
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
        return sizeof(GLTypeHelper<GL_UNSIGNED_INT>::Type);
    case GL_FLOAT:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type);
    case GL_FLOAT_VEC2:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 2;
    case GL_FLOAT_VEC3:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 3;
    case GL_FLOAT_VEC4:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 4;
    case GL_FLOAT_MAT2:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 2 * 2;
    case GL_FLOAT_MAT3:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 3 * 3;
    case GL_FLOAT_MAT4:
        return sizeof(GLTypeHelper<GL_FLOAT>::Type) * 4 * 4;
    case GL_DOUBLE:
        return sizeof(GLTypeHelper<GL_DOUBLE>::Type);
    default:
        die("attempt to get type size using unknown/unsupported enum: %x", type);
    }
}

TREEFACE_NAMESPACE_END
