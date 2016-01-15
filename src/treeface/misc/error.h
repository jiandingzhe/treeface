#ifndef TREECORE_ERROR_H
#define TREECORE_ERROR_H

#include <stdexcept>
#include <treecore/String.h>

namespace treeface
{

class ConfigParseError: public std::runtime_error
{
public:
    ConfigParseError( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~ConfigParseError() = default;
};

class ImageLoadError: public std::runtime_error
{
public:
    ImageLoadError( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~ImageLoadError() = default;
};

class TextureImageFormatError: public std::runtime_error
{
public:
    TextureImageFormatError( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~TextureImageFormatError() = default;
};

} //namespace treecore

#endif // TREECORE_ERROR_H
