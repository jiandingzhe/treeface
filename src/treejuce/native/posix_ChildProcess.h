#ifndef TREEJUCE_NATIVE_POSIX_CHILD_PROCESS_H
#define TREEJUCE_NATIVE_POSIX_CHILD_PROCESS_H

#include "treejuce/ChildProcess.h"
#include "treejuce/BasicNativeHeaders.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

class ChildProcess::ActiveProcess
{
public:
    ActiveProcess (const StringArray& arguments, int streamFlags)
        : childPID (0), pipeHandle (0), readHandle (0)
    {
        // Looks like you're trying to launch a non-existent exe or a folder (perhaps on OSX
        // you're trying to launch the .app folder rather than the actual binary inside it?)
        jassert ((! arguments[0].containsChar ('/'))
                  || File::getCurrentWorkingDirectory().getChildFile (arguments[0]).existsAsFile());

        int pipeHandles[2] = { 0 };

        if (pipe (pipeHandles) == 0)
        {
            const pid_t result = fork();

            if (result < 0)
            {
                close (pipeHandles[0]);
                close (pipeHandles[1]);
            }
            else if (result == 0)
            {
                // we're the child process..
                close (pipeHandles[0]);   // close the read handle

                if ((streamFlags & wantStdOut) != 0)
                    dup2 (pipeHandles[1], 1); // turns the pipe into stdout
                else
                    close (STDOUT_FILENO);

                if ((streamFlags & wantStdErr) != 0)
                    dup2 (pipeHandles[1], 2);
                else
                    close (STDERR_FILENO);

                close (pipeHandles[1]);

                Array<char*> argv;
                for (int i = 0; i < arguments.size(); ++i)
                    if (arguments[i].isNotEmpty())
                        argv.add (const_cast<char*> (arguments[i].toUTF8().getAddress()));

                argv.add (nullptr);

                execvp (argv[0], argv.getRawDataPointer());
                exit (-1);
            }
            else
            {
                // we're the parent process..
                childPID = result;
                pipeHandle = pipeHandles[0];
                close (pipeHandles[1]); // close the write handle
            }
        }
    }

    ~ActiveProcess()
    {
        if (readHandle != 0)
            fclose (readHandle);

        if (pipeHandle != 0)
            close (pipeHandle);
    }

    bool isRunning() const NOEXCEPT
    {
        if (childPID != 0)
        {
            int childState;
            const int pid = waitpid (childPID, &childState, WNOHANG);
            return pid == 0 || ! (WIFEXITED (childState) || WIFSIGNALED (childState));
        }

        return false;
    }

    int read (void* const dest, const int numBytes) NOEXCEPT
    {
        jassert (dest != nullptr);

        #ifdef fdopen
         #error // the zlib headers define this function as NULL!
        #endif

        if (readHandle == 0 && childPID != 0)
            readHandle = fdopen (pipeHandle, "r");

        if (readHandle != 0)
            return (int) fread (dest, 1, (size_t) numBytes, readHandle);

        return 0;
    }

    bool killProcess() const NOEXCEPT
    {
        return ::kill (childPID, SIGKILL) == 0;
    }

    uint32 getExitCode() const NOEXCEPT
    {
        if (childPID != 0)
        {
            int childState = 0;
            const int pid = waitpid (childPID, &childState, WNOHANG);

            if (pid >= 0 && WIFEXITED (childState))
                return WEXITSTATUS (childState);
        }

        return 0;
    }

    int childPID;

private:
    int pipeHandle;
    FILE* readHandle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveProcess)
};

TREEFACE_JUCE_NAMESPACE_END

#endif // TREEJUCE_NATIVE_POSIX_CHILD_PROCESS_H
