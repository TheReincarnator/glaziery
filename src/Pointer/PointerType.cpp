/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PointerType::~PointerType()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PointerType, CObject);
#endif

void PointerType::onPointerMoved(Vector newPosition, Vector oldPosition)
{
	ASSERTION_COBJECT(this);
}
