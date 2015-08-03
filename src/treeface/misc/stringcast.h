#ifndef TREEFACE_STRING_CAST_H
#define TREEFACE_STRING_CAST_H

#include "treeface/common.h"
#include "treeface/enums.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <treecore/String.h>
#include <FreeImage.h>

namespace treeface {

template<typename T>
T from_string(const treecore::String& string);

template<typename T>
treecore::String to_string(T arg);

template<>
MaterialType from_string<MaterialType>(const treecore::String& string);

template<>
treecore::String to_string<MaterialType>(MaterialType value);

template<>
FREE_IMAGE_FORMAT from_string<FREE_IMAGE_FORMAT>(const treecore::String& string);

template<>
treecore::String to_string<FREE_IMAGE_FORMAT>(FREE_IMAGE_FORMAT arg);

template<>
FREE_IMAGE_TYPE from_string<FREE_IMAGE_TYPE>(const treecore::String& string);

template<>
treecore::String to_string<FREE_IMAGE_TYPE>(FREE_IMAGE_TYPE arg);

template<>
FREE_IMAGE_COLOR_TYPE from_string<FREE_IMAGE_COLOR_TYPE>(const treecore::String& string);

template<>
treecore::String to_string<FREE_IMAGE_COLOR_TYPE>(FREE_IMAGE_COLOR_TYPE arg);

GLenum gl_internal_format_from_string(const treecore::String& string);
treecore::String gl_internal_format_to_string(GLenum arg);

GLenum gl_type_from_string(const treecore::String& str);
treecore::String gl_type_to_string(GLenum arg);

GLenum gl_primitive_from_string(const treecore::String& str);
treecore::String gl_primitive_to_string(GLenum arg);

GLenum gl_tex_wrap_from_string(const treecore::String& str);
treecore::String gl_tex_wrap_to_string(GLenum arg);

} // namespace treeface

#endif // TREEFACE_STRING_CAST_H
