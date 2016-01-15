#ifndef TREEFACE_STRING_CAST_H
#define TREEFACE_STRING_CAST_H

#include "treeface/common.h"
#include "treeface/enums.h"
#include "treeface/gl/enums.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <treecore/String.h>
#include <treecore/StringCast.h>
#include <FreeImage.h>

namespace treecore {

template<>
bool fromString<treeface::MaterialType>( const treecore::String& string, treeface::MaterialType& result );

template<>
bool fromString<treeface::LineCap>( const String& str, treeface::LineCap& result );

template<>
bool fromString<treeface::LineJoin>( const String& str, treeface::LineJoin& result );

template<>
bool fromString<FREE_IMAGE_FORMAT>( const treecore::String& string, FREE_IMAGE_FORMAT& result );

template<>
bool fromString<FREE_IMAGE_TYPE>( const treecore::String& string, FREE_IMAGE_TYPE& result );

template<>
bool fromString<FREE_IMAGE_COLOR_TYPE>( const treecore::String& string, FREE_IMAGE_COLOR_TYPE& result );

template<>
bool fromString<treeface::GLBufferType>( const treecore::String& string, treeface::GLBufferType& result );

template<>
bool fromString<treeface::GLBufferUsage>( const treecore::String& string, treeface::GLBufferUsage& result );

template<>
bool fromString<treeface::GLImageFormat>( const treecore::String& string, treeface::GLImageFormat& result );

template<>
bool fromString<treeface::GLInternalImageFormat>( const treecore::String& string, treeface::GLInternalImageFormat& result );

template<>
bool fromString<treeface::GLPrimitive>( const treecore::String& string, treeface::GLPrimitive& result );

template<>
bool fromString<treeface::GLTextureType>( const treecore::String& string, treeface::GLTextureType& result );

template<>
bool fromString<treeface::GLTextureFilter>( const treecore::String& string, treeface::GLTextureFilter& result );

template<>
bool fromString<treeface::GLTextureWrap>( const treecore::String& string, treeface::GLTextureWrap& result );

template<>
bool fromString<treeface::GLType>( const treecore::String& string, treeface::GLType& result );

template<>
bool fromString<treeface::TextureImageSoloChannelPolicy>(const treecore::String& string, treeface::TextureImageSoloChannelPolicy& result);

template<>
bool fromString<treeface::TextureImageDualChannelPolicy>(const treecore::String& string, treeface::TextureImageDualChannelPolicy& result);

template<>
bool fromString<treeface::TextureImageIntDataPolicy>(const treecore::String& string, treeface::TextureImageIntDataPolicy& result);

template<>
treecore::String toString<treeface::MaterialType>( treeface::MaterialType value );

template<>
treecore::String toString<treeface::LineCap>( treeface::LineCap cap );

template<>
treecore::String toString<treeface::LineJoin>( treeface::LineJoin join );

template<>
treecore::String toString<FREE_IMAGE_FORMAT>( FREE_IMAGE_FORMAT arg );

template<>
treecore::String toString<FREE_IMAGE_TYPE>( FREE_IMAGE_TYPE arg );

template<>
treecore::String toString<FREE_IMAGE_COLOR_TYPE>( FREE_IMAGE_COLOR_TYPE arg );

template<>
treecore::String toString<treeface::GLBufferType>( treeface::GLBufferType arg );

template<>
treecore::String toString<treeface::GLBufferUsage>( treeface::GLBufferUsage arg );

template<>
treecore::String toString<treeface::GLImageFormat>( treeface::GLImageFormat arg );

template<>
treecore::String toString<treeface::GLInternalImageFormat>( treeface::GLInternalImageFormat arg );

template<>
treecore::String toString<treeface::GLImageDataType>( treeface::GLImageDataType arg );

template<>
treecore::String toString<treeface::GLPrimitive>( treeface::GLPrimitive arg );

template<>
treecore::String toString<treeface::GLTextureFilter>( treeface::GLTextureFilter arg );

template<>
treecore::String toString<treeface::GLTextureType>( treeface::GLTextureType arg );

template<>
treecore::String toString<treeface::GLTextureWrap>( treeface::GLTextureWrap arg );

template<>
treecore::String toString<treeface::GLType>( treeface::GLType arg );

template<>
treecore::String toString<treeface::TextureImageSoloChannelPolicy>(treeface::TextureImageSoloChannelPolicy arg);

template<>
treecore::String toString<treeface::TextureImageDualChannelPolicy>(treeface::TextureImageDualChannelPolicy arg);

template<>
treecore::String toString<treeface::TextureImageIntDataPolicy>(treeface::TextureImageIntDataPolicy arg);

} // namespace treecore

#endif // TREEFACE_STRING_CAST_H
