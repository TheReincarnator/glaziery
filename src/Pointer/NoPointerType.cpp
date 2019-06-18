/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


void NoPointerType::draw(DrawingContext & context, Vector position)
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(NoPointerType, PointerType);
#endif
