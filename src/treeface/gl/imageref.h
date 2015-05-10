#ifndef TREEFACE_GL_IMAGE_REF_H
#define TREEFACE_GL_IMAGE_REF_H

#include "treeface/common.h"

#include <GL/glew.h>

TREEFACE_NAMESPACE_BEGIN

/**
 * @brief The ImageRef struct holds image metadata for OpenGL.
 *
 * These information is enough for the image to be used as textures. The raw
 * data is not owned by ImageRef, and will be kept untouched when ImageRef is
 * destroyed.
 */
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
