/*
  ==============================================================================

   This file is part of the juce_core module of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission to use, copy, modify, and/or distribute this software for any purpose with
   or without fee is hereby granted, provided that the above copyright notice and this
   permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
   TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
   NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
   DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
   IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ------------------------------------------------------------------------------

   NOTE! This permissive ISC license applies ONLY to files within the juce_core module!
   All other JUCE modules are covered by a dual GPL/commercial license, so if you are
   using any other modules, be sure to check that you also comply with their license.

   For more details, visit www.juce.com

  ==============================================================================
*/

#include "treejuce/BasicNativeHeaders.h"
#include "treejuce/CriticalSection.h"
#include "treejuce/DynamicLibrary.h"
#include "treejuce/HighResolutionTimer.h"
#include "treejuce/Process.h"
#include "treejuce/StringArray.h"
#include "treejuce/StringRef.h"
#include "treejuce/Thread.h"
#include "treejuce/WaitableEvent.h"

#include "treejuce/native/win32_ChildProcess.h"
#include "treejuce/native/win32_HighResolutionTimer.h"
#include "treejuce/native/win32_InterProcessLock.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

HWND juce_messageWindowHandle = 0;  // (this is used by other parts of the codebase)

void* getUser32Function (const char* functionName)
{
    HMODULE module = GetModuleHandleA ("user32.dll");
    jassert (module != 0);
    return (void*) GetProcAddress (module, functionName);
}

//==============================================================================
#if ! JUCE_USE_INTRINSICS
// In newer compilers, the inline versions of these are used (in juce_Atomic.h), but in
// older ones we have to actually call the ops as win32 functions..
long juce_InterlockedExchange (volatile long* a, long b) NOEXCEPT                { return InterlockedExchange (a, b); }
long juce_InterlockedIncrement (volatile long* a) NOEXCEPT                       { return InterlockedIncrement (a); }
long juce_InterlockedDecrement (volatile long* a) NOEXCEPT                       { return InterlockedDecrement (a); }
long juce_InterlockedExchangeAdd (volatile long* a, long b) NOEXCEPT             { return InterlockedExchangeAdd (a, b); }
long juce_InterlockedCompareExchange (volatile long* a, long b, long c) NOEXCEPT { return InterlockedCompareExchange (a, b, c); }

__int64 juce_InterlockedCompareExchange64 (volatile __int64* value, __int64 newValue, __int64 valueToCompare) NOEXCEPT
{
    jassertfalse; // This operation isn't available in old MS compiler versions!

    __int64 oldValue = *value;
    if (oldValue == valueToCompare)
        *value = newValue;

    return oldValue;
}

#endif

//==============================================================================
CriticalSection::CriticalSection() NOEXCEPT
{
    // (just to check the MS haven't changed this structure and broken things...)
   #if JUCE_VC7_OR_EARLIER
    static_jassert (sizeof (CRITICAL_SECTION) <= 24);
   #else
    static_jassert (sizeof (CRITICAL_SECTION) <= sizeof (lock));
   #endif

    InitializeCriticalSection ((CRITICAL_SECTION*) lock);
}

CriticalSection::~CriticalSection() NOEXCEPT        { DeleteCriticalSection ((CRITICAL_SECTION*) lock); }
void CriticalSection::enter() const NOEXCEPT        { EnterCriticalSection ((CRITICAL_SECTION*) lock); }
bool CriticalSection::tryEnter() const NOEXCEPT     { return TryEnterCriticalSection ((CRITICAL_SECTION*) lock) != FALSE; }
void CriticalSection::exit() const NOEXCEPT         { LeaveCriticalSection ((CRITICAL_SECTION*) lock); }


//==============================================================================
WaitableEvent::WaitableEvent (const bool manualReset) NOEXCEPT
    : handle (CreateEvent (0, manualReset ? TRUE : FALSE, FALSE, 0)) {}

WaitableEvent::~WaitableEvent() NOEXCEPT        { CloseHandle (handle); }

void WaitableEvent::signal() const NOEXCEPT     { SetEvent (handle); }
void WaitableEvent::reset() const NOEXCEPT      { ResetEvent (handle); }

bool WaitableEvent::wait (const int timeOutMs) const NOEXCEPT
{
    return WaitForSingleObject (handle, (DWORD) timeOutMs) == WAIT_OBJECT_0;
}

//==============================================================================
void JUCE_API juce_threadEntryPoint (void*);

static unsigned int __stdcall threadEntryProc (void* userData)
{
    if (juce_messageWindowHandle != 0)
        AttachThreadInput (GetWindowThreadProcessId (juce_messageWindowHandle, 0),
                           GetCurrentThreadId(), TRUE);

    juce_threadEntryPoint (userData);

    _endthreadex (0);
    return 0;
}

void Thread::launchThread()
{
    unsigned int newThreadId;
    threadHandle = (void*) _beginthreadex (0, 0, &threadEntryProc, this, 0, &newThreadId);
    threadId = (ThreadID) newThreadId;
}

