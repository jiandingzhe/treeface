#ifndef TREEFACE_GL_IMAGE_REF_H
#define TREEFACE_GL_IMAGE_REF_H

#include "treeface/common.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

struct ImageRef
{
    GLenum format;
    GLenum type;
    int width;
    int height;
    void* data;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GL_IMAGE_REF_H
