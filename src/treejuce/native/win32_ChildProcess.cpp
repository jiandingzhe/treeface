#include "treejuce/native/win32_ChildProcess.h"

#include "treejuce/String.h"
#include "treejuce/StringArray.h"
#include "treejuce/StringRef.h"
#include "treejuce/Thread.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

bool ChildProcess::start (const String& command, int streamFlags)
{
    activeProcess = new ActiveProcess (command, streamFlags);

    if (! activeProcess->ok)
        activeProcess = nullptr;

    return activeProcess != nullptr;
}

bool ChildProcess::start (const StringArray& args, int streamFlags)
{
    String escaped;

    for (int i = 0; i < args.size(); ++i)
    {
        String arg (args[i]);

        // If there are spaces, surround it with quotes. If there are quotes,
        // replace them with \" so that CommandLineToArgv will correctly parse them.
        if (arg.containsAnyOf ("\" "))
            arg = arg.replace ("\"", "\\\"").quoted();

        escaped << arg << ' ';
    }

    return start (escaped.trim(), streamFlags);
}

ChildProcess::ActiveProcess::ActiveProcess (const String& command, int streamFlags)
	: ok (false), readPipe (0), writePipe (0)
{
	SECURITY_ATTRIBUTES securityAtts = { 0 };
	securityAtts.nLength = sizeof (securityAtts);
	securityAtts.bInheritHandle = TRUE;

	if (CreatePipe (&readPipe, &writePipe, &securityAtts, 0)
		 && SetHandleInformation (readPipe, HANDLE_FLAG_INHERIT, 0))
	{
		STARTUPINFOW startupInfo = { 0 };
		startupInfo.cb = sizeof (startupInfo);

		startupInfo.hStdOutput = (streamFlags | wantStdOut) != 0 ? writePipe : 0;
		startupInfo.hStdError  = (streamFlags | wantStdErr) != 0 ? writePipe : 0;
		startupInfo.dwFlags = STARTF_USESTDHANDLES;

		ok = CreateProcess (nullptr, const_cast <LPWSTR> (command.toWideCharPointer()),
							nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
							nullptr, nullptr, &startupInfo, &processInfo) != FALSE;
	}
}

ChildProcess::ActiveProcess::~ActiveProcess()
{
	if (ok)
	{
		CloseHandle (processInfo.hThread);
		CloseHandle (processInfo.hProcess);
	}

	if (readPipe != 0)
		CloseHandle (readPipe);

	if (writePipe != 0)
		CloseHandle (writePipe);
}

int ChildProcess::ActiveProcess::read (void* dest, int numNeeded) const noexcept
{
	int total = 0;

	while (ok && numNeeded > 0)
	{
		DWORD available = 0;

		if (! PeekNamedPipe ((HANDLE) readPipe, nullptr, 0, nullptr, &available, nullptr))
			break;

		const int numToDo = jmin ((int) available, numNeeded);

		if (available == 0)
		{
			if (! isRunning())
				break;

			Thread::yield();
		}
		else
		{
			DWORD numRead = 0;
			if (! ReadFile ((HANDLE) readPipe, dest, numToDo, &numRead, nullptr))
				break;

			total += numRead;
			dest = addBytesToPointer (dest, numRead);
			numNeeded -= numRead;
		}
	}

	return total;
}

uint32 ChildProcess::ActiveProcess::getExitCode() const noexcept
{
	DWORD exitCode = 0;
	GetExitCodeProcess (processInfo.hProcess, &exitCode);
	return (uint32) exitCode;
}

TREEFACE_JUCE_NAMESPACE_END
