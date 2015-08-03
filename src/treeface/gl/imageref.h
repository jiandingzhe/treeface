#ifndef TREEFACE_GL_IMAGE_REF_H
#define TREEFACE_GL_IMAGE_REF_H

#include "treeface/common.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface {

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

} // namespace treeface

#endif // TREEFACE_GL_IMAGE_REF_H
