#ifndef TREEJUCE_NATIVE_POSIX_INTER_PROCESS_LOCK_H
#define TREEJUCE_NATIVE_POSIX_INTER_PROCESS_LOCK_H

#include "treejuce/InterProcessLock.h"
#include "treejuce/BasicNativeHeaders.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

class InterProcessLock::Pimpl
{
public:
    Pimpl (const String& lockName, const int timeOutMillisecs)
        : handle (0), refCount (1)
    {
       #if JUCE_MAC
        if (! createLockFile (File ("~/Library/Caches/com.juce.locks").getChildFile (lockName), timeOutMillisecs))
            // Fallback if the user's home folder is on a network drive with no ability to lock..
            createLockFile (File ("/tmp/com.juce.locks").getChildFile (lockName), timeOutMillisecs);

       #else
        File tempFolder ("/var/tmp");
        if (! tempFolder.isDirectory())
            tempFolder = "/tmp";

        createLockFile (tempFolder.getChildFile (lockName), timeOutMillisecs);
       #endif
    }

    ~Pimpl()
    {
        closeFile();
    }

    bool createLockFile (const File& file, const int timeOutMillisecs)
    {
        file.create();
        handle = open (file.getFullPathName().toUTF8(), O_RDWR);

        if (handle != 0)
        {
            struct flock fl;
            zerostruct (fl);

            fl.l_whence = SEEK_SET;
            fl.l_type = F_WRLCK;

            const int64 endTime = Time::currentTimeMillis() + timeOutMillisecs;

            for (;;)
            {
                const int result = fcntl (handle, F_SETLK, &fl);

                if (result >= 0)
                    return true;

                const int error = errno;

                if (error != EINTR)
                {
                    if (error == EBADF || error == ENOTSUP)
                        return false;

                    if (timeOutMillisecs == 0
                         || (timeOutMillisecs > 0 && Time::currentTimeMillis() >= endTime))
                        break;

                    Thread::sleep (10);
                }
            }
        }

        closeFile();
        return true; // only false if there's a file system error. Failure to lock still returns true.
    }

    void closeFile()
    {
        if (handle != 0)
        {
            struct flock fl;
            zerostruct (fl);

            fl.l_whence = SEEK_SET;
            fl.l_type = F_UNLCK;

            while (! (fcntl (handle, F_SETLKW, &fl) >= 0 || errno != EINTR))
            {}

            close (handle);
            handle = 0;
        }
    }

    int handle, refCount;
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_POSIX_INTER_PROCESS_LOCK_H
