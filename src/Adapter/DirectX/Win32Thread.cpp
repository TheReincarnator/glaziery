/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Win32Thread::Win32Thread(int (*function)(Thread & thread, void * parameter), void * parameter)
	: Thread(function, parameter)
{
	ASSERTION_COBJECT(this);

	unsigned long threadId;
	if ((handle = ::CreateThread(NULL, 0, &Win32Thread::threadFunction, this,
		CREATE_SUSPENDED, &threadId)) == NULL)
		throw EDEVICEERROR("Cannot create Win32 thread");

	suspended = true;
}

Win32Thread::~Win32Thread()
{
	ASSERTION_COBJECT(this);
	kill();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Win32Thread, Thread);
#endif

bool Win32Thread::isFinished()
{
	ASSERTION_COBJECT(this);

	if (handle == NULL)
		return true;

	unsigned long exitCode;
	if (!::GetExitCodeThread(handle, &exitCode))
		return true;

	if (exitCode == STILL_ACTIVE)
		return false;
	else
	{
		::CloseHandle(handle);
		handle = NULL;

		return true;
	}
}

bool Win32Thread::isSuspended()
{
	ASSERTION_COBJECT(this);
	return handle != NULL && suspended;
}

void Win32Thread::kill()
{
	ASSERTION_COBJECT(this);

	if (handle == NULL)
		return;

	::TerminateThread(handle, 1);
	::CloseHandle(handle);
	handle = NULL;
}

void Win32Thread::resume()
{
	ASSERTION_COBJECT(this);

	if (handle == NULL || !suspended)
		return;

	::ResumeThread(handle);
	suspended = false;
}

void Win32Thread::setExitCodeByThreadFunction(int exitCode)
{
	ASSERTION_COBJECT(this);
	setExitCode(exitCode);
}

void Win32Thread::setHandle(HANDLE handle)
{
	ASSERTION_COBJECT(this);
	this->handle = handle;
}

void Win32Thread::suspend()
{
	ASSERTION_COBJECT(this);

	if (handle == NULL || suspended)
		return;

	::SuspendThread(handle);
	suspended = true;
}

DWORD WINAPI Win32Thread::threadFunction(LPVOID parameter)
{
	Win32Thread * thread = (Win32Thread *) parameter;

	int (*function)(Thread & thread, void * parameter) = thread->getFunction();
	void * functionParameter = thread->getParameter();

	int exitCode = function(*thread, functionParameter);
	thread->setExitCodeByThreadFunction(exitCode);

	return 0;
}