void Thread::closeThreadHandle()
{
    CloseHandle ((HANDLE) threadHandle);
    threadId = 0;
    threadHandle = 0;
}

void Thread::killThread()
{
    if (threadHandle != 0)
    {
       #if JUCE_DEBUG
        OutputDebugStringA ("** Warning - Forced thread termination **\n");
       #endif
        TerminateThread (threadHandle, 0);
    }
}

void JUCE_CALLTYPE Thread::setCurrentThreadName (const String& name)
{
   #if JUCE_DEBUG && JUCE_MSVC
    struct
    {
        DWORD dwType;
        LPCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags;
    } info;

    info.dwType = 0x1000;
    info.szName = name.toUTF8();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;

    __try
    {
        RaiseException (0x406d1388 /*MS_VC_EXCEPTION*/, 0, sizeof (info) / sizeof (ULONG_PTR), (ULONG_PTR*) &info);
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {}
   #else
    (void) name;
   #endif
}

Thread::ThreadID JUCE_CALLTYPE Thread::getCurrentThreadId()
{
    return (ThreadID) (pointer_sized_int) GetCurrentThreadId();
}

bool Thread::setThreadPriority (void* handle, int priority)
{
    int pri = THREAD_PRIORITY_TIME_CRITICAL;

    if (priority < 1)       pri = THREAD_PRIORITY_IDLE;
    else if (priority < 2)  pri = THREAD_PRIORITY_LOWEST;
    else if (priority < 5)  pri = THREAD_PRIORITY_BELOW_NORMAL;
    else if (priority < 7)  pri = THREAD_PRIORITY_NORMAL;
    else if (priority < 9)  pri = THREAD_PRIORITY_ABOVE_NORMAL;
    else if (priority < 10) pri = THREAD_PRIORITY_HIGHEST;

    if (handle == 0)
        handle = GetCurrentThread();

    return SetThreadPriority (handle, pri) != FALSE;
}

void JUCE_CALLTYPE Thread::setCurrentThreadAffinityMask (const uint32 affinityMask)
{
    SetThreadAffinityMask (GetCurrentThread(), affinityMask);
}

//==============================================================================
struct SleepEvent
{
    SleepEvent() NOEXCEPT
        : handle (CreateEvent (nullptr, FALSE, FALSE,
                              #if JUCE_DEBUG
                               _T("JUCE Sleep Event")))
                              #else
                               nullptr))
                              #endif
    {}

    ~SleepEvent() NOEXCEPT
    {
        CloseHandle (handle);
        handle = 0;
    }

    HANDLE handle;
};

static SleepEvent sleepEvent;

void JUCE_CALLTYPE Thread::sleep (const int millisecs)
{
    jassert (millisecs >= 0);

    if (millisecs >= 10 || sleepEvent.handle == 0)
        Sleep ((DWORD) millisecs);
    else
        // unlike Sleep() this is guaranteed to return to the current thread after
        // the time expires, so we'll use this for short waits, which are more likely
        // to need to be accurate
        WaitForSingleObject (sleepEvent.handle, (DWORD) millisecs);
}

void Thread::yield()
{
    Sleep (0);
}

//==============================================================================
static int lastProcessPriority = -1;

// called when the app gains focus because Windows does weird things to process priority
// when you swap apps, and this forces an update when the app is brought to the front.
void juce_repeatLastProcessPriority()
{
    if (lastProcessPriority >= 0) // (avoid changing this if it's not been explicitly set by the app..)
    {
        DWORD p;

        switch (lastProcessPriority)
        {
            case Process::LowPriority:          p = IDLE_PRIORITY_CLASS; break;
            case Process::NormalPriority:       p = NORMAL_PRIORITY_CLASS; break;
            case Process::HighPriority:         p = HIGH_PRIORITY_CLASS; break;
            case Process::RealtimePriority:     p = REALTIME_PRIORITY_CLASS; break;
            default:                            jassertfalse; return; // bad priority value
        }

        SetPriorityClass (GetCurrentProcess(), p);
    }
}

void JUCE_CALLTYPE Process::setPriority (ProcessPriority prior)
{
    if (lastProcessPriority != (int) prior)
    {
        lastProcessPriority = (int) prior;
        juce_repeatLastProcessPriority();
    }
}

JUCE_API bool JUCE_CALLTYPE juce_isRunningUnderDebugger()
{
    return IsDebuggerPresent() != FALSE;
}

bool JUCE_CALLTYPE Process::isRunningUnderDebugger()
{
    return juce_isRunningUnderDebugger();
}

static void* currentModuleHandle = nullptr;

void* JUCE_CALLTYPE Process::getCurrentModuleInstanceHandle() NOEXCEPT
{
    if (currentModuleHandle == nullptr)
        currentModuleHandle = GetModuleHandleA (nullptr);

    return currentModuleHandle;
}

void JUCE_CALLTYPE Process::setCurrentModuleInstanceHandle (void* const newHandle) NOEXCEPT
{
    currentModuleHandle = newHandle;
}

void JUCE_CALLTYPE Process::raisePrivilege()
{
    jassertfalse; // xxx not implemented
}

