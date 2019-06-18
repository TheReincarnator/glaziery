/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


InputEvent::InputEvent(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	this->option1 = option1;
	this->option2 = option2;
}

InputEvent::~InputEvent()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(InputEvent, CObject);
#endif
