#ifndef TREEFACE_GL_IMAGE_REF_H
#define TREEFACE_GL_IMAGE_REF_H

#include "treeface/base/Common.h"

#include "treeface/gl/Enums.h"

namespace treeface {

/**
 * @brief The ImageRef struct holds image metadata for OpenGL.
 *
 * These information is enough for the image to be used as textures. The raw
 * data is not owned by ImageRef, and will be kept untouched when ImageRef is
 * destroyed.
 */
struct TextureCompatibleImageRef
{
    GLImageFormat format;
    GLInternalImageFormat internal_format;
    GLImageDataType  type;
    GLsizei width;
    GLsizei height;
    void*   data;
};

struct TextureCompatibleImageArrayRef
{
    GLImageFormat format;
    GLInternalImageFormat internal_format;
    GLImageDataType  type;
    GLsizei width;
    GLsizei height;
    GLsizei num_frame;
    void*   data;
};

struct TextureCompatibleVoxelBlockRef
{
    GLImageFormat format;
    GLInternalImageFormat internal_format;
    GLImageDataType  type;
    GLsizei width;
    GLsizei height;
    GLsizei depth;
    void*   data;
};

} // namespace treeface

#endif // TREEFACE_GL_IMAGE_REF_H