void JUCE_CALLTYPE Process::lowerPrivilege()
{
    jassertfalse; // xxx not implemented
}

void JUCE_CALLTYPE Process::terminate()
{
   #if JUCE_MSVC && JUCE_CHECK_MEMORY_LEAKS
    _CrtDumpMemoryLeaks();
   #endif

    // bullet in the head in case there's a problem shutting down..
    ExitProcess (1);
}

bool juce_isRunningInWine()
{
    HMODULE ntdll = GetModuleHandleA ("ntdll");
    return ntdll != 0 && GetProcAddress (ntdll, "wine_get_version") != nullptr;
}

//==============================================================================
bool DynamicLibrary::open (const String& name)
{
    close();

    JUCE_TRY
    {
        handle = LoadLibrary (name.toWideCharPointer());
    }
    JUCE_CATCH_ALL

    return handle != nullptr;
}

void DynamicLibrary::close()
{
    JUCE_TRY
    {
        if (handle != nullptr)
        {
            FreeLibrary ((HMODULE) handle);
            handle = nullptr;
        }
    }
    JUCE_CATCH_ALL
}

void* DynamicLibrary::getFunction (const String& functionName) NOEXCEPT
{
    return handle != nullptr ? (void*) GetProcAddress ((HMODULE) handle, functionName.toUTF8()) // (void* cast is required for mingw)
                             : nullptr;
}


//==============================================================================

InterProcessLock::InterProcessLock (const String& name_)
    : name (name_)
{
}

InterProcessLock::~InterProcessLock()
{
}

bool InterProcessLock::enter (const int timeOutMillisecs)
{
    const ScopedLock sl (lock);

    if (pimpl == nullptr)
    {
        pimpl = new Pimpl (name, timeOutMillisecs);

        if (pimpl->handle == 0)
            pimpl = nullptr;
    }
    else
    {
        pimpl->refCount++;
    }

    return pimpl != nullptr;
}

void InterProcessLock::exit()
{
    const ScopedLock sl (lock);

    // Trying to release the lock too many times!
    jassert (pimpl != nullptr);

    if (pimpl != nullptr && --(pimpl->refCount) == 0)
        pimpl = nullptr;
}

InterProcessLock::Pimpl::Pimpl (String name, const int timeOutMillisecs)
    : handle (0), refCount (1)
{
    name = name.replaceCharacter ('\\', '/');
    handle = CreateMutexW (0, TRUE, ("Global\\" + name).toWideCharPointer());

    // Not 100% sure why a global mutex sometimes can't be allocated, but if it fails, fall back to
    // a local one. (A local one also sometimes fails on other machines so neither type appears to be
    // universally reliable)
    if (handle == 0)
        handle = CreateMutexW (0, TRUE, ("Local\\" + name).toWideCharPointer());

    if (handle != 0 && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        if (timeOutMillisecs == 0)
        {
            close();
            return;
        }

        switch (WaitForSingleObject (handle, timeOutMillisecs < 0 ? INFINITE : timeOutMillisecs))
        {
            case WAIT_OBJECT_0:
            case WAIT_ABANDONED:
                break;

            case WAIT_TIMEOUT:
            default:
                close();
                break;
        }
    }
}

InterProcessLock::Pimpl::~Pimpl()
{
    close();
}

void InterProcessLock::Pimpl::close()
{
    if (handle != 0)
    {
        ReleaseMutex (handle);
        CloseHandle (handle);
        handle = 0;
    }
}

//==============================================================================
HighResolutionTimer::Pimpl::Pimpl (HighResolutionTimer& t) NOEXCEPT  : owner (t), periodMs (0)
{
}

HighResolutionTimer::Pimpl::~Pimpl()
{
    jassert (periodMs == 0);
}

void HighResolutionTimer::Pimpl::start (int newPeriod)
{
    if (newPeriod != periodMs)
    {
        stop();
        periodMs = newPeriod;

        TIMECAPS tc;
        if (timeGetDevCaps (&tc, sizeof (tc)) == TIMERR_NOERROR)
        {
            const int actualPeriod = jlimit ((int) tc.wPeriodMin, (int) tc.wPeriodMax, newPeriod);

            timerID = timeSetEvent (actualPeriod, tc.wPeriodMin, callbackFunction, (DWORD_PTR) this,
                                    TIME_PERIODIC | TIME_CALLBACK_FUNCTION | 0x100 /*TIME_KILL_SYNCHRONOUS*/);
        }
    }
}

void HighResolutionTimer::Pimpl::stop()
{
    periodMs = 0;
    timeKillEvent (timerID);
}

void __stdcall HighResolutionTimer::Pimpl::callbackFunction (UINT, UINT, DWORD_PTR userInfo, DWORD_PTR, DWORD_PTR)
{
    if (Pimpl* const timer = reinterpret_cast<Pimpl*> (userInfo))
        if (timer->periodMs != 0)
            timer->owner.hiResTimerCallback();
}

TREEFACE_JUCE_NAMESPACE_END
