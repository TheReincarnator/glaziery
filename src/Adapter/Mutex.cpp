/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const long Mutex::NO_TIMEOUT = -1;

Mutex::~Mutex()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Mutex, CObject);
#endif
