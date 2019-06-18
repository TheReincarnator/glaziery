/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Layouter::Layouter()
{
	ASSERTION_COBJECT(this);
}

Layouter::~Layouter()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Layouter, CObject);
#endif
