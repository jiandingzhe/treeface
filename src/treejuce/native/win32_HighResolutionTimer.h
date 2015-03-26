#ifndef TREEJUCE_NATIVE_WIN32_HIGH_RESOLUTION_TIMER_H
#define TREEJUCE_NATIVE_WIN32_HIGH_RESOLUTION_TIMER_H

#include "treejuce/HighResolutionTimer.h"
#include "treejuce/StandardHeader.h"
#include "treejuce/BasicNativeHeaders.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

struct HighResolutionTimer::Pimpl
{
    Pimpl (HighResolutionTimer& t) noexcept;

    ~Pimpl();

    void start (int newPeriod);

    void stop();

    HighResolutionTimer& owner;
    int periodMs;

private:
    unsigned int timerID;

    static void __stdcall callbackFunction (UINT, UINT, DWORD_PTR userInfo, DWORD_PTR, DWORD_PTR);

    JUCE_DECLARE_NON_COPYABLE (Pimpl)
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_WIN32_HIGH_RESOLUTION_TIMER_H
