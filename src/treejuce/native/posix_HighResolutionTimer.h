#ifndef TREEJUCE_NATIVE_POSIX_HIGH_RESOLUTION_TIMER_H
#define TREEJUCE_NATIVE_POSIX_HIGH_RESOLUTION_TIMER_H

#include "treejuce/BasicNativeHeaders.h"
#include "treejuce/HighResolutionTimer.h"
#include "treejuce/MathsFunctions.h"
#include "treejuce/Thread.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

struct HighResolutionTimer::Pimpl
{
    Pimpl (HighResolutionTimer& t)  : owner (t), thread (0), shouldStop (false)
    {
    }

    ~Pimpl()
    {
        jassert (thread == 0);
    }

    void start (int newPeriod)
    {
        if (periodMs != newPeriod)
        {
            if (thread != pthread_self())
            {
                stop();

                periodMs = newPeriod;
                shouldStop = false;

                if (pthread_create (&thread, nullptr, timerThread, this) == 0)
                    setThreadToRealtime (thread, (uint64) newPeriod);
                else
                    jassertfalse;
            }
            else
            {
                periodMs = newPeriod;
                shouldStop = false;
            }
        }
    }

    void stop()
    {
        if (thread != 0)
        {
            shouldStop = true;

            while (thread != 0 && thread != pthread_self())
                Thread::yield();
        }
    }

    HighResolutionTimer& owner;
    int volatile periodMs;

private:
    pthread_t thread;
    bool volatile shouldStop;

    static void* timerThread (void* param)
    {
       #if JUCE_ANDROID
        const AndroidThreadScope androidEnv;
       #else
        int dummy;
        pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &dummy);
       #endif

        reinterpret_cast<Pimpl*> (param)->timerThread();
        return nullptr;
    }

    void timerThread()
    {
        int lastPeriod = periodMs;
        Clock clock (lastPeriod);

        while (! shouldStop)
        {
            clock.wait();
            owner.hiResTimerCallback();

            if (lastPeriod != periodMs)
            {
                lastPeriod = periodMs;
                clock = Clock (lastPeriod);
            }
        }

        periodMs = 0;
        thread = 0;
    }

    struct Clock
    {
       #if JUCE_MAC || JUCE_IOS
        Clock (double millis) NOEXCEPT
        {
            mach_timebase_info_data_t timebase;
            (void) mach_timebase_info (&timebase);
            delta = (((uint64_t) (millis * 1000000.0)) * timebase.denom) / timebase.numer;
            time = mach_absolute_time();
        }

        void wait() NOEXCEPT
        {
            time += delta;
            mach_wait_until (time);
        }

        uint64_t time, delta;

       #elif JUCE_ANDROID
        Clock (double millis) NOEXCEPT  : delta ((uint64) (millis * 1000000))
        {
        }

        void wait() NOEXCEPT
        {
            struct timespec t;
            t.tv_sec  = (time_t) (delta / 1000000000);
            t.tv_nsec = (long)   (delta % 1000000000);
            nanosleep (&t, nullptr);
        }

        uint64 delta;
       #else
        Clock (double millis) NOEXCEPT  : delta ((uint64) (millis * 1000000))
        {
            struct timespec t;
            clock_gettime (CLOCK_MONOTONIC, &t);
            time = 1000000000 * (int64) t.tv_sec + t.tv_nsec;
        }

        void wait() NOEXCEPT
        {
            time += delta;

            struct timespec t;
            t.tv_sec  = (time_t) (time / 1000000000);
            t.tv_nsec = (long)   (time % 1000000000);

            clock_nanosleep (CLOCK_MONOTONIC, TIMER_ABSTIME, &t, nullptr);
        }

        uint64 time, delta;
       #endif
    };

    static bool setThreadToRealtime (pthread_t thread, uint64 periodMs)
    {
       #if JUCE_MAC || JUCE_IOS
        thread_time_constraint_policy_data_t policy;
        policy.period      = (uint32_t) (periodMs * 1000000);
        policy.computation = 50000;
        policy.constraint  = policy.period;
        policy.preemptible = true;

        return thread_policy_set (pthread_mach_thread_np (thread),
                                  THREAD_TIME_CONSTRAINT_POLICY,
                                  (thread_policy_t) &policy,
                                  THREAD_TIME_CONSTRAINT_POLICY_COUNT) == KERN_SUCCESS;

       #else
        (void) periodMs;
        struct sched_param param;
        param.sched_priority = sched_get_priority_max (SCHED_RR);
        return pthread_setschedparam (thread, SCHED_RR, &param) == 0;

       #endif
    }

    JUCE_DECLARE_NON_COPYABLE (Pimpl)
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_POSIX_HIGH_RESOLUTION_TIMER_H
