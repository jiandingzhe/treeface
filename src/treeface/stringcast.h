#ifndef TREEFACE_STRING_CAST_H
#define TREEFACE_STRING_CAST_H

#include "treeface/common.h"

#include <treejuce/String.h>
#include <FreeImage.h>

TREEFACE_NAMESPACE_BEGIN

template<typename T>
T from_string(const treejuce::String& string);

template<typename T>
treejuce::String to_string(T arg);

template<>
FREE_IMAGE_FORMAT from_string<FREE_IMAGE_FORMAT>(const treejuce::String& string);

template<>
treejuce::String to_string<FREE_IMAGE_FORMAT>(FREE_IMAGE_FORMAT arg);

template<>
FREE_IMAGE_TYPE from_string<FREE_IMAGE_TYPE>(const treejuce::String& string);

template<>
treejuce::String to_string<FREE_IMAGE_TYPE>(FREE_IMAGE_TYPE arg);

template<>
FREE_IMAGE_COLOR_TYPE from_string<FREE_IMAGE_COLOR_TYPE>(const treejuce::String& string);

template<>
treejuce::String to_string<FREE_IMAGE_COLOR_TYPE>(FREE_IMAGE_COLOR_TYPE arg);


TREEFACE_NAMESPACE_END

#endif // TREEFACE_STRING_CAST_H
