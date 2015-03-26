#ifndef TREEJUCE_NATIVE_WIN32_INTER_PROCESS_LOCK_H
#define TREEJUCE_NATIVE_WIN32_INTER_PROCESS_LOCK_H

#include "treejuce/InterProcessLock.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

class InterProcessLock::Pimpl
{
public:
    Pimpl (String name, const int timeOutMillisecs);
    ~Pimpl();
    void close();

    HANDLE handle;
    int refCount;
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_WIN32_INTER_PROCESS_LOCK_H
