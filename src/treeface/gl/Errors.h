#ifndef TREEFACE_GL_ERROR_H
#define TREEFACE_GL_ERROR_H

#include <stdexcept>
#include <treecore/String.h>

namespace treeface
{

class GLInvalidEnum: public std::runtime_error
{
public:
    GLInvalidEnum( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~GLInvalidEnum() = default;
};

class GLInvalidValue: public std::runtime_error
{
public:
    GLInvalidValue( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~GLInvalidValue() = default;
};

class GLInvalidOperation: public std::runtime_error
{
public:
    GLInvalidOperation( const treecore::String& message ): std::runtime_error( message.toRawUTF8() ) {}
    virtual ~GLInvalidOperation() = default;
};

class ProgramCompileError: public std::runtime_error
{
public:
    ProgramCompileError( const treecore::String& msg ): std::runtime_error( msg.toRawUTF8() ) {}
    ~ProgramCompileError() override {}
};

class ProgramLinkError: public std::runtime_error
{
public:
    ProgramLinkError( const treecore::String& msg ): std::runtime_error( msg.toRawUTF8() ) {}
    ~ProgramLinkError() override {}
};

} // namespace treeface

#endif // TREEFACE_GL_ERROR_H
