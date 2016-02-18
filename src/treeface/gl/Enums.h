#ifndef TREEFACE_GL_ENUMS_H
#define TREEFACE_GL_ENUMS_H

#define GLEW_STATIC
#include <GL/glew.h>

namespace treeface
{

typedef enum
{
    TFGL_BUFFER_VERTEX   = GL_ARRAY_BUFFER,
    TFGL_BUFFER_INDEX    = GL_ELEMENT_ARRAY_BUFFER,
    TFGL_BUFFER_PACK     = GL_PIXEL_PACK_BUFFER,
    TFGL_BUFFER_UNPACK   = GL_PIXEL_UNPACK_BUFFER,
    TFGL_BUFFER_READ     = GL_COPY_READ_BUFFER,
    TFGL_BUFFER_WRITE    = GL_COPY_WRITE_BUFFER,
    TFGL_BUFFER_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
    TFGL_BUFFER_UNIFORM  = GL_UNIFORM_BUFFER,
} GLBufferType;

typedef enum
{
    TFGL_TEXTURE_2D       = GL_TEXTURE_2D,
    TFGL_TEXTURE_3D       = GL_TEXTURE_3D,
    TFGL_TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TFGL_TEXTURE_CUBE     = GL_TEXTURE_CUBE_MAP,
} GLTextureType;

typedef enum
{
    TFGL_TEXTURE_CUBE_XP = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    TFGL_TEXTURE_CUBE_YP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    TFGL_TEXTURE_CUBE_ZP = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    TFGL_TEXTURE_CUBE_XN = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    TFGL_TEXTURE_CUBE_YN = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TFGL_TEXTURE_CUBE_ZN = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
} GLTextureCubeSide;

typedef enum
{
    TFGL_BUFFER_STREAM_DRAW  = GL_STREAM_DRAW,
    TFGL_BUFFER_STREAM_READ  = GL_STREAM_READ,
    TFGL_BUFFER_STREAM_COPY  = GL_STREAM_COPY,
    TFGL_BUFFER_STATIC_DRAW  = GL_STATIC_DRAW,
    TFGL_BUFFER_STATIC_READ  = GL_STATIC_READ,
    TFGL_BUFFER_STATIC_COPY  = GL_STATIC_COPY,
    TFGL_BUFFER_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    TFGL_BUFFER_DYNAMIC_READ = GL_DYNAMIC_READ,
    TFGL_BUFFER_DYNAMIC_COPY = GL_DYNAMIC_COPY,
} GLBufferUsage;

typedef enum
{
    TFGL_PRIMITIVE_POINTS = GL_POINTS,
    TFGL_PRIMITIVE_LINES  = GL_LINES,
    TFGL_PRIMITIVE_LINE_STRIP     = GL_LINE_STRIP,
    TFGL_PRIMITIVE_LINE_LOOP      = GL_LINE_LOOP,
    TFGL_PRIMITIVE_TRIANGLES      = GL_TRIANGLES,
    TFGL_PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TFGL_PRIMITIVE_TRIANGLE_FAN   = GL_TRIANGLE_FAN,
} GLPrimitive;

typedef enum
{
    TFGL_IMAGE_FORMAT_ALPHA = GL_ALPHA,
    TFGL_IMAGE_FORMAT_RGBA  = GL_RGBA,
    TFGL_IMAGE_FORMAT_RGB   = GL_RGB,
    TFGL_IMAGE_FORMAT_RG    = GL_RG,
    TFGL_IMAGE_FORMAT_RED   = GL_RED,
    TFGL_IMAGE_FORMAT_RGBA_INT        = GL_RGBA_INTEGER,
    TFGL_IMAGE_FORMAT_RGB_INT         = GL_RGB_INTEGER,
    TFGL_IMAGE_FORMAT_RG_INT          = GL_RG_INTEGER,
    TFGL_IMAGE_FORMAT_RED_INT         = GL_RED_INTEGER,
    TFGL_IMAGE_FORMAT_DEPTH           = GL_DEPTH,
    TFGL_IMAGE_FORMAT_DEPTH_STENCIL   = GL_DEPTH_STENCIL,
    TFGL_IMAGE_FORMAT_LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    TFGL_IMAGE_FORMAT_LUMINANCE       = GL_LUMINANCE,
} GLImageFormat;

typedef enum
{
    TFGL_IMAGE_DATA_BYTE = GL_BYTE,
    TFGL_IMAGE_DATA_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    TFGL_IMAGE_DATA_SHORT = GL_SHORT,
    TFGL_IMAGE_DATA_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_6_5   = GL_UNSIGNED_SHORT_5_6_5,
    TFGL_IMAGE_DATA_UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    TFGL_IMAGE_DATA_UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    TFGL_IMAGE_DATA_INT = GL_INT,
    TFGL_IMAGE_DATA_UNSIGNED_INT = GL_UNSIGNED_INT,
    TFGL_IMAGE_DATA_UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,
    TFGL_IMAGE_DATA_UNSIGNED_INT_5_9_9_9_REV     = GL_UNSIGNED_INT_5_9_9_9_REV,
    TFGL_IMAGE_DATA_UNSIGNED_INT_2_10_10_10_REV  = GL_UNSIGNED_INT_2_10_10_10_REV,
    TFGL_IMAGE_DATA_UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8,
    TFGL_IMAGE_DATA_HALF_FLOAT        = GL_HALF_FLOAT,
    TFGL_IMAGE_DATA_FLOAT = GL_FLOAT,
    TFGL_IMAGE_DATA_FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
} GLImageDataType;

typedef enum
{
    TFGL_INTERNAL_IMAGE_FORMAT_ALPHA = GL_ALPHA,
    TFGL_INTERNAL_IMAGE_FORMAT_BLUE  = GL_BLUE,
    TFGL_INTERNAL_IMAGE_FORMAT_GREEN = GL_GREEN,
    TFGL_INTERNAL_IMAGE_FORMAT_RED   = GL_RED,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB   = GL_RGB,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA  = GL_RGBA,
    TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE_ALPHA   = GL_LUMINANCE_ALPHA,
    TFGL_INTERNAL_IMAGE_FORMAT_LUMINANCE         = GL_LUMINANCE,
    TFGL_INTERNAL_IMAGE_FORMAT_DEPTH16           = GL_DEPTH_COMPONENT16,
    TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24           = GL_DEPTH_COMPONENT24,
    TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F          = GL_DEPTH_COMPONENT32F,
    TFGL_INTERNAL_IMAGE_FORMAT_DEPTH24_STENCIL8  = GL_DEPTH24_STENCIL8,
    TFGL_INTERNAL_IMAGE_FORMAT_DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
    TFGL_INTERNAL_IMAGE_FORMAT_R8                = GL_R8,
    TFGL_INTERNAL_IMAGE_FORMAT_R8_SNORM          = GL_R8_SNORM,
    TFGL_INTERNAL_IMAGE_FORMAT_R8I               = GL_R8I,
    TFGL_INTERNAL_IMAGE_FORMAT_R8UI              = GL_R8UI,
    TFGL_INTERNAL_IMAGE_FORMAT_R16F              = GL_R16F,
    TFGL_INTERNAL_IMAGE_FORMAT_R32F              = GL_R32F,
    TFGL_INTERNAL_IMAGE_FORMAT_R16UI             = GL_R16UI,
    TFGL_INTERNAL_IMAGE_FORMAT_R16I              = GL_R16I,
    TFGL_INTERNAL_IMAGE_FORMAT_R32UI             = GL_R32UI,
    TFGL_INTERNAL_IMAGE_FORMAT_R32I              = GL_R32I,
    TFGL_INTERNAL_IMAGE_FORMAT_RG8               = GL_RG8,
    TFGL_INTERNAL_IMAGE_FORMAT_RG8_SNORM         = GL_RG8_SNORM,
    TFGL_INTERNAL_IMAGE_FORMAT_RG16F             = GL_RG16F,
    TFGL_INTERNAL_IMAGE_FORMAT_RG32F             = GL_RG32F,
    TFGL_INTERNAL_IMAGE_FORMAT_RG8UI             = GL_RG8UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RG8I              = GL_RG8I,
    TFGL_INTERNAL_IMAGE_FORMAT_RG16UI            = GL_RG16UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RG16I             = GL_RG16I,
    TFGL_INTERNAL_IMAGE_FORMAT_RG32UI            = GL_RG32UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RG32I             = GL_RG32I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB8              = GL_RGB8,
    TFGL_INTERNAL_IMAGE_FORMAT_SRGB8             = GL_SRGB8,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB565            = GL_RGB565,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB8_SNORM        = GL_RGB8_SNORM,
    TFGL_INTERNAL_IMAGE_FORMAT_R11F_G11F_B10F    = GL_R11F_G11F_B10F,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB9_E5           = GL_RGB9_E5,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB16F            = GL_RGB16F,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB32F            = GL_RGB32F,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB8UI            = GL_RGB8UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB8I             = GL_RGB8I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB16UI           = GL_RGB16UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB16I            = GL_RGB16I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB32UI           = GL_RGB32UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB32I            = GL_RGB32I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA8             = GL_RGBA8,
    TFGL_INTERNAL_IMAGE_FORMAT_SRGB8_ALPHA8      = GL_SRGB8_ALPHA8,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA8_SNORM       = GL_RGBA8_SNORM,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB5_A1           = GL_RGB5_A1,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA4             = GL_RGBA4,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2          = GL_RGB10_A2,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA16F           = GL_RGBA16F,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA32F           = GL_RGBA32F,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA8UI           = GL_RGBA8UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA8I            = GL_RGBA8I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGB10_A2UI        = GL_RGB10_A2UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA16UI          = GL_RGBA16UI,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA16I           = GL_RGBA16I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA32I           = GL_RGBA32I,
    TFGL_INTERNAL_IMAGE_FORMAT_RGBA32UI          = GL_RGBA32UI,
} GLInternalImageFormat;

typedef enum
{
    TFGL_TYPE_BOOL  = GL_BOOL,
    TFGL_TYPE_BYTE  = GL_BYTE,
    TFGL_TYPE_SHORT = GL_SHORT,
    TFGL_TYPE_INT   = GL_INT,
    TFGL_TYPE_UNSIGNED_BYTE  = GL_UNSIGNED_BYTE,
    TFGL_TYPE_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    TFGL_TYPE_UNSIGNED_INT   = GL_UNSIGNED_INT,
    TFGL_TYPE_FLOAT          = GL_FLOAT,
    TFGL_TYPE_DOUBLE         = GL_DOUBLE,

    TFGL_TYPE_FLOAT_VEC2 = GL_FLOAT_VEC2,
    TFGL_TYPE_FLOAT_VEC3 = GL_FLOAT_VEC3,
    TFGL_TYPE_FLOAT_VEC4 = GL_FLOAT_VEC4,
    TFGL_TYPE_VEC2F      = GL_FLOAT_VEC2,
    TFGL_TYPE_VEC3F      = GL_FLOAT_VEC3,
    TFGL_TYPE_VEC4F      = GL_FLOAT_VEC4,

    TFGL_TYPE_INT_VEC2 = GL_INT_VEC2,
    TFGL_TYPE_INT_VEC3 = GL_INT_VEC3,
    TFGL_TYPE_INT_VEC4 = GL_INT_VEC4,
    TFGL_TYPE_VEC2I    = GL_INT_VEC2,
    TFGL_TYPE_VEC3I    = GL_INT_VEC3,
    TFGL_TYPE_VEC4I    = GL_INT_VEC4,

    TFGL_TYPE_UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2,
    TFGL_TYPE_UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3,
    TFGL_TYPE_UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4,
    TFGL_TYPE_UNSIGNED_VEC2UI   = GL_UNSIGNED_INT_VEC2,
    TFGL_TYPE_UNSIGNED_VEC3UI   = GL_UNSIGNED_INT_VEC3,
    TFGL_TYPE_UNSIGNED_VEC4UI   = GL_UNSIGNED_INT_VEC4,

    TFGL_TYPE_BOOL_VEC2 = GL_BOOL_VEC2,
    TFGL_TYPE_BOOL_VEC3 = GL_BOOL_VEC3,
    TFGL_TYPE_BOOL_VEC4 = GL_BOOL_VEC4,

    TFGL_TYPE_FLOAT_MAT2   = GL_FLOAT_MAT2,
    TFGL_TYPE_FLOAT_MAT3   = GL_FLOAT_MAT3,
    TFGL_TYPE_FLOAT_MAT4   = GL_FLOAT_MAT4,
    TFGL_TYPE_FLOAT_MAT2x3 = GL_FLOAT_MAT2x3,
    TFGL_TYPE_FLOAT_MAT2x4 = GL_FLOAT_MAT2x4,
    TFGL_TYPE_FLOAT_MAT3x2 = GL_FLOAT_MAT3x2,
    TFGL_TYPE_FLOAT_MAT3x4 = GL_FLOAT_MAT3x4,
    TFGL_TYPE_FLOAT_MAT4x2 = GL_FLOAT_MAT4x2,
    TFGL_TYPE_FLOAT_MAT4x3 = GL_FLOAT_MAT4x3,
    TFGL_TYPE_MAT2F        = GL_FLOAT_MAT2,
    TFGL_TYPE_MAT3F        = GL_FLOAT_MAT3,
    TFGL_TYPE_MAT4F        = GL_FLOAT_MAT4,
    TFGL_TYPE_MAT2x3F      = GL_FLOAT_MAT2x3,
    TFGL_TYPE_MAT2x4F      = GL_FLOAT_MAT2x4,
    TFGL_TYPE_MAT3x2F      = GL_FLOAT_MAT3x2,
    TFGL_TYPE_MAT3x4F      = GL_FLOAT_MAT3x4,
    TFGL_TYPE_MAT4x2F      = GL_FLOAT_MAT4x2,
    TFGL_TYPE_MAT4x3F      = GL_FLOAT_MAT4x3,

    TFGL_TYPE_SAMPLER_2D        = GL_SAMPLER_2D,
    TFGL_TYPE_SAMPLER_3D        = GL_SAMPLER_3D,
    TFGL_TYPE_SAMPLER_CUBE      = GL_SAMPLER_CUBE,
    TFGL_TYPE_SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,
    TFGL_TYPE_SAMPLER_2D_ARRAY  = GL_SAMPLER_2D_ARRAY,
    TFGL_TYPE_SAMPLER_2D_ARRAY_SHADOW       = GL_SAMPLER_2D_ARRAY_SHADOW,
    TFGL_TYPE_SAMPLER_CUBE_SHADOW           = GL_SAMPLER_CUBE_SHADOW,
    TFGL_TYPE_INT_SAMPLER_2D                = GL_INT_SAMPLER_2D,
    TFGL_TYPE_INT_SAMPLER_3D                = GL_INT_SAMPLER_3D,
    TFGL_TYPE_INT_SAMPLER_CUBE              = GL_INT_SAMPLER_CUBE,
    TFGL_TYPE_INT_SAMPLER_2D_ARRAY          = GL_INT_SAMPLER_2D_ARRAY,
    TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D       = GL_UNSIGNED_INT_SAMPLER_2D,
    TFGL_TYPE_UNSIGNED_INT_SAMPLER_3D       = GL_UNSIGNED_INT_SAMPLER_3D,
    TFGL_TYPE_UNSIGNED_INT_SAMPLER_CUBE     = GL_UNSIGNED_INT_SAMPLER_CUBE,
    TFGL_TYPE_UNSIGNED_INT_SAMPLER_2D_ARRAY = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
} GLType;

typedef enum
{
    TFGL_TEXTURE_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
    TFGL_TEXTURE_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    TFGL_TEXTURE_REPEAT          = GL_REPEAT,
} GLTextureWrap;

typedef  enum
{
    TFGL_TEXTURE_NEAREST = GL_NEAREST,
    TFGL_TEXTURE_LINEAR  = GL_LINEAR,
    TFGL_TEXTURE_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    TFGL_TEXTURE_NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    TFGL_TEXTURE_LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    TFGL_TEXTURE_LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
} GLTextureFilter;

typedef enum
{
    TFGL_FB_ATTACH_COLOR0        = GL_COLOR_ATTACHMENT0,
    TFGL_FB_ATTACH_COLOR1        = GL_COLOR_ATTACHMENT1,
    TFGL_FB_ATTACH_COLOR2        = GL_COLOR_ATTACHMENT2,
    TFGL_FB_ATTACH_COLOR3        = GL_COLOR_ATTACHMENT3,
    TFGL_FB_ATTACH_COLOR4        = GL_COLOR_ATTACHMENT4,
    TFGL_FB_ATTACH_COLOR5        = GL_COLOR_ATTACHMENT5,
    TFGL_FB_ATTACH_COLOR6        = GL_COLOR_ATTACHMENT6,
    TFGL_FB_ATTACH_COLOR7        = GL_COLOR_ATTACHMENT7,
    TFGL_FB_ATTACH_COLOR8        = GL_COLOR_ATTACHMENT8,
    TFGL_FB_ATTACH_COLOR9        = GL_COLOR_ATTACHMENT9,
    TFGL_FB_ATTACH_COLOR10       = GL_COLOR_ATTACHMENT10,
    TFGL_FB_ATTACH_COLOR11       = GL_COLOR_ATTACHMENT11,
    TFGL_FB_ATTACH_COLOR12       = GL_COLOR_ATTACHMENT12,
    TFGL_FB_ATTACH_COLOR13       = GL_COLOR_ATTACHMENT13,
    TFGL_FB_ATTACH_COLOR14       = GL_COLOR_ATTACHMENT14,
    TFGL_FB_ATTACH_COLOR15       = GL_COLOR_ATTACHMENT15,
    TFGL_FB_ATTACH_DEPTH         = GL_DEPTH_ATTACHMENT,
    TFGL_FB_ATTACH_STENCIL       = GL_STENCIL_ATTACHMENT,
    TFGL_FB_ATTACH_DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
} GLFramebufferAttachment;

} // namespace treeface

#endif // TREEFACE_GL_ENUMS_H
