/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Win32Mutex::Win32Mutex()
{
	ASSERTION_COBJECT(this);

	if ((handle = ::CreateMutex(NULL, false, NULL)) == NULL)
		throw EDEVICEERROR("Cannot create Win32 mutex");
}

Win32Mutex::~Win32Mutex()
{
	ASSERTION_COBJECT(this);

	if (handle != NULL)
	{
		::CloseHandle(handle);
		handle = NULL;
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Win32Mutex, Mutex);
#endif

bool Win32Mutex::lock(long timeout)
{
	ASSERTION_COBJECT(this);

	while (true)
	{
		unsigned long result = ::WaitForSingleObject(handle, timeout >= 0 ? timeout : INFINITE);
		if (result == WAIT_OBJECT_0 || result == WAIT_ABANDONED)
			return true;
		else if (result == WAIT_TIMEOUT && timeout >= 0)
			return false;
	}
}

void Win32Mutex::release()
{
	ASSERTION_COBJECT(this);
	::ReleaseMutex(handle);
}
